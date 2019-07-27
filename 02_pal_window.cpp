#include <iostream>
#include <string>
#include <glad/glad.h>
#include "filereader.hpp"
#include "Pal.hpp"
#include "window/Window.hpp"

using namespace std;

class PalWindow : public Window {
public:
    explicit PalWindow(Pal pal) : pal_{ std::move(pal) } {}

private:
    void paint() override;

    Pal pal_;
};

void PalWindow::paint()
{
    const int rect_width = width() / 16;
    const int rect_height = height() / 16;
    
    glClear(GL_COLOR_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, width(), height(), 0, 0.0, 1.0);

	for (int i = 0; i < 16; i++)
    {
        for (int j = 0; j < 16; j++)
        {
            const Color& c = pal_[i*j];

            glPushMatrix();
            glTranslatef(i * rect_width, j * rect_height, 0.0);
            glBegin(GL_QUADS);
            glColor4f(c.r / 255, c.g / 255, c.b / 255, 1.0);
            glVertex2i(0, 0);
            glVertex2i(0, rect_height);
            glVertex2i(rect_width, rect_height);
            glVertex2i(rect_width, 0);
            glEnd();
            glPopMatrix();
        }
    }

    glMatrixMode(GL_MODELVIEW);
}

int main(int argc, const char* argv[])
{
    if (argc < 2) {
        cout << "Usage: " << argv[0] << " <pal file>" << endl;
        return 1;
    }

    try {
        Pal pal(readFile(argv[1]));
        PalWindow win(move(pal));
        
        if (!win.show(16 * 20, 16 * 20, argv[1])) {
            cout << "Could not open window" << endl;
            return 1;
        }

        win.loop();
    }
    catch (const exception& e) {
        cout << "Exception: " << e.what() << endl;
        return 1;
    }

    return 0;
}