//
// Created by cyf-m on 2020/12/10.
//

#include <SDL_image.h>
#include <string>
#include "Renderer2D.h"
#include "../Core/Log.h"

using namespace __HGImpl::V1;

SDL_Texture *Renderer2D::CreateTextureFromFile(const char *pStrFileName) {
    auto pImg = IMG_Load(pStrFileName );
    if( !pImg ) {
        HG_LOG_FAILED( std::string(pStrFileName).append(" -> create texture failed; details:").append( IMG_GetError() ).c_str());
        return nullptr;
    } else {
        HG_LOG_SUCCESS( std::string(pStrFileName).append(" -> create texture ").c_str() );
    }

    auto pTex = SDL_CreateTextureFromSurface( pHandle, pImg );
    if ( pTex == nullptr ) {
        HG_LOG_SDL_ERROR( SDL_LOG_CATEGORY_RENDER, "SDL_CreateTextureFromSurface in Renderer2D::CreateTextureFromFile" );
        return nullptr;
    } else {
        HG_LOG_SUCCESS( "SDL_CreateTextureFromSurface in Renderer2D::CreateTextureFromFile" );
    }

    SDL_FreeSurface( pImg );
    pImg = nullptr;

    umTextures[ pStrFileName ] = pTex;

    return pTex;
}

void Renderer2D::Copy(const GameObject2D *pGameObject) {
    SDL_RenderCopy( pHandle, pGameObject->GetTexture(), nullptr, nullptr );
}
