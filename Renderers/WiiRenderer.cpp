// Nintendo Wii renderer implementation.
// -------------------------------------------------------------------
// Copyright (C) 2007 OpenEngine.dk (See AUTHORS) 
// 
// This program is free software; It is covered by the GNU General 
// Public License version 2 or any later version. 
// See the GNU General Public License for more details (see LICENSE). 
//--------------------------------------------------------------------

#include <Renderers/WiiRenderer.h>

#include <Core/Exceptions.h>
#include <Display/IViewingVolume.h>

#include <Logging/Logger.h>

namespace OpenEngine {
namespace Renderers {

using namespace Core; 
using namespace Display;
   
WiiRenderer::WiiRenderer(): init(false) {

}

WiiRenderer::~WiiRenderer() {

}

void WiiRenderer::Handle(Renderers::InitializeEventArg arg) {
    if (init) return;
    logger.info << "INITIALIZE RENDERER" << logger.end;
    bgColor = Vector<4,float>(0.0,0.0,0.0,1.0);
	GX_SetCullMode(GX_CULL_NONE);
    init = true;
}

void WiiRenderer::Handle(Renderers::ProcessEventArg arg) {
	// clears the bgColor and the z buffer
    GXColor bg = {bgColor[0] * 0xff, bgColor[1] * 0xff, bgColor[2] * 0xff, bgColor[3] * 0xff};
	GX_SetCopyClear(bg, 0x00ffffff);
    unsigned int width = arg.canvas.GetWidth();
    unsigned int height = arg.canvas.GetHeight();
    // GXRModeObj* rmode;
	// rmode = VIDEO_GetPreferredMode(NULL);

    IViewingVolume* volume = arg.canvas.GetViewingVolume();
    // If no viewing volume is set for the viewport ignore it.
    if (volume != NULL) {
        volume->SignalRendering(arg.approx);
		// do this before drawing
		// GX_SetViewport(0,0,rmode->fbWidth,rmode->efbHeight,0,1);
		GX_SetViewport(0,0,width,height,0,1);
        GX_SetScissor(0,0,width,height);
        // apply the volume
        ApplyViewingVolume(*volume);
    }
    
    // run the processing phases
    RenderingEventArg rarg(arg.canvas, *this, arg.start, arg.approx);

    // bgColor += Vector<4,float>(0.01,0.001,0.0001,0.0);
    this->preProcess.Notify(rarg);
    this->stage = RENDERER_PROCESS;
    this->process.Notify(rarg);
    this->stage = RENDERER_POSTPROCESS;
    this->postProcess.Notify(rarg);
    this->stage = RENDERER_PREPROCESS;
}

void WiiRenderer::Handle(Renderers::DeinitializeEventArg arg) {
    if (!init) return;
    this->stage = RENDERER_DEINITIALIZE;
    this->deinitialize.Notify(RenderingEventArg(arg.canvas, *this));
    init = false;
}

IEvent<RenderingEventArg>& WiiRenderer::InitializeEvent() {
    return initialize;
}

IEvent<RenderingEventArg>& WiiRenderer::PreProcessEvent() {
    return preProcess;
}

IEvent<RenderingEventArg>& WiiRenderer::ProcessEvent() {
    return process;
}

IEvent<RenderingEventArg>& WiiRenderer::PostProcessEvent() {
    return postProcess;
}
IEvent<RenderingEventArg>& WiiRenderer::DeinitializeEvent() {
    return deinitialize;
}

void WiiRenderer::ApplyViewingVolume(IViewingVolume& volume) {
    Mtx44 m;
    Matrix<4,4,float> proj = volume.GetProjectionMatrix();
    proj.ToArray((float*)&m);
    GX_LoadProjectionMtx(m, GX_PERSPECTIVE);
}

bool WiiRenderer::BufferSupport(){
    return false;
}

bool WiiRenderer::FrameBufferSupport(){
    return false;
}

void WiiRenderer::LoadTexture(ITexture2DPtr texr) {
    LoadTexture(texr.get());
}

void WiiRenderer::LoadTexture(ITexture2D* texr) {
    if (texr == NULL) return;

    // check if textures has already been bound.
    if (texr->GetID() != 0) return;

    //@todo load the damn texture (whatever that means)
}

void WiiRenderer::LoadTexture(ITexture3DPtr texr) {
    LoadTexture(texr.get());
}

void WiiRenderer::LoadTexture(ITexture3D* texr) {
    throw NotImplemented("LoadTexture(ITexture3D*) not supported.");
}

void WiiRenderer::RebindTexture(ITexture2DPtr texr, unsigned int xOffset, unsigned int yOffset, unsigned int width, unsigned int height) {
    RebindTexture(texr.get(), xOffset, yOffset, width, height);
}

void WiiRenderer::RebindTexture(ITexture2D* texr, unsigned int xOffset, unsigned int yOffset, unsigned int width, unsigned int height) {
    if (texr == NULL) return;

#ifdef OE_SAFE
    if (texr->GetID() == 0)
        throw Exception("Trying to rebind unbound texture.");
#endif
}

void WiiRenderer::RebindTexture(ITexture3DPtr texr, 
                                unsigned int xOffset, 
                                unsigned int yOffset,
                                unsigned int zOffset,
                                unsigned int width,
                                unsigned int height,
                                unsigned int depth) {
    RebindTexture(texr.get(), xOffset, yOffset, zOffset, width, height, depth);
}

void WiiRenderer::RebindTexture(ITexture3D* texr, 
                                unsigned int xOffset, 
                                unsigned int yOffset, 
                                unsigned int zOffset, 
                                unsigned int width, 
                                unsigned int height, 
                                unsigned int depth) {
    throw NotImplemented("RebindTexture(ITexture3D*,...) not supported.");
}

void WiiRenderer::BindFrameBuffer(FrameBuffer* fb){
    throw NotImplemented("BindFrameBuffer() not supported.");
}

void WiiRenderer::BindDataBlock(IDataBlock* bo){
    throw NotImplemented("BindDataBlock() not supported.");
}

void WiiRenderer::DrawFace(FacePtr f) {
    throw NotImplemented("DrawFace() not supported.");
}

void WiiRenderer::DrawFace(FacePtr face, Vector<3,float> color, float width) {
    throw NotImplemented("DrawFace() not supported.");
}

void WiiRenderer::DrawLine(Line line, Vector<3,float> color, float width) {
    throw NotImplemented("DrawLine() not supported.");
}

void WiiRenderer::DrawPoint(Vector<3,float> point, Vector<3,float> color , float size) {
    throw NotImplemented("DrawPoint() not supported.");

}

void WiiRenderer::SetBackgroundColor(Vector<4,float> color) {
    bgColor = color;
}

Vector<4,float> WiiRenderer::GetBackgroundColor() {
     return bgColor;
}

void WiiRenderer::DrawSphere(Vector<3,float> center, float radius, Vector<3,float> color) {
    throw NotImplemented("DrawSphere() not supported.");
}

} // NS Renderers
} // NS OpenEngine
