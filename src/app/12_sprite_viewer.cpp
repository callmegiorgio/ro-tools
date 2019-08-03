#include <cstring>
#include <iostream>
#include <glad/glad.h>
#include "../format/Sprite.hpp"
#include "../gl/ApolloSprite.hpp"
#include "../gl/Texture.hpp"
#include "../util/filehandler.hpp"
#include "../window/Window.hpp"

using namespace std;
using namespace format;
using namespace gl;

class SpriteViewer : public Window {
public:
    explicit SpriteViewer(ApolloSprite sprite)
        : sprite_{ std::move(sprite) } {}

    void setup();

private:
    void onKeyEvent(KeyEvent evt) override;
    
    void update(double dt) override
    {
        if (animating_)
            sprite_.update(dt);
    }

    void draw() override;
    void drawCoordinateAxes();

    ApolloSprite sprite_;
    int center_x_, center_y_;
    bool animating_ = true;
};

void SpriteViewer::setup()
{
    sprite_.load();

    center_x_ = width() / 2;
    center_y_ = height() / 2;
    
    glClearColor(0.05, 0.05, 0.05, 1.0);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_ALPHA_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void SpriteViewer::onKeyEvent(KeyEvent evt)
{
    if (evt.action() != KeyEvent::Pressed)
        return;

    bool changed_animation = false;

    switch (evt.key())
    {
        case Key::Escape: 
            Window::close();
            break;

        case Key::Space:
            animating_ = !animating_;
            cout << "animating: " << std::boolalpha << animating_ << endl;
            break;

        case Key::Left:
            sprite_.recedeFrame();
            changed_animation = true;
            break;

        case Key::Right:
            sprite_.advanceFrame();
            changed_animation = true;
            break;

        case Key::Up:
            sprite_.advanceAnimation();
            changed_animation = true;
            break;

        case Key::Down:
            sprite_.recedeAnimation();
            changed_animation = true;
            break;
    }

    if (changed_animation)
        cout << "Animation " << sprite_.currentAnimationIndex() << ", frame " << sprite_.currentFrameIndex() << endl;
}

void SpriteViewer::draw()
{
    glClear(GL_COLOR_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, width(), height(), 0, 0.0, 1.0);

    drawCoordinateAxes();

    glPushMatrix();
    glTranslated(center_x_, center_y_, 0);
    sprite_.draw();
    glPopMatrix();

    glMatrixMode(GL_MODELVIEW);
}

void SpriteViewer::drawCoordinateAxes()
{
    glPushMatrix();
    glBegin(GL_LINES);
    glColor4f(1.f, 0.f, 0.f, 1.f); // red

    // x axis
    glVertex3f(0.f,     center_y_, 0.f);
    glVertex3f(width(), center_y_, 0.f);

    // y axis
    glVertex3f(center_x_, 0.f,      0.f);
    glVertex3f(center_x_, height(), 0.f);

    glEnd();
    glPopMatrix();
}

int main(int argc, const char* argv[])
{
    if (argc < 2) {
        cout << "Usage: " << argv[0] << " <sprite file>" << endl;
        return 1;
    }

    try {
        format::Sprite sprite(readFile(argv[1]));

        ApolloSprite ap_sprite(sprite, sprite.pal);
        SpriteViewer viewer(move(ap_sprite));

        if (!viewer.show(800, 600, "Sprite viewer")) {
            cout << "Could not create window" << endl;
            return 1;
        }

        viewer.setup();
        viewer.loop();
    }
    catch (const exception& e) {
        cout << "Exception: " << e.what() << endl;
        return 1;
    }    

    return 0;
}