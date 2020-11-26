//
// Created by cyf-m on 2020/11/25.
//

#include "Window.h"
#include "Error.h"
#include "Math.h"
#include "Log.h"

using namespace HG;
using namespace HG::Math;

Window::Window(
        const char *title,
        int x, int y, int w,
        int h, Uint32 flags ) {
    this->pWin = SDL_CreateWindow( title, x, y, w, h, flags );
    if (this->pWin == nullptr){
        Log->Failed(SDL_LOG_CATEGORY_SYSTEM, "SDL_CreateWindow");
    } else {
        Log->Info( SDL_LOG_CATEGORY_SYSTEM, "SDL_CreateWindow" );
    }
}

Window::~Window() {
    SDL_DestroyWindow( this->pWin );
}

HGResult Window::GetSize( HGRect* pSize ) {
    int w = 0, h = 0;
    SDL_GetWindowSize( this->pWin, &w, &h );
    if ( w == 0 && h == 0 ) {
        return HG_ERR_NULL_OUTPUT;
    }
    pSize->H = h;
    pSize->W = w;
    return HG_ERR_OK;
}

HGResult Window::SetCenterScreen() {
    HGPos tPos;
    HGRect tSize;
    HG_RESULT_RES

    CHECK_ERR( GetSize( &tSize ) )
    CHECK_ERR( tScreen.GetCenterPos( tSize.W, tSize.H, &tPos ) )
    SDL_SetWindowPosition( pWin, tPos.X, tPos.Y );
    return HG_ERR_OK;
}


