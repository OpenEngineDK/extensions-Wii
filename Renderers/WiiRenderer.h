// Nintendo Wii renderer implementation.
// -------------------------------------------------------------------
// Copyright (C) 2007 OpenEngine.dk (See AUTHORS) 
// 
// This program is free software; It is covered by the GNU General 
// Public License version 2 or any later version. 
// See the GNU General Public License for more details (see LICENSE). 
//--------------------------------------------------------------------

#ifndef _OE_WII_RENDERER_H_
#define _OE_WII_RENDERER_H_

#include <Renderers/IRenderer.h>

// ogc stuff
#include <gccore.h>


namespace OpenEngine {
namespace Renderers {

using Core::Event;

/**
 * Nintendo wii renderer using ogc
 *
 * @class WiiRenderer WiiRenderer.h Renderers/WiiRenderer.h
 */
class WiiRenderer : public IRenderer {
private:
    bool init;
    Vector<4,float> bgColor;

    // Event lists for the rendering phases.
    Event<RenderingEventArg> initialize;
    Event<RenderingEventArg> preProcess;
    Event<RenderingEventArg> process;
    Event<RenderingEventArg> postProcess;
    Event<RenderingEventArg> deinitialize;

public:
    WiiRenderer();
    virtual ~WiiRenderer();

    void Handle(Renderers::InitializeEventArg arg);
    void Handle(Renderers::DeinitializeEventArg arg);
    void Handle(Renderers::ProcessEventArg arg);

    /**
     * Event lists for the rendering phases.
     */
    IEvent<RenderingEventArg>& InitializeEvent();
    IEvent<RenderingEventArg>& PreProcessEvent();
    IEvent<RenderingEventArg>& ProcessEvent();
    IEvent<RenderingEventArg>& PostProcessEvent();
    IEvent<RenderingEventArg>& DeinitializeEvent();

    bool BufferSupport();
    bool FrameBufferSupport();

    void SetBackgroundColor(Vector<4,float> color);
    Vector<4,float> GetBackgroundColor();

    void ApplyViewingVolume(Display::IViewingVolume& volume);
    void LoadTexture(ITexture2DPtr texr);
    void LoadTexture(ITexture2D* texr);
    void LoadTexture(ITexture3DPtr texr);
    void LoadTexture(ITexture3D* texr);
    void RebindTexture(ITexture2DPtr texr, unsigned int x, unsigned int y, unsigned int w, unsigned int h);
    void RebindTexture(ITexture2D* texr, unsigned int x, unsigned int y, unsigned int w, unsigned int h);
    void RebindTexture(ITexture3DPtr texr, unsigned int x, unsigned int y, unsigned int z, unsigned int w, unsigned int h, unsigned int d);
    void RebindTexture(ITexture3D* texr, unsigned int x, unsigned int y, unsigned int z, unsigned int w, unsigned int h, unsigned int d);
    void BindFrameBuffer(FrameBuffer* fb);
    void BindDataBlock(IDataBlock* bo);
    void RebindDataBlock(Resources::IDataBlockPtr ptr, unsigned int start, unsigned int end);
    void DrawFace(FacePtr face);
    void DrawFace(FacePtr face, Vector<3,float> color, float width = 1);
    void DrawLine(Line line, Vector<3,float> color, float width = 1);
    void DrawPoint(Vector<3,float> point, Vector<3,float> color , float size);
    void DrawSphere(Vector<3,float> center, float radius, Vector<3,float> color);
};

} // NS Renderers
} // NS OpenEngine

#endif // _OE_WII_RENDERER_H_
