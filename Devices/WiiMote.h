//
// -------------------------------------------------------------------
// Copyright (C) 2007 OpenEngine.dk (See AUTHORS)
//
// This program is free software; It is covered by the GNU General
// Public License version 2 or any later version.
// See the GNU General Public License for more details (see LICENSE).
//--------------------------------------------------------------------


#ifndef _OE_WII_MOTE_H_
#define _OE_WII_MOTE_H_

#include <Core/Event.h>
#include <Devices/Symbols.h>

#include <Math/Vector.h>

namespace OpenEngine {
namespace Devices {

    class WiiMoteManager;
    class WiiMote;

    using Math::Vector;
    enum WiiButton {
        WII_REMOTE_NONE   = 0,
        WII_REMOTE_2      = 1<<0,
        WII_REMOTE_1      = 1<<1,
        WII_REMOTE_B      = 1<<2,
        WII_REMOTE_A      = 1<<3,
        WII_REMOTE_MINUS  = 1<<4,
        WII_REMOTE_HOME   = 1<<5,
        WII_REMOTE_LEFT   = 1<<6,
        WII_REMOTE_RIGHT  = 1<<7,
        WII_REMOTE_DOWN   = 1<<8,
        WII_REMOTE_UP     = 1<<9,
        WII_REMOTE_PLUS   = 1<<10
    };

    enum WiiPlayerID {
        WII_PLAYER_ID_NONE = 0,
        WII_PLAYER_ID_1    = 1,
        WII_PLAYER_ID_2    = 2,
        WII_PLAYER_ID_3    = 3,
        WII_PLAYER_ID_4    = 4
    };

    struct WiiButtonEventArg {
        WiiMote& mote;
        ButtonEvent type;
        WiiButton btn;
        WiiButtonEventArg(WiiMote& mote): mote(mote) {}
    };

    struct WiiAccelerationEventArg {
        WiiMote& mote;
        Vector<3,float> orient;
        Vector<3,float> gforce;
        WiiAccelerationEventArg(WiiMote& mote): mote(mote) {}
    };

    struct WiiIREventArg {
        WiiMote& mote;
        Vector<2,float> point;
        Vector<2,short> dots[4];
        float angle;
        WiiIREventArg(WiiMote& mote): mote(mote) {}
    };

class WiiMote {
private:
    friend class WiiMoteManager;
    WiiPlayerID playerid;
    WiiMoteManager& manager;
    bool connected;
    unsigned int btns;
    Core::Event<WiiButtonEventArg> buttonEvent;
    Core::Event<WiiAccelerationEventArg> accEvent;
    Core::Event<WiiIREventArg> irEvent;
    WiiMote(WiiPlayerID playerid, WiiMoteManager& manager);
public:
    void Disconnect();
    Core::IEvent<WiiButtonEventArg>& WiiButtonEvent() { return buttonEvent;}
    Core::IEvent<WiiAccelerationEventArg>& WiiAccelerationEvent() { return accEvent;}
    Core::IEvent<WiiIREventArg>& WiiIREvent() { return irEvent;}
};

} // NS Devices
} // NS OpenEngine

#endif // _OE_WII_MOTE_H_
