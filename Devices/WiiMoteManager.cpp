#include <Devices/WiiMoteManager.h>
#include <Devices/WiiMote.h>
#include <Logging/Logger.h>

#include <Math/Math.h>

#include <stdlib.h>

namespace OpenEngine {
namespace Devices {

using namespace Core;
using namespace std;

#define CHK_BTN(oebtn, wpadbtn)                     \
    barg.btn = oebtn;				     	        \
    if (data->btns_d & wpadbtn) {                   \
    	barg.type = EVENT_PRESS;				    \
	    mote->WiiButtonEvent().Notify(barg);        \
    }                                               \
    else if (data->btns_u & wpadbtn) {              \
    	barg.type = EVENT_RELEASE;	                \
	    mote->WiiButtonEvent().Notify(barg);        \
    }                                           

WiiMoteManager* theManager;

int chan2i(s32 chan) {
    switch (chan) {
    case WPAD_CHAN_0: return 0;
    case WPAD_CHAN_1: return 1;
    case WPAD_CHAN_2: return 2;
    case WPAD_CHAN_3: return 3;
    default: return -1;
    }
}

s32 i2chan(int i) {
    switch (i) {
    case 0: return WPAD_CHAN_0;
    case 1: return WPAD_CHAN_1;
    case 2: return WPAD_CHAN_2;
    case 3: return WPAD_CHAN_3;
    default: return -1;
    }
}

void WiiMoteManager::datacb_static(s32 chan, const WPADData *data) {
    theManager->datacb(chan, data);
}

void WiiMoteManager::datacb(s32 chan, const WPADData *data) {
    if (data->err != 0) return;
    WiiMote* mote = motes[chan2i(chan)];
    if (!mote->connected) {
        mote->connected = true;
        connectEvent.Notify(WiiMoteConnectEventArg(*mote));
    }

    // buttons
    mote->btns = data->btns_h;

    WiiButtonEventArg barg(*mote);
    CHK_BTN(WII_REMOTE_2	, WPAD_BUTTON_2)
    CHK_BTN(WII_REMOTE_1	, WPAD_BUTTON_1)
    CHK_BTN(WII_REMOTE_A	, WPAD_BUTTON_A)
    CHK_BTN(WII_REMOTE_B	, WPAD_BUTTON_B)
    CHK_BTN(WII_REMOTE_MINUS, WPAD_BUTTON_MINUS)
    CHK_BTN(WII_REMOTE_HOME	, WPAD_BUTTON_HOME)
    CHK_BTN(WII_REMOTE_PLUS	, WPAD_BUTTON_PLUS)
    CHK_BTN(WII_REMOTE_UP	, WPAD_BUTTON_UP)
    CHK_BTN(WII_REMOTE_DOWN	, WPAD_BUTTON_DOWN)
    CHK_BTN(WII_REMOTE_LEFT	, WPAD_BUTTON_LEFT)
    CHK_BTN(WII_REMOTE_RIGHT, WPAD_BUTTON_RIGHT)

    // acceleration
    WiiAccelerationEventArg aarg(*mote);
    const float d2r = Math::PI/180.0;
    aarg.orient = Vector<3,float>(data->orient.pitch*d2r,  data->orient.yaw*d2r, data->orient.roll*d2r);
    aarg.gforce = Vector<3,float>(data->gforce.x, data->gforce.y, data->gforce.z);
    mote->WiiAccelerationEvent().Notify(aarg);

    WiiIREventArg iarg(*mote);
    iarg.point = Vector<2,float> (data->ir.sx, data->ir.sy);
    for (u8 i = 0; i < 4; ++i) {
        iarg.dots[i] = Vector<2,short>(data->ir.dot[i].rx, data->ir.dot[i].ry);
    }
    iarg.angle = d2r*data->ir.angle;
    mote->WiiIREvent().Notify(iarg);
}

WiiMoteManager::WiiMoteManager() {
    theManager = this;
    motes[0] = new WiiMote(WII_PLAYER_ID_1, *this);
    motes[1] = new WiiMote(WII_PLAYER_ID_2, *this);
    motes[2] = new WiiMote(WII_PLAYER_ID_3, *this);
    motes[3] = new WiiMote(WII_PLAYER_ID_4, *this);
}

void WiiMoteManager::LookForMote() {
}

void WiiMoteManager::Disconnect(WiiMote* mote) {
    if (!mote->connected) return;
    for (u8 i = 0; i < 4; ++i) {
        if (motes[i] == mote) {
            WPAD_Disconnect(i2chan(i));
            mote->connected = false;
            disconnectEvent.Notify(WiiMoteDisconnectEventArg(*mote));
            return;
        }
    }
}

void WiiMoteManager::Handle(InitializeEventArg arg) {
    WPAD_Init();
    WPAD_SetDataFormat(WPAD_CHAN_ALL, WPAD_FMT_BTNS_ACC_IR);
	//WPAD_SetVRes(WPAD_CHAN_ALL, rmode->fbWidth, rmode->xfbHeight);
}

void WiiMoteManager::Handle(ProcessEventArg arg) {
    WPAD_ReadPending(WPAD_CHAN_ALL, datacb_static);
}

void WiiMoteManager::Handle(DeinitializeEventArg arg) {
    WPAD_Shutdown();
}

}
}
