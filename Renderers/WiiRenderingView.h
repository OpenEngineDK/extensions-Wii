// Nintendo Wii rendering view.
// -------------------------------------------------------------------
// Copyright (C) 2007 OpenEngine.dk (See AUTHORS) 
// 
// This program is free software; It is covered by the GNU General 
// Public License version 2 or any later version. 
// See the GNU General Public License for more details (see LICENSE). 
//--------------------------------------------------------------------

#ifndef _OE_WII_RENDERING_VIEW_H_
#define _OE_WII_RENDERING_VIEW_H_

#include <Renderers/IRenderingView.h>
#include <Math/Matrix.h>

#include <stack>

namespace OpenEngine {
namespace Renderers {

using namespace Scene;
using Math::Matrix;
using std::stack;

/**
 * Traverse and render scene on the Wii.
 */
class WiiRenderingView 
    : public IRenderingView {    
private:
    stack<Matrix<4,4,float> > mv;
    inline void ApplyModelViewMatrix(Matrix<4,4,float> m);
protected:
    // save the event arg 
    RenderingEventArg* arg;
public:
    WiiRenderingView();
    virtual ~WiiRenderingView();
    void VisitMeshNode(MeshNode* node);
    void VisitGeometryNode(GeometryNode* node);
    void VisitVertexArrayNode(VertexArrayNode* node);
    void VisitTransformationNode(TransformationNode* node);
    void VisitRenderStateNode(RenderStateNode* node);
    void VisitRenderNode(RenderNode* node);
    // void VisitDisplayListNode(DisplayListNode* node);
    void VisitBlendingNode(BlendingNode* node);
    void VisitPostProcessNode(PostProcessNode* node);
    void Handle(RenderingEventArg arg);
};

} // NS Renderers
} // NS OpenEngine

#endif // _OE_WII_RENDERING_VIEW_H_
