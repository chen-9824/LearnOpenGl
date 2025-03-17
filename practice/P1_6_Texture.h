#ifndef P1_6_TEXTURE_H
#define P1_6_TEXTURE_H

#pragma once

#include "P1_5_Shader.h"

class P1_6_Texture : public P1_5_Shader
{
public:
    P1_6_Texture();
    ~P1_6_Texture();

    int draw_texture();

private:
    int load_img(const char *path, GLenum img_format);
};

#endif