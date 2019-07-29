#include <iostream>
#include "../resource/Pal.hpp"
#include "../util/filehandler.hpp"

using namespace std;

int main(int argc, const char* argv[])
{
    if (argc < 2) {
        cout << "Usage: " << argv[0] << " <pal file>" << endl;
        return 1;
    }

    try {
        Pal pal(readFile(argv[1]));
        
        cout << "Pal file '" << argv[1] << "' loaded:" << endl;
        cout << "  colors: " << pal.colors.size() << endl;
        
        for (const Color& color : pal.colors)
            cout << "   - " << color << endl;
    }
    catch (const exception& e) {
        cout << "Exception: " << e.what() << endl;
        return 1;
    }

    return 0;
}