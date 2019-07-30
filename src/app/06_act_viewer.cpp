#include <cstring>
#include <iostream>
#include <glad/glad.h>
#include "../format/Act.hpp"
#include "../format/Spr.hpp"
#include "../gl/Sprite.hpp"
#include "../gl/Texture.hpp"
#include "../util/filehandler.hpp"
#include "../window/Window.hpp"

using namespace std;

class ActViewer : public Window {
public:
    explicit ActViewer(Sprite sprite)
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

    Sprite sprite_;
    int center_x_, center_y_;
    bool animating_ = true;
};

void ActViewer::setup()
{
    if (!sprite_.load()) {
        cout << "Could not load sprite object" << endl;
        Window::close();
        return;
    }

    center_x_ = width() / 2;
    center_y_ = height() / 2;
    
    glClearColor(0.05, 0.05, 0.05, 1.0);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_ALPHA_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void ActViewer::onKeyEvent(KeyEvent evt)
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

void ActViewer::draw()
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

void ActViewer::drawCoordinateAxes()
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
        cout << "Usage: " << argv[0] << " <act file>" << endl;
        return 1;
    }

    const char* filename = argv[1];
    const size_t filename_size = strlen(filename);

    if (filename_size < 3) {
        cout << "File name is too short" << endl;
        return 1;
    }

    try {
        Act act(readFile(filename));
        
        // Replace "act" extension with "spr"
        memcpy(const_cast<char*>(&filename[filename_size - 3]), "spr", 3);
        
        Spr spr(readFile(filename));
        Sprite sprite(act, spr);

        ActViewer viewer(move(sprite));

        if (!viewer.show(800, 600, "Act viewer")) {
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