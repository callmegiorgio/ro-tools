#include <iostream>
#include "filereader.hpp"
#include "Pal.hpp"

using namespace std;

int main(int argc, const char* argv[])
{
    if (argc < 2) {
        cout << "Usage: " << argv[0] << " <pal file>" << endl;
        return 1;
    }

    try {
        Pal pal(readFile(argv[1]));
        
        cout << "Pal file " << argv[1] << " loaded:" << endl;
        cout << "  colors:" << endl;
        
        for (auto& color : pal.colors())
            cout << "   - rgb(" << color.r << ',' << color.g << ',' << color.b << ')' << endl;
    }
    catch (const exception& e) {
        cout << "Exception: " << e.what() << endl;
        return 1;
    }

    return 0;
}