#include "pch.h"

#include "Hello_Triangle.h"
#include "P1_5_Shader.h"
#include "P1_6_Texture.h"

int main(int, char **)
{
    std::cout << "Hello, from LearnOpenGl!\n";
#if 0
    spdlog::set_level(spdlog::level::debug);
    // draw_single_triangle();
    // draw_double_triangle();
    // draw_single_rectangle();
#elif 0
    P1_5_Shader practice_1_5;
    // practice_1_5.draw_triangle_uniform_color();
    practice_1_5.draw_triangle_VAO_color();
#elif 1
    P1_6_Texture _1_6_pra;
    _1_6_pra.draw_texture();
#endif
}
