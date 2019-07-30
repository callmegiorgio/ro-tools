#include <array>
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

constexpr static array<Texture::ResizeFilter, 6> filters {
    Texture::Nearest,
    Texture::Linear,
    Texture::NearestMipmapNearest,
    Texture::NearestMipmapLinear,
    Texture::LinearMipmapLinear,
    Texture::LinearMipmapNearest
};

static const char* resizeFilterName(Texture::ResizeFilter filter)
{
    switch (filter)
    {
        case Texture::Nearest: return "nearest";
        case Texture::Linear: return "linear";
        case Texture::NearestMipmapNearest: return "nearest mipmap nearest";
        case Texture::NearestMipmapLinear: return "nearest mipmap linear";
        case Texture::LinearMipmapLinear: return "linear mipmap linear";
        case Texture::LinearMipmapNearest: return "linear mipmap nearest";
    }

    return "";
}

class BodyHeadViewer : public Window {
public:
    explicit BodyHeadViewer(Sprite body_sprite, Sprite head_sprite)
        : body_sprite_{ std::move(body_sprite) }
        , head_sprite_{ std::move(head_sprite) } {}

    void setup();

private:
    void showCommands();

    void onKeyEvent(KeyEvent evt) override;
    void onMouseWheelEvent(MouseWheelEvent evt) override
    {
        scale_per_ += evt.y() * 30;
        cout << "scale: " << scale_per_ << '%' << endl;
    }
    
    void update(double dt) override
    {
        if (animating_) {
            body_sprite_.update(dt);
            head_sprite_.update(dt);
        }
    }

    void draw() override;
    void drawCoordinateAxes();
    void drawSprites();

    Sprite body_sprite_, head_sprite_;
    int center_x_, center_y_;
    int scale_per_ = 100;
    bool animating_ = false;
    int min_filter_idx_ = 0;
};

void BodyHeadViewer::setup()
{
    if (!body_sprite_.load() || !head_sprite_.load()) {
        cout << "Could not load sprite objects" << endl;
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

    showCommands();
}

void BodyHeadViewer::showCommands()
{
    cout << "Escape   exit" << endl;
    cout << "Space    toggle animation on/off" << endl;
    cout << "Left     recede frame" << endl;
    cout << "Right    advance frame" << endl;
    cout << "Up       advance animation" << endl;
    cout << "Down     recede animation" << endl;
    cout << "M        change sprite magnification filter" << endl;
    cout << "N        change sprite minifying filter" << endl;
}

void BodyHeadViewer::onKeyEvent(KeyEvent evt)
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
            body_sprite_.recedeFrame();
            head_sprite_.recedeFrame();
            changed_animation = true;
            break;

        case Key::Right:
            body_sprite_.advanceFrame();
            head_sprite_.advanceFrame();
            changed_animation = true;
            break;

        case Key::Up:
            body_sprite_.advanceAnimation();
            head_sprite_.advanceAnimation();
            changed_animation = true;
            break;

        case Key::Down:
            body_sprite_.recedeAnimation();
            head_sprite_.recedeAnimation();
            changed_animation = true;
            break;

        case Key::M:
        {
            Texture::ResizeFilter mag_filter = body_sprite_.magFilter() == Texture::Linear ? Texture::Nearest : Texture::Linear;
            body_sprite_.setMagFilter(mag_filter);
            head_sprite_.setMagFilter(mag_filter);
            cout << "using magnification filter: " << resizeFilterName(mag_filter) << endl;
        } break;

        case Key::N:
        {
            if (++min_filter_idx_ >= filters.size())
                min_filter_idx_ = 0;

            Texture::ResizeFilter min_filter = filters[min_filter_idx_];
            body_sprite_.setMagFilter(min_filter);
            head_sprite_.setMagFilter(min_filter);
            cout << "using minifying filter: " << resizeFilterName(min_filter) << endl;
        } break;
    }

    if (changed_animation)
        cout << "Animation " << body_sprite_.currentAnimationIndex() << ", frame " << body_sprite_.currentFrameIndex() << endl;
}

void BodyHeadViewer::draw()
{
    glClear(GL_COLOR_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, width(), height(), 0, 0.0, 1.0);

    drawCoordinateAxes();
    drawSprites();

    glMatrixMode(GL_MODELVIEW);
}

void BodyHeadViewer::drawCoordinateAxes()
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

void BodyHeadViewer::drawSprites()
{
    glPushMatrix();
    glTranslated(center_x_, center_y_, 0);
    glScalef(scale_per_ / 100.f, scale_per_ / 100.f, scale_per_ / 100.f);
    
    body_sprite_.draw();
    head_sprite_.draw(body_sprite_);

    glPopMatrix();
}

int main(int argc, const char* argv[])
{
    if (argc < 3) {
        cout << "Usage: " << argv[0] << " <body act file> <head act file>" << endl;
        return 1;
    }

    const char* body_fn = argv[1];
    const char* head_fn = argv[2];
    const size_t body_fn_size = strlen(body_fn);
    const size_t head_fn_size = strlen(head_fn);

    if (body_fn_size < 3 || head_fn_size < 3) {
        cout << "File name is too short" << endl;
        return 1;
    }

    try {
        Act body_act(readFile(body_fn));
        Act head_act(readFile(head_fn));
        
        // Replace "act" extension with "spr"
        memcpy(const_cast<char*>(&body_fn[body_fn_size - 3]), "spr", 3);
        memcpy(const_cast<char*>(&head_fn[head_fn_size - 3]), "spr", 3);
        
        Spr body_spr(readFile(body_fn));
        Spr head_spr(readFile(head_fn));

        Sprite body_sprite(body_act, body_spr);
        Sprite head_sprite(head_act, head_spr);

        BodyHeadViewer viewer(move(body_sprite), move(head_sprite));

        if (!viewer.show(800, 600, "Body-head viewer")) {
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