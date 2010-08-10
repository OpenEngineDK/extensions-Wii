// Nintendo Wii drawing canvas implementation
// -------------------------------------------------------------------
// Copyright (C) 2007 OpenEngine.dk (See AUTHORS) 
// 
// This program is free software; It is covered by the GNU General 
// Public License version 2 or any later version. 
// See the GNU General Public License for more details (see LICENSE). 
//--------------------------------------------------------------------

#include <Display/WiiCanvas.h>
#include <Renderers/IRenderer.h>
#include <Core/Exceptions.h>

namespace OpenEngine {
namespace Display {

    using namespace Core;

WiiCanvas::WiiCanvas()
    : IRenderCanvas()
    , init(false)
{

}

WiiCanvas::~WiiCanvas() {
    
}

void WiiCanvas::Handle(Display::InitializeEventArg arg) {
    if (init) return;
    width = arg.canvas.GetWidth();
    height = arg.canvas.GetHeight();
    
    if (renderer)
        ((IListener<Renderers::InitializeEventArg>*)renderer)->Handle(Renderers::InitializeEventArg(*this));
    init = true;
}

void WiiCanvas::Handle(ResizeEventArg arg) {

}

void WiiCanvas::Handle(Display::ProcessEventArg arg) {
    #ifdef OE_SAFE
    if (renderer == NULL) throw new Exception("NULL renderer in WiiCanvas.");
    #endif
    ((IListener<Renderers::ProcessEventArg>*)renderer)
        ->Handle(Renderers::ProcessEventArg(*this, arg.start, arg.approx));
}

void WiiCanvas::Handle(DeinitializeEventArg arg) { 
    if (!init) return;
    ((IListener<Renderers::DeinitializeEventArg>*)renderer)->Handle(Renderers::DeinitializeEventArg(*this));
    init = false;
}

unsigned int WiiCanvas::GetWidth() const {
    return width;
}

unsigned int WiiCanvas::GetHeight() const {
    return height;
}
    
void WiiCanvas::SetWidth(const unsigned int width) {
    this->width = width;
}

void WiiCanvas::SetHeight(const unsigned int height) {
    this->height = height;
}
    
ITexture2DPtr WiiCanvas::GetTexture() {
    throw NotImplemented("GetTexture() not supported on WiiCanvas");
}


} // NS Display
} // NS OpenEngine
