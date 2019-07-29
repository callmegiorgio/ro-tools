#include <iostream>
#include <memory>
#include <vector>
#include <glad/glad.h>
#include "../resource/Spr.hpp"
#include "../util/filehandler.hpp"
#include "../util/Texture.hpp"
#include "../window/Window.hpp"

using namespace std;

class SprWindow : public Window
{
public:
    explicit SprWindow(Spr spr) : spr_{ std::move(spr) } {}

    void setup();

private:
    void paint() override;

    Spr spr_;
    vector<unique_ptr<Texture>> textures_;
};

void SprWindow::setup()
{
    for (const Spr::PaletteImage& img : spr_.palette_images)
    {
        if (spr_.pal)
        {
            vector<uint8_t> pixels;

            for (uint8_t index : img.indices)
            {
                const Color& color = spr_.pal->colors[index];

                pixels.push_back(color.r);
                pixels.push_back(color.g);
                pixels.push_back(color.b);
                pixels.push_back(color.a);
            }

            textures_.emplace_back(make_unique<Texture>(img.width, img.height, 4, pixels.data()));
        }
    }

    glClearColor(0.1, 0.1, 0.1, 1.0);
    glEnable(GL_TEXTURE_2D);
}

void SprWindow::paint()
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
        
        textures_[i]->bind();

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