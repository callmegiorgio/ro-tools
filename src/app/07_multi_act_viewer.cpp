#include <array>
#include <cstring>
#include <iostream>
#include <memory>
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

class MultiActViewer : public Window {
public:
    explicit MultiActViewer(vector<Sprite> sprites)
        : sprites_{ std::move(sprites) } {}

    void setup();

private:
    void showCommands();

    void onKeyEvent(KeyEvent evt) override;
    void onMouseWheelEvent(MouseWheelEvent evt) override
    {
        scale_per_ += evt.y() * 30;
        cout << "scale: " << scale_per_ << '%' << endl;
    }
    
    void update(double dt) override;
    void draw() override;
    void drawCoordinateAxes();
    void drawSprites();

    // Sprites in their anchor dependency order i.e. sprites_[0] has no dependency
    vector<Sprite> sprites_;

    int center_x_, center_y_;
    int scale_per_ = 100;
    bool animating_ = false;
    int min_filter_idx_ = 0;
};

void MultiActViewer::setup()
{
    for (auto& sprite : sprites_)
    {
        if (!sprite.load()) {
            cout << "Could not load sprite" << endl;
            Window::close();
            return;
        }
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

void MultiActViewer::showCommands()
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

void MultiActViewer::onKeyEvent(KeyEvent evt)
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
            for (auto& sprite : sprites_) sprite.recedeFrame();
            changed_animation = true;
            break;

        case Key::Right:
            for (auto& sprite : sprites_) sprite.advanceFrame();
            changed_animation = true;
            break;

        case Key::Up:
            for (auto& sprite : sprites_) sprite.advanceAnimation();
            changed_animation = true;
            break;

        case Key::Down:
            for (auto& sprite : sprites_) sprite.recedeAnimation();
            changed_animation = true;
            break;

        case Key::M:
        {
            Texture::ResizeFilter mag_filter = sprites_[0].magFilter() == Texture::Linear ? Texture::Nearest : Texture::Linear;

            for (auto& sprite : sprites_)
                sprite.setMagFilter(mag_filter);

            cout << "using magnification filter: " << resizeFilterName(mag_filter) << endl;
        } break;

        case Key::N:
        {
            if (++min_filter_idx_ >= filters.size())
                min_filter_idx_ = 0;

            Texture::ResizeFilter min_filter = filters[min_filter_idx_];
            
            for (auto& sprite : sprites_)
                sprite.setMagFilter(min_filter);
                
            cout << "using minifying filter: " << resizeFilterName(min_filter) << endl;
        } break;
    }

    if (changed_animation)
        cout << "Animation " << sprites_[0].currentAnimationIndex() << ", frame " << sprites_[0].currentFrameIndex() << endl;
}

void MultiActViewer::update(double dt)
{
    if (!animating_)
        return;
        
    for (auto& sprite : sprites_)
        sprite.update(dt);
}

void MultiActViewer::draw()
{
    glClear(GL_COLOR_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, width(), height(), 0, 0.0, 1.0);

    drawCoordinateAxes();
    drawSprites();

    glMatrixMode(GL_MODELVIEW);
}

void MultiActViewer::drawCoordinateAxes()
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

void MultiActViewer::drawSprites()
{
    glPushMatrix();
    glTranslated(center_x_, center_y_, 0);
    glScalef(scale_per_ / 100.f, scale_per_ / 100.f, scale_per_ / 100.f);
    
    auto sprite_iter = sprites_.cbegin();
    const Sprite& body_sprite = *sprite_iter;

    // The body sprite has no anchor
    body_sprite.draw();

    // Draw every other sprite using the body one as its anchor
    while (++sprite_iter != sprites_.cend())
        sprite_iter->draw(body_sprite);

    glPopMatrix();
}

int main(int argc, const char* argv[])
{
    if (argc < 3)
    {
        cout << "Usage: " << argv[0] << " <body act file> <anchored act file>..." << endl;
        cout << endl;
        cout << "Draws a number of act objects anchored by a body act. The order of drawing is that of the arguments." << endl;
        cout << "So, for example, running '" << argv[0] << " body_act act1 act2' will make 'act1' and 'act2' use 'body_act' as its anchor." << endl;

        return 1;
    }

    for (int i = 1; i < argc; i++) 
    {
        // Ensure every act file's name is at least 3-length long so that we can replace its "act" extension with "spr".
        if (strlen(argv[i]) < 3)
        {
            cout << "File '" << argv[i] << "' extension is too short" << endl;
            return 1;
        }
    }

    try {
        vector<Sprite> sprites;

        // Needed since Sprite class neither own Spr nor Act objects
        vector<unique_ptr<Act>> acts;
        vector<unique_ptr<Spr>> sprs;

        // Tries opening every .act and .spr for each given .act
        for (int i = 1; i < argc; i++) 
        {
            char* filename = const_cast<char*>(argv[i]);

            acts.emplace_back(make_unique<Act>(readFile(filename)));
            memcpy(&filename[strlen(filename) - 3], "spr", 3);
            sprs.emplace_back(make_unique<Spr>(readFile(filename)));

            sprites.emplace_back(Sprite(*acts.back(), *sprs.back()));
        }

        MultiActViewer viewer(move(sprites));

        if (!viewer.show(800, 600, "Multi act viewer")) {
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