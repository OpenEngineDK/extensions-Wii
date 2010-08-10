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
#include <Logging/Logger.h>

// temporary input handle
#include <wiiuse/wpad.h>

#include <malloc.h>
#include <string.h>

#include <debug.h>

#define HEST() logger.info << "line: " << __LINE__ << "in file: " << __FILE__ << logger.end 

#define DEFAULT_FIFO_SIZE	(256*1024)

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

    logger.info << "Preferred video mode:" << logger.end;
    logger.info << "fbWidth: " << rmode->fbWidth << " xfbHeight: " 
                << rmode->xfbHeight << " efbHeight: " << rmode->efbHeight << logger.end;
    logger.info << "viWidth: " << rmode->viWidth << " viHeight: " << rmode->viHeight << logger.end;

    width = rmode->fbWidth;
    height = rmode->efbHeight;
    // allocate 2 framebuffers for double buffering
	frameBuffer[0] = MEM_K0_TO_K1(SYS_AllocateFramebuffer(rmode));
	frameBuffer[1] = MEM_K0_TO_K1(SYS_AllocateFramebuffer(rmode));
    //setup video
	VIDEO_Configure(rmode);
	VIDEO_SetNextFramebuffer(frameBuffer[fb]);
	VIDEO_SetBlack(FALSE);
	VIDEO_Flush();
	VIDEO_WaitVSync();
    if(rmode->viTVMode&VI_NON_INTERLACE) VIDEO_WaitVSync();

	// setup the fifo and then init the flipper
	void *gp_fifo = NULL;
	gp_fifo = memalign(32,DEFAULT_FIFO_SIZE);
	memset(gp_fifo,0,DEFAULT_FIFO_SIZE);
	GX_Init(gp_fifo,DEFAULT_FIFO_SIZE);

    // setup copy environment from efb to xfb. 
    // frame buffer size often varies from screen resolution
    // so filtering and scaling must be applied.
	float yscale = GX_GetYScaleFactor(rmode->efbHeight,rmode->xfbHeight);
	unsigned int lines = GX_SetDispCopyYScale(yscale);
	GX_SetDispCopySrc(0,0,rmode->fbWidth,rmode->efbHeight);
	GX_SetDispCopyDst(rmode->fbWidth,lines);
	GX_SetCopyFilter(rmode->aa,rmode->sample_pattern,GX_TRUE,rmode->vfilter);
	GX_SetFieldMode(rmode->field_rendering,((rmode->viHeight==2*rmode->xfbHeight)?GX_ENABLE:GX_DISABLE));
	GX_SetDispCopyGamma(GX_GM_1_0);
    GX_SetZMode(GX_TRUE, GX_LEQUAL, GX_TRUE);
    GX_SetColorUpdate(GX_TRUE);

    //clear the first visible fb to black
    GXColor bg = {0x00, 0x00, 0x00, 0x00};
	GX_SetCopyClear(bg, 0x00ffffff);
    GX_CopyDisp(frameBuffer[fb],GX_TRUE);
	VIDEO_SetBlack(FALSE);
    VIDEO_Flush();
    VIDEO_WaitVSync();


	// // setup the vertex descriptor
	// // tells the flipper to expect direct data
	// GX_ClearVtxDesc();
	// GX_SetVtxDesc(GX_VA_POS, GX_DIRECT);
 	// GX_SetVtxDesc(GX_VA_CLR0, GX_DIRECT);
 
	// // setup the vertex attribute table
	// // describes the data
	// // args: vat location 0-7, type of data, data format, size, scale
	// // so for ex. in the first call we are sending position data with
	// // 3 values X,Y,Z of size F32. scale sets the number of fractional
	// // bits for non float data.
	// GX_SetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
	// GX_SetVtxAttrFmt(GX_VTXFMT0, GX_VA_CLR0, GX_CLR_RGBA, GX_RGB8, 0);
 
	// GX_SetNumChans(1);
	// GX_SetNumTexGens(0);
	// GX_SetTevOrder(GX_TEVSTAGE0, GX_TEXCOORDNULL, GX_TEXMAP_NULL, GX_COLOR0A0);
	// GX_SetTevOp(GX_TEVSTAGE0, GX_PASSCLR);

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
    if (canvas) { 
        ((IListener<Display::ProcessEventArg>*)canvas)->Handle(ProcessEventArg(fc, arg.start, arg.approx));
    }
    fb ^= 1;		// flip framebuffer
    GX_CopyDisp(frameBuffer[fb],GX_TRUE);
    VIDEO_WaitVSync();
    if(rmode->viTVMode&VI_NON_INTERLACE) VIDEO_WaitVSync();
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
