// 
// -------------------------------------------------------------------
// Copyright (C) 2007 OpenEngine.dk (See AUTHORS)
//
// This program is free software; It is covered by the GNU General
// Public License version 2 or any later version.
// See the GNU General Public License for more details (see LICENSE).
//--------------------------------------------------------------------



#include <Devices/WiiMote.h>
#include <Devices/WiiMoteManager.h>

namespace OpenEngine {
namespace Devices {

WiiMote::WiiMote(WiiPlayerID playerid, WiiMoteManager& manager)
    : playerid(playerid)
    , manager(manager)
    , connected(false)
    , btns(0) 
{

}

void WiiMote::Disconnect() {
    manager.Disconnect(this);
}

} // NS Devices
} // NS OpenEngine


