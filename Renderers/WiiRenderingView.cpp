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
#include <Scene/MeshNode.h>
#include <Scene/GeometryNode.h>
#include <Scene/VertexArrayNode.h>
#include <Scene/PostProcessNode.h>
#include <Scene/BlendingNode.h>
#include <Display/IViewingVolume.h>
#include <Logging/Logger.h>

#include <Geometry/Mesh.h>
#include <Geometry/GeometrySet.h>
#include <Resources/DataBlock.h>

// ogc stuff
#include <gccore.h>

namespace OpenEngine {
namespace Renderers {

using namespace Core;
using namespace Scene;
using namespace Math;
using namespace Display;
using namespace Geometry;

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
    m.Transpose();
    m.ToArray((float*)&modelview);

    GX_LoadPosMtxImm(modelview.mtx, GX_PNMTX0);
}
    
void WiiRenderingView::Handle(RenderingEventArg arg) {
    logger.info << "Rendering VIEW" << logger.end;
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
    
    TransformationNode mt;
    mt.Move(-1.5, 0.0,-6.0);
    Matrix<4,4,float> m1 = mt.GetTransformationMatrix();
    rt.Rotate(0.0,0.0,0.1);
    Matrix<4,4,float> m2 = rt.GetTransformationMatrix();
    
    ApplyModelViewMatrix(m*m2*m1);
    
    // setup the vertex descriptor
    // tells the flipper to expect direct data
    GX_ClearVtxDesc();
    GX_SetVtxDesc(GX_VA_POS, GX_DIRECT);
    GX_SetVtxDesc(GX_VA_CLR0, GX_DIRECT);
    
	// setup the vertex attribute table
	// describes the data
	// args: vat location 0-7, type of data, data format, size, scale
	// so for ex. in the first call we are sending position data with
	// 3 values X,Y,Z of size F32. scale sets the number of fractional
	// bits for non float data.
	GX_SetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
	GX_SetVtxAttrFmt(GX_VTXFMT0, GX_VA_CLR0, GX_CLR_RGBA, GX_RGB8, 0);
    
	GX_SetNumChans(1);
	GX_SetNumTexGens(0);
	GX_SetTevOrder(GX_TEVSTAGE0, GX_TEXCOORDNULL, GX_TEXMAP_NULL, GX_COLOR0A0);
	GX_SetTevOp(GX_TEVSTAGE0, GX_PASSCLR);
    
    GX_Begin(GX_TRIANGLES, GX_VTXFMT0, 3);
    GX_Position3f32( 0.0f, 1.0f, 0.0f);		// Top
    GX_Color3f32(1.0f,0.0f,0.0f);			// Set The Color To Red
    GX_Position3f32(-1.0f,-1.0f, 0.0f);	// Bottom Left
    GX_Color3f32(0.0f,1.0f,0.0f);			// Set The Color To Green
    GX_Position3f32( 1.0f,-1.0f, 0.0f);	// Bottom Right
    GX_Color3f32(0.0f,0.0f,1.0f);			// Set The Color To Blue
    GX_End();

    
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
    mv.push(mv.top()*m);
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
    // setup the vertex descriptor
    // tells the flipper to expect direct data
    GX_ClearVtxDesc();
    GX_SetVtxDesc(GX_VA_POS, GX_DIRECT);
    GX_SetVtxDesc(GX_VA_CLR0, GX_DIRECT);
    GX_SetVtxDesc(GX_VA_NRM, GX_DIRECT);
    
	// setup the vertex attribute table
	// describes the data
	// args: vat location 0-7, type of data, data format, size, scale
	// so for ex. in the first call we are sending position data with
	// 3 values X,Y,Z of size F32. scale sets the number of fractional
	// bits for non float data.
	GX_SetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
	GX_SetVtxAttrFmt(GX_VTXFMT0, GX_VA_CLR0, GX_CLR_RGBA, GX_RGB8, 0);
	GX_SetVtxAttrFmt(GX_VTXFMT0, GX_VA_NRM, GX_NRM_XYZ, GX_F32, 0);
    
	GX_SetNumChans(1);
	GX_SetNumTexGens(0);
	GX_SetTevOrder(GX_TEVSTAGE0, GX_TEXCOORDNULL, GX_TEXMAP_NULL, GX_COLOR0A0);
	GX_SetTevOp(GX_TEVSTAGE0, GX_PASSCLR);

    Mesh* mesh = node->GetMesh().get();
    GeometrySet* gs = mesh->GetGeometrySet().get();
 
    IndicesPtr is = mesh->GetIndices();
    DataBlock<3,float>* vs = (DataBlock<3,float>*)gs->GetVertices().get();
    DataBlock<3,float>* ns = (DataBlock<3,float>*)gs->GetNormals().get();
    IDataBlock* cs = gs->GetColors().get();
    u8 type = 0;
    switch (mesh->GetType()) {
    case TRIANGLES:
        logger.info << "Draw Triangles" << logger.end;
        type = GX_TRIANGLES;
        break;
    default:
        logger.warning << "Unsupported Geometry" << logger.end;
    }
    if (type) {
        u16 count = mesh->GetDrawingRange();
        GX_Begin(type, GX_VTXFMT0, count);
        for (u16 i = mesh->GetIndexOffset(); i < count; ++i) {
            unsigned int index = (*is)[i][0];
            Vector<3,float> v = (*vs)[index];
            Vector<3,float> n = (*vs)[index];
            GX_Position3f32( v[0], v[1], v[2]);
            GX_Normal3f32( n[0], n[1], n[2]);
            GX_Color3f32(1.0f,1.0f,1.0f);
        }
        GX_End();
    }
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
