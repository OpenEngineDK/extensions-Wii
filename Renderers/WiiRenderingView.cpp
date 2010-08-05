// Nintendo Wii rendering view.
// -------------------------------------------------------------------
// Copyright (C) 2007 OpenEngine.dk (See AUTHORS) 
// 
// This program is free software; It is covered by the GNU General 
// Public License version 2 or any later version. 
// See the GNU General Public License for more details (see LICENSE). 
//--------------------------------------------------------------------

#include <Renderers/WiiRenderingView.h>
#include <Core/Exceptions.h>
#include <Math/Matrix.h>
#include <Scene/ISceneNode.h>
#include <Scene/RenderNode.h>
#include <Scene/RenderStateNode.h>
#include <Scene/TransformationNode.h>
#include <Scene/MeshNode.h>
#include <Scene/GeometryNode.h>
#include <Scene/VertexArrayNode.h>
#include <Scene/PostProcessNode.h>
#include <Scene/BlendingNode.h>
#include <Display/IViewingVolume.h>

// ogc stuff
#include <gccore.h>

namespace OpenEngine {
namespace Renderers {

using namespace Core;
using namespace Scene;
using namespace Math;
using namespace Display;

WiiRenderingView::WiiRenderingView() {

}

WiiRenderingView::~WiiRenderingView() {

}

void WiiRenderingView::ApplyModelViewMatrix(Matrix<4,4,float> m) {
    // weird hack. gx requires 3x4 matrices, we have 4x4 so we
    // allocate 16 floats and copies the data from
    // Matrix<4,4,float>. But we only give the 3 first rows to gx.
    union MtxAndFloat {
        Mtx mtx;
        float m[16];
    };
    MtxAndFloat modelview;
    m.ToArray((float*)&modelview);
    GX_LoadPosMtxImm(modelview.mtx, GX_PNMTX0);
}

void WiiRenderingView::Handle(RenderingEventArg arg) {
    return;
#ifdef OE_SAFE
        if (arg.canvas.GetScene() == NULL) 
            throw Exception("Scene was NULL while rendering.");
        if (arg.canvas.GetViewingVolume() == NULL) 
            throw Exception("ViewingVolume was NULL while rendering.");
        if (!mv.empty())
            throw Exception("Assertion failed: ModelView stack not empty.");
#endif

        IViewingVolume* volume = arg.canvas.GetViewingVolume();
        Matrix<4,4,float> m = volume->GetViewMatrix();
        mv.push(m);
        ApplyModelViewMatrix(m);
        this->arg = &arg;
        arg.canvas.GetScene()->Accept(*this);
        this->arg = NULL;        
        mv.pop();
}
    
/**
 * Process a render node.
 *
 * @param node Rendering node to apply.
 */
void WiiRenderingView::VisitRenderNode(RenderNode* node) {
    node->Apply(*arg, *this);
}

/**
 * Process a render state node.
 *
 * @param node Render state node to apply.
 */
void WiiRenderingView::VisitRenderStateNode(Scene::RenderStateNode* node) {
    //@todo IMPLEMENT!!!
    node->VisitSubNodes(*this);
}

/**
 * Process a transformation node.
 *
 * @param node Transformation node to apply.
 */
void WiiRenderingView::VisitTransformationNode(TransformationNode* node) {
    // push transformation matrix
    Matrix<4,4,float> m = node->GetTransformationMatrix() * mv.top();
    mv.push(m);
    ApplyModelViewMatrix(m);
    // traverse sub nodes
    node->VisitSubNodes(*this);
    // pop transformation matrix
    mv.pop();
    // apply previous transformation matrix
    ApplyModelViewMatrix(mv.top());
}


/**
 * Process a mesh node.
 *
 * @param node Mesh node to render
 */
void WiiRenderingView::VisitMeshNode(MeshNode* node) {
    node->VisitSubNodes(*this);
}

/**
 * Process a geometry node.
 *
 * @param node Geometry node to render
 */
void WiiRenderingView::VisitGeometryNode(GeometryNode* node) {
    throw Exception("Geometry nodes are deprecated and not supported by the Wii renderer");
}

/**
 *   Process a Vertex Array Node which may contain a list of vertex arrays
 *   sorted by texture id.
 */
void WiiRenderingView::VisitVertexArrayNode(VertexArrayNode* node){
    node->VisitSubNodes(*this);
}

// void WiiRenderingView::VisitDisplayListNode(DisplayListNode* node) {
//     node->VisitSubNodes(*this);
// }

void WiiRenderingView::VisitPostProcessNode(PostProcessNode* node) {
    throw Exception("Post processing is not supported by the Wii renderer");
}
    
void WiiRenderingView::VisitBlendingNode(BlendingNode* node) {
    throw NotImplemented("Blending is currently not supported by the Wii renderer");
}

} // NS Renderers
} // NS OpenEngine
