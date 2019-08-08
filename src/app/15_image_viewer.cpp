#include <iostream>
#include <vector>
#include <glad/glad.h>
#include "../format/Image.hpp"
#include "../gl/Texture.hpp"
#include "../util/filehandler.hpp"
#include "../window/Window.hpp"

using namespace std;
using namespace format;
using namespace gl;

class ImageViewer : public Window
{
public:
    void setup(vector<Image> images);

private:
    void draw() override;

    vector<Texture> textures_;
};

void ImageViewer::setup(vector<Image> images)
{
    for (const Image& img : images)
        textures_.emplace_back(img.width, img.height, img.channels == 3 ? Texture::Rgb : Texture::Rgba, img.pixels.get());

    glClearColor(0.1, 0.1, 0.1, 1.0);
    glEnable(GL_TEXTURE_2D);
}

void ImageViewer::draw()
{
    glClear(GL_COLOR_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, width(), height(), 0, 0.0, 1.0);

    int x = 0;
    int y = 0;

    for (const Texture& texture : textures_)
    {
        const int w = texture.width();
        const int h = texture.height();
        
        texture.bind();

        glPushMatrix();
        glTranslated(x, y, 0);
        glBegin(GL_QUADS);
        glTexCoord2d(0, 0); glVertex3d(0, 0, 0); // bottom left
        glTexCoord2d(1, 0); glVertex3d(w, 0, 0); // bottom right
        glTexCoord2d(1, 1); glVertex3d(w, h, 0); // top right
        glTexCoord2d(0, 1); glVertex3d(0, h, 0); // top left
        glEnd();
        glPopMatrix();

        x += w;

        if (x >= width()) {
            x = 0;
            y += h;
        }
    }

    glMatrixMode(GL_MODELVIEW);
}

int main(int argc, const char* argv[])
{
    if (argc < 2) {
        cout << "Usage: " << argv[0] << " <image file>..." << endl;
        return 1;
    }

    try {
        vector<Image> images;

        for (int i = 1; i < argc; i++)
        {
            Image image(readFile(argv[i]));
            
            cout << "image: " << argv[i] << " (" << image.width << 'x' << image.height << 'x' << image.channels << ')' << endl;

            images.emplace_back(move(image));
        }
        
        ImageViewer win;

        if (!win.show(800, 600, "Image viewer")) {
            cout << "Could not create window" << endl;
            return 1;
        }

        win.setup(move(images));
        win.loop();
    }
    catch (const exception& e) {
        cout << "Exception: " << e.what() << endl;
        return 1;
    }    

    return 0;
}