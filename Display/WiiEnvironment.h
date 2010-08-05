// WiiEnvironment 
// -------------------------------------------------------------------
// Copyright (C) 2007 OpenEngine.dk (See AUTHORS) 
// 
// This program is free software; It is covered by the GNU General 
// Public License version 2 or any later version. 
// See the GNU General Public License for more details (see LICENSE). 
//--------------------------------------------------------------------

#ifndef _OE_WII_ENVIRONMENT_H_
#define _OE_WII_ENVIRONMENT_H_

#include <Display/IEnvironment.h>
#include <Core/Exceptions.h>
#include <Display/WiiFrame.h>
//#include <Devices/WiiInput.h>

namespace OpenEngine {
namespace Display {

using Core::Exception;

/**
 * An environment for the nintendo wii.
 *
 * @class WiiEnvironment WiiEnvironment.h Display/WiiEnvironment.h
 */
class WiiEnvironment : public IEnvironment {
private:

    WiiFrame* frame;
    //    Devices::WiiInput* input;

public:

    WiiEnvironment() {
        frame = new WiiFrame();
        //input = new Devices::WiiInput();
    }

    void Handle(Core::InitializeEventArg arg) {
        frame->Handle(arg);
        //input->Handle(arg);
    }
    void Handle(Core::ProcessEventArg arg) {
        frame->Handle(arg);
        //input->Handle(arg);
    }
    void Handle(Core::DeinitializeEventArg arg) {
        frame->Handle(arg);
        //input->Handle(arg);
    }

    IFrame&             CreateFrame() { return *frame; }
    Devices::IMouse*    GetMouse()    { throw Exception("Not implemented: GetMouse()");    }
    Devices::IKeyboard* GetKeyboard() { throw Exception("Not implemented: GetKeyboard()"); }
    Devices::IJoystick* GetJoystick() { throw Exception("Not implemented: GetJoystick()"); }

};

} // NS Display
} // NS OpenEngine

#endif // _OE_WII_ENVIRONMENT_H_
