// Nintendo Wii light setup preprocessor implementation.
// -------------------------------------------------------------------
// Copyright (C) 2007 OpenEngine.dk (See AUTHORS) 
// 
// This program is free software; It is covered by the GNU General 
// Public License version 2 or any later version. 
// See the GNU General Public License for more details (see LICENSE). 
//--------------------------------------------------------------------

#ifndef _OE_WII_LIGHT_VISITOR_H_
#define _OE_WII_LIGHT_VISITOR_H_

#include <Renderers/IRenderer.h>
#include <Scene/ISceneNodeVisitor.h>
#include <Core/IListener.h>
#include <Math/Matrix.h>

#include <stack>

namespace OpenEngine {

    //forward declarations
    namespace Scene {
        class TransformationNode;
        class PointLightNode;
        class DirectionalLightNode;
        class SpotLightNode;
    }


namespace Renderers {

using OpenEngine::Scene::TransformationNode;
using OpenEngine::Scene::PointLightNode;
using OpenEngine::Scene::DirectionalLightNode;
using OpenEngine::Scene::SpotLightNode;
using OpenEngine::Scene::ISceneNodeVisitor;
using OpenEngine::Core::IListener;
using OpenEngine::Renderers::IRenderer;
using OpenEngine::Renderers::RenderingEventArg;
using Math::Matrix;

using std::stack;

/**
 * Setup lighting on the Nintendo Wii
 *
 * @class WiiLightVisitor WiiLightVisitor.h Renderers/OpenGL/WiiLightVisitor.h
 */
class WiiLightVisitor: public ISceneNodeVisitor, public IListener<RenderingEventArg> {
private:
    //float pos[4], dir[4];
    Vector<4,float> pos, dir;
    unsigned short count;
    stack<Matrix<4,4,float> > mv;

public:

    WiiLightVisitor(); 
    ~WiiLightVisitor();
        
    void Handle(RenderingEventArg arg);
    void VisitTransformationNode(TransformationNode* node);
    void VisitDirectionalLightNode(DirectionalLightNode* node);
    void VisitPointLightNode(PointLightNode* node);
    void VisitSpotLightNode(SpotLightNode* node);
};

} // NS Renderers
} // NS OpenEngine

#endif
