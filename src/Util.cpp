//
// Util.cpp
// Nathanael Epps
//

#include "Util.hpp"

#ifdef DEBUG
# include <iostream>
#endif

using namespace sf;

void GetVideoMode(WindowMode wm, sf::VideoMode& vmref, sf::Uint32& maskref)
{
    if (wm == WindowMode::Desktop) {
        vmref = VideoMode::getDesktopMode();
        maskref = Style::Default;
    }
    
    else {
        const auto& modes = VideoMode::getFullscreenModes();
        auto desk = VideoMode::getDesktopMode();
        
        auto itr = std::find_if(modes.begin(), modes.end(), [=](VideoMode vm){
            return ((vm.width <= desk.width) &&
                    (vm.height <= desk.height) &&
                    (vm.bitsPerPixel <= vm.bitsPerPixel));
        });
        
        if (wm == WindowMode::Fullscreen) // force fullscreen
        {
            if (itr == modes.end())
                throw MakeException("Fullscreen not supported!");
            
            vmref = *itr;
            maskref = Style::Fullscreen;
        }
        else // TryFullscreen
        {
            if (itr == modes.end()) // fall back on desktop mode
            {
#ifdef DEBUG
                std::cerr << "Can't create fullscreen, falling back on desktop...\n";
#endif
                vmref = desk;
                maskref = Style::Default;
            }
            
            else // go fullscreen
            {
                vmref = *itr;
                maskref = Style::Fullscreen;
            }
        }
    }
}

ReallyLong AmtHanoiMoves(ReallyLong x)
{
    if (x <= 1)
        return x;
    else
        return 2 * AmtHanoiMoves(x - 1) + 1;
}

static size_t GetAlt(size_t x, size_t y)
{
    if (x == 1)
    {
        if (y == 2)
            return 3;
        else if (y == 3)
            return 2;
    }
    else if (x == 2)
    {
        if (y == 1)
            return 3;
        else if (y == 3)
            return 1;
    }
    else if (x == 3)
    {
        if (y == 1)
            return 2;
        else if (y == 2)
            return 1;
    }
    
    throw MakeException("Invalid input!");
}

/*
 attempt 3: uses a dynamic and recursive approach, storing stuff
 Although not nearly as easy to read, it works slightly faster
 */

#include <unordered_map>

namespace {
    struct move_t
    {
        size_t amt;
        size_t from;
        size_t dest;
    };
    
    bool operator==(move_t left, move_t right)
    {
        return (left.amt == right.amt) &&
            (left.from == right.from) &&
            (left.dest == right.dest);
    }
    
    /* bool operator<(move_t left, move_t right)
    {
        if (left.amt == right.amt)
            if (left.from == right.from)
                return left.dest < right.dest;
            else
                return left.from < right.from;
        else
            return left.amt < right.amt;
    } */
}

namespace std
{
    template <>
    struct hash<move_t>
    {
        size_t operator()(const move_t& m) const noexcept
        {
            return m.amt * 100 + m.from * 10 + m.dest;
        }
    };
}

/*
 * The algorithm itself. Returns a string encoding the moves. For example,
 *  GetHanoiMoves(3) returns, "13123213212313". This means "move the ring
 *  from pile 1 to pile 3, move the ring from pile 1 to pile 2, move the ring
 *  from pile 3 to pile 2," etc.
 */

// Attempt 3: Hash tables!

string GetHanoiMoves(size_t amt, size_t from, size_t dest)
{
    static std::unordered_map<move_t, string> AllMoves;
    
    if (amt == 0)
        throw MakeException("\'amt\' can't be zero");
    
    // base case
    if (amt == 1)
        return lexical_cast<string>(from) + lexical_cast<string>(dest);
    
    size_t alt = GetAlt(from, dest);
    move_t req = {amt, from, dest};
    auto itr = AllMoves.find(req);
    
    // if (element does not exist)
    if (itr == AllMoves.end())
    {
        // calculate it...
        AllMoves[req] = GetHanoiMoves(amt - 1, from, alt) +
                        GetHanoiMoves(1, from, dest) +
                        GetHanoiMoves(amt - 1, alt, dest);
        
        // ... and call function again
        return GetHanoiMoves(amt, from, dest);
    }
    
    // if (it does exist), return it
    return itr->second;
}

/*
 attempt 2: uses recursion, but with multithreading when the amount of
    input is high in an attempt to gain speed. unfortunately doesn't
    seem to scale well either.
 
#include <future>

static constexpr size_t Threshold = 5;

string GetHanoiMoves(size_t amt, size_t from, size_t dest)
{
    if (amt == 0)
        throw MakeException("Invalid input!");
    
    size_t alt = GetAlt(from, dest);
    
    if (amt == 1) {
        return lexical_cast<string>(from) + lexical_cast<string>(dest);
    }
    else if (amt < Threshold) {
        return GetHanoiMoves(amt - 1, from, alt) +
            GetHanoiMoves(1, from, dest) +
            GetHanoiMoves(amt - 1, alt, dest);
    }
    else {
        auto left = std::async(std::launch::async, GetHanoiMoves, amt - 1, from, alt);
        auto right = std::async(std::launch::async, GetHanoiMoves, amt - 1, alt, dest);
        
        return left.get() + GetHanoiMoves(1, from, dest) + right.get();
    }
}
*/
/*
Attempt 1: uses pure recursion. correct, but doesn't scale well
 
string GetHanoiMoves(size_t amt, size_t from, size_t dest)
{
    // check amt
    if (amt == 0)
        throw MakeException("Invalid input!");
    
    // get the alternate pole
    size_t alt = GetAlt(from, dest);
    
    if (amt == 1)
        return lexical_cast<string>(from) + lexical_cast<string>(dest);
    else
        return GetHanoiMoves(amt - 1, from, alt) +
                lexical_cast<string>(from) +
                lexical_cast<string>(dest) +
                GetHanoiMoves(amt - 1, alt, dest);
}
 */

#include <future>

string GetHanoiMovesAsync(size_t amt, std::ostream& stream)
{
    auto handle = std::async(std::launch::async, GetHanoiMoves, amt, 1, 3);
    
    stream << "(GetHanoiMovesAsync) Calculating moves..." << std::flush;
    
    const auto WaitTime = std::chrono::milliseconds(750);
    
    while (handle.wait_for(WaitTime) == std::future_status::timeout)
        stream << "." << std::flush;
    
    stream << "\n";
    
    return handle.get();
}

bool IsClose(sf::Vector2f left, sf::Vector2f right, float epsilon)
{
    return IsClose(left.x, right.x, epsilon) &&
            IsClose(left.y, right.y, epsilon);
}

bool IsClose(float left, float right, float epsilon)
{
    return std::abs(left - right) < epsilon;
}

sf::Color GetRandColor()
{
    static const Color colors[] = {
        Color::Red,
        Color(255, 165, 0), // orange
        Color(220, 220, 0), // yellow?
        Color(0, 128, 50), // lowkey green
        Color::Blue,
        Color(75, 0, 130), // indigo
        Color(128, 0, 128) // violet/purple
    };
    
    // start high so we can wrap around
    // and start at zero
    static size_t index = 0xffff;
    static const size_t limit = 7;
    
    index++;
    if (index >= limit)
        index = 0;
    
    return colors[index];
}

float Scale(float lo, float hi, size_t div, size_t index)
{
    if (lo >= hi || div == 0 || index >= div)
        throw MakeException("Invalid parameters");
    
    float range = hi - lo;
    float size = range / div;
    
    return lo + (size * index);
}

int Round(float x)
{
    return static_cast<int>(x + 0.5);
}
