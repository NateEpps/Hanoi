//
// main.cpp
// Nathanael Epps
//

#include <vector>
#include <iostream>
#include <algorithm>
#include <regex>
#include <future>
#include <SFML/Graphics.hpp>
#include "Util.hpp"
#include "Canvas.hpp"

int main2(std::vector<string> args)
{
    // put the options in a second vector
    std::regex isOption ("-+.*");
    std::vector<string> opts;
    auto point = std::remove_if(args.begin(), args.end(), [&](string s) {
        if (std::regex_match(s, isOption)) {
            opts.push_back(s);
            return true;
        }
        else {
            return false;
        }
    });
    args.erase(point, args.end());
    
    // for now, don't recognize any options
    if (opts.size() > 0) {
        for (string s : opts)
            std::cerr << s << ": Unrecognized option\n";
        return -1;
    }
    
    // if wrong number of args, print usage
    if (args.size() != 2) {
        std::cerr << "Usage: " << args.at(0) << " (num rings)\n";
        return -2;
    }
    
    // convert the argument
    size_t N = lexical_cast<size_t>(args.at(1));
    
    using namespace sf;
    
    // add graphics
    RenderWindow window;
#define DISPLAY
#ifdef DISPLAY
    VideoMode mode;
    Uint32 flags;
    
    GetVideoMode(WindowMode::TryFullscreen, mode, flags);
    //GetVideoMode(WindowMode::Desktop, mode, flags);
    
    window.create(mode, "Towers of Hanoi", flags);
#endif
    Canvas screen (N, window);
#ifdef DISPLAY
    Clock timer;
    Event e;
    
    while (window.isOpen())
    {
        while (window.pollEvent(e))
        {
            if (e.type == Event::Closed)
                window.close();
            else if (e.type == Event::KeyPressed)
                if (e.key.code == Keyboard::Escape)
                    window.close();
        }
        
        screen.update(timer.restart());
        
        window.clear(Color(20, 20, 20));
        window.draw(screen);
        window.display();
    }
    
    std::cout << "(main) Bounds: " << window.getSize() << "\n";
#endif
    return 0;
}

int main(int argc, const char** argv)
{
    std::vector<string> args (argv, argv + argc);
    
    try {
        return main2(args);
    }
    catch (std::exception& ex) {
        std::cerr << "\nInternal Exception: " << ex.what() << "\n";
        return -1;
    }
}
