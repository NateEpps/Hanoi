//
// Util.hpp
// Nathanael Epps
//

#ifndef UTIL_HPP
#define UTIL_HPP

#include <string>

// Make my life easier
using std::string;

// Functions
#include <SFML/Graphics.hpp>

enum class WindowMode { Desktop, Fullscreen, TryFullscreen };

void GetVideoMode(WindowMode, sf::VideoMode&, sf::Uint32&);

typedef unsigned long long ReallyLong;

ReallyLong AmtHanoiMoves(ReallyLong);

string GetHanoiMoves(size_t amt,
                     size_t from = 1,
                     size_t dest = 3);

string GetHanoiMovesAsync(size_t, std::ostream&);

bool IsClose(sf::Vector2f,
             sf::Vector2f,
             float t = 10);
bool IsClose(float, float, float t = 0.2);

sf::Color GetRandColor();

float Scale(float lo, float hi,
            size_t div, size_t index);

int Round(float);

// MakeException

#include <stdexcept>

#define MakeException(s) std::runtime_error(__PRETTY_FUNCTION__ + std::string(":\n\t") + s)

// lexical_cast

#include <sstream>

namespace util
{
    template <class RType, class InType>
    struct conv {
        static RType convert(InType obj) {
            RType r;
            std::stringstream() << obj >> r;
            return r;
        }
    };
    
    template <class InType>
    struct conv<std::string, InType> {
        inline static std::string convert(InType obj) {
            return (std::stringstream() << obj).str();
        }
    };
}

template <class RType, class InType>
inline RType lexical_cast(InType obj)
{
    return util::conv<RType, InType>::convert(obj);
}

// print out containers
#include <vector>
#include <deque>

template <class T>
std::ostream& operator<<(std::ostream& stream, const sf::Vector2<T>& v)
{
    stream << "(" << v.x << ", " << v.y << ")";
    return stream;
}

template <class T>
std::ostream& operator<<(std::ostream& stream, const std::vector<T>& vec)
{
    std::stringstream ss;
    ss << "{";
    
    for (const T& t : vec)
        ss << t << ", ";
    
    stream << ss.str().substr(0, ss.str().length() - 2) + "}";
    
    return stream;
}

template <class T>
std::ostream& operator<<(std::ostream& stream, const std::deque<T>& vec)
{
    std::stringstream ss;
    ss << "{";
    
    for (const T& t : vec)
        ss << t << ", ";
    
    stream << ss.str().substr(0, ss.str().length() - 2) + "}";
    
    return stream;
}

#ifdef DEBUG
#include <iostream>
#define Print() std::cout << __PRETTY_FUNCTION__ << '\n'
#else
#define Print() (void) 0
#endif

#endif
