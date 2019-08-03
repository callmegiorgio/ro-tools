#include <iostream>
#include <vector>
#include <glad/glad.h>
#include "../format/Spr.hpp"
#include "../gl/Texture.hpp"
#include "../util/filehandler.hpp"
#include "../window/Window.hpp"

using namespace std;
using namespace format;
using namespace gl;

class SprWindow : public Window
{
public:
    explicit SprWindow(Spr spr) : spr_{ std::move(spr) } {}

    void setup();

private:
    void draw() override;

    Spr spr_;
    vector<Texture> textures_;
};

void SprWindow::setup()
{
    for (const Spr::PaletteImage& img : spr_.palette_images)
    {
        if (spr_.pal)
        {
            vector<uint8_t> pixels(img.indices.size() * 4);

            for (int i = 0; i < img.indices.size(); i++)
            {
                uint8_t index = img.indices[i];
                const Color& color = spr_.pal->colors[index];

                pixels[i*4 + 0] = color.r;
                pixels[i*4 + 1] = color.g;
                pixels[i*4 + 2] = color.b;
                pixels[i*4 + 3] = color.a;
            }

            textures_.emplace_back(Texture(img.width, img.height, Texture::Rgba, pixels.data()));
        }
    }

    glClearColor(0.1, 0.1, 0.1, 1.0);
    glEnable(GL_TEXTURE_2D);
}

void SprWindow::draw()
{
    glClear(GL_COLOR_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, width(), height(), 0, 0.0, 1.0);

    float x = 0;
    float y = 0;

    for (int i = 0; i < textures_.size(); i++)
    {
        const Spr::PaletteImage& img = spr_.palette_images[i];
        const float w = img.width;
        const float h = img.height;
        
        textures_[i].bind();

        glPushMatrix();
        glTranslatef(x, y, 0.0);
        glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(0, 0, 0.f); // bottom left
        glTexCoord2f(1.0f, 0.0f); glVertex3f(w, 0, 0.f); // bottom right
        glTexCoord2f(1.0f, 1.0f); glVertex3f(w, h, 0.f); // top right
        glTexCoord2f(0.0f, 1.0f); glVertex3f(0, h, 0.f); // top left
        glEnd();
        glPopMatrix();

        x += img.width;

        if (x >= width()) {
            x = 0;
            y += img.height;
        }
    }

    glMatrixMode(GL_MODELVIEW);
}

int main(int argc, const char* argv[])
{
    if (argc < 2) {
        cout << "Usage: " << argv[0] << " <spr file>" << endl;
        return 1;
    }

    try {
        Spr spr(readFile(argv[1]));
        SprWindow win(move(spr));

        if (!win.show(800, 600, argv[1])) {
            cout << "Could not create window" << endl;
            return 1;
        }

        win.setup();
        win.loop();
    }
    catch (const exception& e) {
        cout << "Exception: " << e.what() << endl;
        return 1;
    }    

    return 0;
}