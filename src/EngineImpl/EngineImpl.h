//
// Created by cyf-m on 2020/11/27.
//

#ifndef HONEYGAME_ENGINEIMPL_H
#define HONEYGAME_ENGINEIMPL_H

#include "../Engine/HG.h"
#include "../Core/Loop.h"
#include "../Core/Window.h"
#include "../Core/Thread.h"
#include "Renderer2D.h"

namespace __HGImpl { namespace V1 {
    class Scene;
    class HGMainLoop : public HGCore::Loop {
    public:
        void _RunTask() override;
        void _PaddingTask() override;
        void _StopTask() override;
    };
    class HGUpdateLoop : public HGCore::Loop {
    public:
        void _RunTask() override;
        void _PaddingTask() override;
        void _StopTask() override;
    };
    class HGRenderLoop : public HGCore::Loop {
    public:
        void _RunTask() override;
        void _PaddingTask() override;
        void _StopTask() override;
    };

    /// \brief the engine of HG
    /// \note the initialization order of Scenes, GameObjects, Engine Should be
    /// Scenes -> Engine -> GameObjects
    class EngineImpl : public HG::V1::Engine {
        private:
            HGMainLoop tLoopMain;
            HGUpdateLoop tLoopUpdate;
            HGRenderLoop tLoopRender;

            HGCore::Thread *pUpdateThread{};
            HGCore::Thread *pRenderThread{};

            HGCore::Window *pWindow;

            Renderer2D *pRenderer;

            Scene* pCurrentScene;
            static void SetEngine( EngineImpl* pEngine ) { EngineImpl::pEngine = pEngine; }

        public:
            Scene* GetCurrentScene() { return pCurrentScene; }
            void NavigateScene(const char * strSceneName );
            static EngineImpl* GetEngine() { return pEngine; }
            static EngineImpl* pEngine;

            HGCore::Window * GetWindow() const { return pWindow; }
            Renderer2D * GetRenderer2D() const { return pRenderer; }

            void Exit();

            int Run() override;

            EngineImpl(int argc, char **argv );
            virtual ~EngineImpl();
    };

} }

#endif //HONEYGAME_ENGINEIMPL_H
