#include "pch.h"

#include "practice.h"

int main(int, char **)
{
    std::cout << "Hello, from LearnOpenGl!\n";

    spdlog::set_level(spdlog::level::debug);
    // draw_single_triangle();
    draw_double_triangle();
    // draw_single_rectangle();
}
