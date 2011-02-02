// Native Nintendo Wii implementation of the wiimote 
// -------------------------------------------------------------------
// Copyright (C) 2007 OpenEngine.dk (See AUTHORS)
//
// This program is free software; It is covered by the GNU General
// Public License version 2 or any later version.
// See the GNU General Public License for more details (see LICENSE).
//--------------------------------------------------------------------


#ifndef _OE_NATIVE_WII_MOTE_MANAGER_H_
#define _OE_NATIVE_WII_MOTE_MANAGER_H_

#include <Core/Event.h>
#include <Core/IModule.h>
#include <vector>

#include <wiiuse/wpad.h>

namespace OpenEngine {
namespace Devices {

class WiiMote;

// class WiiMoteFoundEventArg {
// public:
//     WiiMote* wiimote;
//     WiiMoteFoundEventArg(WiiMote* m) : wiimote(m) {}
// };

class WiiMoteConnectEventArg {
public:
    WiiMote& mote;
    WiiMoteConnectEventArg(WiiMote& mote) : mote(mote) {}
};

class WiiMoteDisconnectEventArg {
public:
    WiiMote& mote;
    WiiMoteDisconnectEventArg(WiiMote& mote) : mote(mote) {}
};


/**
 * Manage wiimote connections on the Nintendo Wii.
 *
 * @class WiiMoteManager WiiMoteManager.h ons/WiiMote/Devices/WiiMoteManager.h
 */
class WiiMoteManager : public Core::IModule
{
private:
    // Core::Event<WiiMoteFoundEventArg> foundEvent;
    Core::Event<WiiMoteConnectEventArg> connectEvent;
    Core::Event<WiiMoteDisconnectEventArg> disconnectEvent;

    static void datacb_static(s32 chan, const WPADData *data);
    void datacb(s32 chan, const WPADData *data);
    //int nmotes;
    WiiMote* motes[4];
public:
    WiiMoteManager();
    
    void LookForMote();
    void Disconnect(WiiMote* mote);

    virtual void Handle(Core::InitializeEventArg arg);
    virtual void Handle(Core::ProcessEventArg arg);
    virtual void Handle(Core::DeinitializeEventArg arg);

    // Core::IEvent<WiiMoteFoundEventArg>& WiiMoteFoundEvent() {
    //     return foundEvent;
    // }

    Core::IEvent<WiiMoteConnectEventArg>& WiiMoteConnectEvent() {
        return connectEvent;
    }

    Core::IEvent<WiiMoteDisconnectEventArg>& WiiMoteDisconnectEvent() {
        return disconnectEvent;
    }

};

} // NS Devices
} // NS OpenEngine

#endif // _OE_WII_MOTE_MANAGER_H_
