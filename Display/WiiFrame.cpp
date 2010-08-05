// Nintendo Wii implementation of IFrame
// -------------------------------------------------------------------
// Copyright (C) 2007 OpenEngine.dk (See AUTHORS) 
// 
// This program is free software; It is covered by the GNU General 
// Public License version 2 or any later version. 
// See the GNU General Public License for more details (see LICENSE). 
//--------------------------------------------------------------------

#include <Core/Exceptions.h>
#include <Display/WiiFrame.h>
#include <Display/StereoCamera.h>
#include <Display/ViewingVolume.h>

// temporary input handle
#include <wiiuse/wpad.h>


namespace OpenEngine {
namespace Display {

using OpenEngine::Core::Exception;
    
WiiFrame::WiiFrame()
    : width(0)
    , height(0)
    , depth(32)
    , options(FrameOption()) 
    , init(false) 
    , canvas(NULL)
    , fc(FrameCanvas(*this))
    , rmode(NULL)
    , fb(0)
{
    frameBuffer[0] = NULL;
    frameBuffer[1] = NULL;
}

WiiFrame::~WiiFrame() {

}

bool WiiFrame::IsFocused() const {
    return true;
}

unsigned int WiiFrame::GetWidth() const {
    return width;
}

unsigned int WiiFrame::GetHeight() const {
    return height;
}

unsigned int WiiFrame::GetDepth() const {
    return depth;
}

FrameOption WiiFrame::GetOptions() const {
    return options;
}

bool WiiFrame::GetOption(const FrameOption option) const {
    return (option & GetOptions()) == option;
}

void WiiFrame::SetWidth(const unsigned int width) {
    if (!init) this->width = width;
}

void WiiFrame::SetHeight(const unsigned int height) {
    if (!init) this->height = height;
}

void WiiFrame::SetDepth(const unsigned int depth) {
    if (!init) this->depth = depth;
}

void WiiFrame::SetOptions(const FrameOption options) {
    this->options = options;
    // for now ignore all options.
}

void WiiFrame::ToggleOption(const FrameOption option) {
    FrameOption opt = FrameOption(options ^ option);
    SetOptions(opt);
}

void WiiFrame::Handle(Core::InitializeEventArg arg) {
    if (init) return;

    // initialize the video subsystem
	VIDEO_Init();

    // temporary controller init
	WPAD_Init();

    // get preferred mode (e.g resolution, origin, tv mode and that kind of stuff)
	rmode = VIDEO_GetPreferredMode(NULL);
    width = rmode->fbWidth;
    height = rmode->xfbHeight;
    // allocate 2 framebuffers for double buffering
	frameBuffer[0] = MEM_K0_TO_K1(SYS_AllocateFramebuffer(rmode));
	frameBuffer[1] = MEM_K0_TO_K1(SYS_AllocateFramebuffer(rmode));
    //setup video
	VIDEO_Configure(rmode);
	VIDEO_SetNextFramebuffer(frameBuffer[fb]);
	VIDEO_SetBlack(FALSE);
	VIDEO_Flush();
    init = true;
    ((IListener<Display::InitializeEventArg>*)canvas)->Handle(Display::InitializeEventArg(fc));
}

void WiiFrame::Handle(Core::ProcessEventArg arg) {
#ifdef OE_SAFE
    if (!init) throw Exception("Wiiframe must be initialized before ProcessEventArg.");
#endif
    // temporary controller exit handle
    WPAD_ScanPads();
    if (WPAD_ButtonsDown(0) & WPAD_BUTTON_HOME) exit(0);

    if (!canvas) return; 
    ((IListener<Display::ProcessEventArg>*)canvas)->Handle(ProcessEventArg(fc, arg.start, arg.approx));
    VIDEO_WaitVSync();
    fb ^= 1;		// flip framebuffer
    GX_SetZMode(GX_TRUE, GX_LEQUAL, GX_TRUE);
    GX_SetColorUpdate(GX_TRUE);
    GX_CopyDisp(frameBuffer[fb],GX_TRUE);
    VIDEO_SetNextFramebuffer(frameBuffer[fb]);
    VIDEO_Flush();
}

void WiiFrame::Handle(Core::DeinitializeEventArg arg) {
    if (!init) return;
    ((IListener<Display::DeinitializeEventArg>*)canvas)->Handle(DeinitializeEventArg(fc));
    init = false;
}

} // NS Display
} // NS OpenEngine
