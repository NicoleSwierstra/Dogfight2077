#pragma once

#include <map>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "glm/glm.hpp"

#include "graphics/Shader.h"


/// Holds all state information relevant to a character as loaded using FreeType
struct Character {
    unsigned int TextureID; // ID handle of the glyph texture
    glm::ivec2   Size;      // size of glyph
    glm::ivec2   Bearing;   // offset from baseline to left/top of glyph
    unsigned int Advance;   // horizontal offset to advance to next glyph
};


// A renderer class for rendering text displayed by a font loaded using the 
// FreeType library. A single font is loaded, processed into a list of Character
// items for later rendering.
class TextRenderer
{
    //static std::map<std::string, TextRenderer*> textRenderers;
public:
    float aspect;
    std::map<char, Character> Characters;
    Shader * TextShader;

    TextRenderer(unsigned int width, unsigned int height);
    ~TextRenderer();

    void Load(std::string font, unsigned int fontSize);
    
    void RenderText(std::string text, float x, float y, float scale, glm::vec3 color = glm::vec3(1.0f));
private:
    unsigned int VAO, VBO;
};