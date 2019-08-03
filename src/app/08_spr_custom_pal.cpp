#include <cmath>
#include <iostream>
#include <vector>
#include <glad/glad.h>
#include "../format/Spr.hpp"
#include "../format/Pal.hpp"
#include "../gl/Texture.hpp"
#include "../util/filehandler.hpp"
#include "../window/Window.hpp"

using namespace std;
using namespace format;
using namespace gl;

class SprCustomPalWindow : public Window
{
public:
    explicit SprCustomPalWindow(Spr spr, vector<Pal> pals)
        : spr_{ std::move(spr) }
        , pals_{ std::move(pals) } {}

    void setup();

private:
    void setTextures(const Pal& pal);
    void showCommands();

    void onKeyEvent(KeyEvent evt) override;

    void draw() override;
    void drawCoordinateAxes();
    void drawSprite();

    Spr spr_;
    vector<Pal> pals_;
    vector<Texture> textures_;
    int current_pal_idx_ = 0;
    int current_tex_idx_ = 0;
    bool use_custom_pal_ = true;
    
    int center_x_;
    int center_y_;
};

void SprCustomPalWindow::setup()
{
    // Use custom palette by default
    setTextures(pals_.front());
    showCommands();

    center_x_ = width() / 2;
    center_y_ = height() / 2;

    glClearColor(0.1, 0.1, 0.1, 1.0);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_ALPHA_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void SprCustomPalWindow::setTextures(const Pal& pal)
{
    // Create textures if they weren't previously created
    if (textures_.empty())
        textures_.resize(spr_.palette_images.size());

    auto texture_iter = textures_.begin();

    for (const Spr::PaletteImage& img : spr_.palette_images)
    {
        vector<uint8_t> pixels(img.indices.size() * 4);
        const Color& transp_color = pal.colors[0];

        for (int i = 0; i < img.indices.size(); i++)
        {
            uint8_t index = img.indices[i];
            const Color& color = pal.colors[index];

            pixels[i*4 + 0] = color.r;
            pixels[i*4 + 1] = color.g;
            pixels[i*4 + 2] = color.b;
            pixels[i*4 + 3] = 
                color.r == transp_color.r &&
                color.g == transp_color.g &&
                color.b == transp_color.b ?
                0 : 255;
        }

        texture_iter->load(img.width, img.height, Texture::Rgba, pixels.data());
        texture_iter++;
    }
}

void SprCustomPalWindow::showCommands()
{
    cout << "Escape   exit" << endl;
    cout << "Left     show previous image from spr file" << endl;
    cout << "Right    show next image from spr file" << endl;
    cout << "Up       use previous custom palette from the list" << endl;
    cout << "Down     use next custom palette from the list" << endl;
    cout << "C        toggle custom/sprite palette" << endl;
}

void SprCustomPalWindow::onKeyEvent(KeyEvent evt)
{
    if (evt.action() == KeyEvent::Released)
        return;

    switch (evt.key())
    {
        case Key::Escape:
            Window::close();
            break;

        case Key::Left:
            if (--current_tex_idx_ < 0)
                current_tex_idx_ = textures_.size() - 1;
            
            cout << "image: " << (current_tex_idx_ + 1) << '/' << textures_.size() << endl;
            break;
        
        case Key::Right:
            if (++current_tex_idx_ >= textures_.size())
                current_tex_idx_ = 0;

            cout << "image: " << (current_tex_idx_ + 1) << '/' << textures_.size() << endl;
            break;

        case Key::Up:
            if (use_custom_pal_ && pals_.size() >= 2 && current_pal_idx_ < (pals_.size() - 1))
                setTextures(pals_[++current_pal_idx_]);

            cout << "custom palette: " << (current_pal_idx_ + 1) << '/' << pals_.size() << endl;
            break;

        case Key::Down:
            if (use_custom_pal_ && pals_.size() >= 2 && current_pal_idx_ > 0)
                setTextures(pals_[--current_pal_idx_]);

            cout << "custom palette: " << (current_pal_idx_ + 1) << '/' << pals_.size() << endl;
            break;

        case Key::C:
            if (!spr_.pal) {
                cout << "Spr has no palette" << endl;
                return;
            }

            use_custom_pal_ = !use_custom_pal_;

            if (use_custom_pal_)
                setTextures(pals_[current_pal_idx_]);
            else
                setTextures(*spr_.pal);

            cout << "palette type: " << (use_custom_pal_ ? "custom" : "sprite") << endl;
            break;
    }
}

void SprCustomPalWindow::draw()
{
    glClear(GL_COLOR_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, width(), height(), 0, 0.0, 1.0);

    drawCoordinateAxes();
    drawSprite();

    glMatrixMode(GL_MODELVIEW);
}

void SprCustomPalWindow::drawCoordinateAxes()
{
    glPushMatrix();
    glBegin(GL_LINES);
    glColor4d(1, 0, 0, 1); // red

    // x axis
    glVertex3d(0,       center_y_, 0);
    glVertex3d(width(), center_y_, 0);

    // y axis
    glVertex3d(center_x_, 0,        0);
    glVertex3d(center_x_, height(), 0);

    glEnd();
    glPopMatrix();
}

void SprCustomPalWindow::drawSprite()
{
    glPushMatrix();
    glTranslated(center_x_, center_y_, 0);
    glColor4d(1, 1, 1, 1);

    const Spr::PaletteImage& img = spr_.palette_images[current_tex_idx_];
    const int x = -static_cast<int>(round(img.width / 2.0));
    const int y = -static_cast<int>(round(img.height / 2.0));
    const int w = img.width;
    const int h = img.height;
    
    textures_[current_tex_idx_].bind();
    
    glBegin(GL_QUADS);
    glTexCoord2d(0, 0); glVertex3d(x,   y,   0); // bottom left
    glTexCoord2d(1, 0); glVertex3d(x+w, y,   0); // bottom right
    glTexCoord2d(1, 1); glVertex3d(x+w, y+h, 0); // top right
    glTexCoord2d(0, 1); glVertex3d(x,   y+h, 0); // top left
    glEnd();
    
    Texture::unbind();

    glPopMatrix();
}

int main(int argc, const char* argv[])
{
    if (argc < 3) {
        cout << "Usage: " << argv[0] << " <spr file> <pal file>..." << endl;
        cout << "Draws a spr file with a custom palette." << endl;

        return 1;
    }

    try {
        Spr spr(readFile(argv[1]));
        
        vector<Pal> pals;

        for (int i = 2; i < argc; i++)
            pals.emplace_back(Pal(readFile(argv[i])));

        SprCustomPalWindow win(move(spr), move(pals));

        if (!win.show(800, 600, "Spr custom pal")) {
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