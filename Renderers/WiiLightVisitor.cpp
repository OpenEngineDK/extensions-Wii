// Nintendo Wii light setup preprocessor implementation.
// -------------------------------------------------------------------
// Copyright (C) 2007 OpenEngine.dk (See AUTHORS) 
// 
// This program is free software; It is covered by the GNU General 
// Public License version 2 or any later version. 
// See the GNU General Public License for more details (see LICENSE). 
//--------------------------------------------------------------------

#include <Renderers/WiiLightVisitor.h>
#include <Display/IViewingVolume.h>
#include <Scene/TransformationNode.h>
#include <Scene/DirectionalLightNode.h>
#include <Scene/PointLightNode.h>
#include <Scene/SpotLightNode.h>
#include <Core/Exceptions.h>

#include <Logging/Logger.h>

// ogc stuff
#include <gccore.h>

#define MAX_LIGHTS 8

namespace OpenEngine {
namespace Renderers {

using namespace Math;
using namespace Core;
using namespace Display;

WiiLightVisitor::WiiLightVisitor()
    : pos(Vector<4,float>(0.0,0.0,0.0,1.0))
    , dir(Vector<4,float>(0.0,-1.0,0.0,1.0))
    , count(0)
{
}

WiiLightVisitor::~WiiLightVisitor() {}
        
void WiiLightVisitor::VisitTransformationNode(TransformationNode* node) {
    // push transformation matrix
    Matrix<4,4,float> m = node->GetTransformationMatrix()*mv.top();
    mv.push(m);
    // traverse sub nodes
    node->VisitSubNodes(*this);
    // pop transformation matrix
    mv.pop();
}
    
void WiiLightVisitor::VisitDirectionalLightNode(DirectionalLightNode* node) {
#ifdef OE_SAFE
    if (count >= MAX_LIGHTS) 
        throw Exception("Maximum number of lights exceeded.");
#endif
    ++count;

    // Vector<3,float> eyedir(0.0,0.0,-1.0);
    // Vector<3,float> lightdir(0.0,1.0,-1.0);
    // Vector<3,float> half(lightdir+eyedir);
    // half.Normalize();
    // GX_InitSpecularDirHA(&lobj, lightdir[0], lightdir[1], lightdir[2], half[0], half[1], half[2]);

    node->VisitSubNodes(*this);            
}
    
void WiiLightVisitor::VisitPointLightNode(PointLightNode* node) {
#ifdef OE_SAFE
    if (count >= MAX_LIGHTS) 
        throw Exception("Maximum number of lights exceeded.");
#endif
    ++count;
    GXLightObj lobj;
    Vector<4,float> diff = node->diffuse;
    Vector<4,u8> c(diff[0]*255, diff[1]*255, diff[2]*255, diff[3]*255);
    GXColor litcol;
    litcol.a = c[3];
    litcol.b = c[2];
    litcol.g = c[1];
    litcol.r = c[0];
    Matrix<4,4,float> top = mv.top();
    top.Transpose();
    Vector<4,float> p = top * pos;
    GX_InitLightPos(&lobj, p[0], p[1], p[2]);
	GX_InitLightColor(&lobj,litcol);
    //GX_InitLightAttnK(&lobj, node->constAtt, node->linearAtt, node->quadAtt);
    GX_InitLightAttnK(&lobj, 1.0, 0.0, 0.0);
	GX_LoadLightObj(&lobj, GX_LIGHT0);
    GX_SetChanCtrl(GX_COLOR0A0,GX_ENABLE,GX_SRC_REG,GX_SRC_REG,GX_LIGHT0,GX_DF_CLAMP,GX_AF_NONE);
    node->VisitSubNodes(*this);
}

void WiiLightVisitor::VisitSpotLightNode(SpotLightNode* node) {
#ifdef OE_SAFE
    if (count >= MAX_LIGHTS) 
        throw Exception("Maximum number of lights exceeded.");
#endif
    ++count;

    //GX_InitLightDir(&lobj, 0.0, 0.0, -1.0);
    node->VisitSubNodes(*this);            
}

void WiiLightVisitor::Handle(RenderingEventArg arg) {
    #ifdef OE_SAFE
    if (arg.canvas.GetScene() == NULL)
        throw Exception("Scene was NULL in WiiLightVisitor.");
    if (arg.canvas.GetViewingVolume() == NULL) 
        throw Exception("ViewingVolume was NULL in WiiLightVisitor.");
    if (!mv.empty())
        throw Exception("Assertion failed: ModelView stack not empty.");
    #endif
    count = 0;
    IViewingVolume* volume = arg.canvas.GetViewingVolume();
    Matrix<4,4,float> m = volume->GetViewMatrix();
    mv.push(m);
    arg.canvas.GetScene()->Accept(*this);
    mv.pop();
}

} // NS Renderers
} // NS OpenEngine
