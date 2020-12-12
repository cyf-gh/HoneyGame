//
// Created by cyf-m on 2020/12/8.
//

#ifndef HONEYGAME_SCENEMAINGAME_H
#define HONEYGAME_SCENEMAINGAME_H

#include <SDL_events.h>
#include "Core/Log.h"
#include "EngineImpl/Scene.h"

using namespace __HGImpl::V1;

class SceneMainGame : public Scene {
public:
    void Update( void* pe ) override {
        auto pEvent = static_cast<SDL_Event *>( pe );
        switch ( pEvent->type ) {
            case SDL_KEYDOWN:
                HG_LOG_INFO( SDL_GetKeyName( pEvent->key.keysym.sym ) );
                HG_LOG_INFO( std::to_string( EngineImpl::GetEngine()->GetMainLoop().GetCurrentFps() ).append(" <- main loop fps").c_str() );
                HG_LOG_INFO( std::to_string( EngineImpl::GetEngine()->GetRenderLoop().GetCurrentFps() ).append(" <- render loop fps").c_str() );
                HG_LOG_INFO( std::to_string( EngineImpl::GetEngine()->GetUpdateLoop().GetCurrentFps() ).append(" <- update loop fps").c_str() );
                break;
            case SDL_QUIT:
                HG_LOG_INFO( "bye!!!" );
                EngineImpl::GetEngine()->Exit();
                break;
        }
    }
    void OnEnable() override {

    }
    void OnDisable() override {

    }
    void OnAttach() override  {

    }
    void Render( HGCore::Renderer *pRenderer ) override {
        renderAllGameObjects( pRenderer );
    }
    explicit SceneMainGame( const char* strName ) : Scene( strName ) {

    }
};


#endif //HONEYGAME_SCENEMAINGAME_H