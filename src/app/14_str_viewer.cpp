#include <cstring>
#include <iostream>
#include <glad/glad.h>
#include "../format/Str.hpp"
#include "../gl/Effect.hpp"
#include "../gl/Texture.hpp"
#include "../util/filehandler.hpp"
#include "../window/Window.hpp"

using namespace std;
using namespace format;
using namespace gl;

class StrViewer : public Window {
public:
    void setup(const Str& str, const char* texture_path);

private:
    void onKeyEvent(KeyEvent evt) override;
    
    void update(double dt) override
    {
        if (animating_)
            effect_.update(dt);
    }

    void draw() override;
    void drawCoordinateAxes();

    Effect effect_;
    int center_x_, center_y_;
    bool animating_ = false;
};

void StrViewer::setup(const Str& str, const char* texture_path)
{
    effect_.load(str, texture_path);

    center_x_ = width() / 2;
    center_y_ = height() / 2;
    
    glClearColor(0.05, 0.05, 0.05, 1.0);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_ALPHA_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void StrViewer::onKeyEvent(KeyEvent evt)
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
            effect_.recedeFrame();
            changed_animation = true;
            break;

        case Key::Right:
            effect_.advanceFrame();
            changed_animation = true;
            break;

        case Key::B:
            effect_.showBorder(!effect_.showingBorder());
            cout << "showing border: " << std::boolalpha << effect_.showingBorder() << endl;
            break;
    }

    if (changed_animation)
    {
        cout << "frame: " << effect_.currentFrame() << '/' << effect_.frameCount() << ", active layers: ";
        
        for (int layer : effect_.activeLayers())
            cout << layer << ", ";

        cout << endl;
    }
}

void StrViewer::draw()
{
    glClear(GL_COLOR_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, width(), height(), 0, 0.0, 1.0);

    drawCoordinateAxes();
    effect_.draw();

    glMatrixMode(GL_MODELVIEW);
}

void StrViewer::drawCoordinateAxes()
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

int main(int argc, const char* argv[])
{
    if (argc < 3) {
        cout << "Usage: " << argv[0] << " <str file> <textures path>" << endl;
        return 1;
    }

    try {
        Str str(readFile(argv[1]));
        StrViewer viewer;

        if (!viewer.show(1024, 768, "Str viewer")) {
            cout << "Could not create window" << endl;
            return 1;
        }

        viewer.setup(str, argv[2]);
        viewer.loop();
    }
    catch (const exception& e) {
        cout << "Exception: " << e.what() << endl;
        return 1;
    }    

    return 0;
}