/* The MIT License
 * 
 * Copyright (c) 2019 João Vicente Souto and Bruno Izaias Bonotto
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#ifndef TRAITS_HPP
#define TRAITS_HPP

#include <iostream>
#include <string>

#include "types.hpp"

/*================================================================================*/
/*                               Global Definitions                               */
/*================================================================================*/

template<typename T>
struct Traits
{
    static const bool debugged = false;             /* Enable debugging.        */
    static const bool hysterically_debugged = true; /* Enables chain debugging. */
};

template<> struct Traits<Debug>
{
    static const bool error   = false; /* Prints erros.                */
    static const bool warning = false; /* Prints warnings.             */
    static const bool info    = true;  /* Prints relevant information. */
    static const bool trace   = true;  /* Prints function call trace.  */
};

/*================================================================================*/
/*                              Specific Definitions                              */
/*================================================================================*/

template<> struct Traits<control::MainControl> : public Traits<void>
{
    static const bool debugged = hysterically_debugged;
};

template<> struct Traits<model::Vector> : public Traits<void>
{
    static const int dimension = 3;
    static const int x = 0;
    static const int y = 0;
    static const int z = (dimension == 3 ? 1 : 0);
    static const int w = 1;
    static const bool debugged = hysterically_debugged;
};

template<> struct Traits<model::Matrix> : public Traits<void>
{
    static const bool debugged = hysterically_debugged;
};

template<> struct Traits<model::Shape> : public Traits<void>
{
    static const bool debugged = hysterically_debugged;
};

template<> struct Traits<model::Point> : public Traits<void>
{
    static const bool debugged = hysterically_debugged;
};

template<> struct Traits<model::Line> : public Traits<void>
{
    static const bool debugged = hysterically_debugged;
};

template<> struct Traits<model::Rectangle> : public Traits<void>
{
    static const bool debugged = hysterically_debugged;
};

template<> struct Traits<model::Polygon> : public Traits<void>
{
    static const bool debugged = hysterically_debugged;
};

template<> struct Traits<model::Bezier> : public Traits<void>
{
    static const bool debugged = hysterically_debugged;
};

template<> struct Traits<model::BSpline> : public Traits<void>
{
    static const bool debugged = hysterically_debugged;
};

template<> struct Traits<model::Window> : public Traits<void>
{
    static const bool need_clipping = true;                  /* Enables clipping. */
    static const bool debugged      = hysterically_debugged; /* Enables debug.    */
};

template<> struct Traits<model::Viewport> : public Traits<void>
{
    static const bool debugged = hysterically_debugged;
};

template<> struct Traits<model::ComplexShape> : public Traits<void>
{
    static const bool debugged = hysterically_debugged;
};

/*================================================================================*/
/*                             Auxiliar Definitions                               */
/*================================================================================*/

template<> struct Traits<sys::Color>
{
    static const std::string reset;
    static const std::string black;
    static const std::string red;
    static const std::string green;
    static const std::string yellow;
    static const std::string blue;
    static const std::string magenta;
    static const std::string cyan;
    static const std::string white;
    static const std::string bold_black;
    static const std::string bold_red;
    static const std::string bold_green;
    static const std::string bold_yellow;
    static const std::string bold_blue;
    static const std::string bold_magenta;
    static const std::string bold_cyan;
    static const std::string bold_white;
};

//! Need define out of struct
const std::string Traits<sys::Color>::reset{"\033[0m"};
const std::string Traits<sys::Color>::black{"\033[30m"};
const std::string Traits<sys::Color>::red{"\033[31m"};
const std::string Traits<sys::Color>::green{"\033[32m"};
const std::string Traits<sys::Color>::yellow{"\033[33m"};
const std::string Traits<sys::Color>::blue{"\033[34m"};
const std::string Traits<sys::Color>::magenta{"\033[35m"};
const std::string Traits<sys::Color>::cyan{"\033[36m"};
const std::string Traits<sys::Color>::white{"\033[37m"};
const std::string Traits<sys::Color>::bold_black{"\033[1m\033[30m"};
const std::string Traits<sys::Color>::bold_red{"\033[1m\033[31m"};
const std::string Traits<sys::Color>::bold_green{"\033[1m\033[32m"};
const std::string Traits<sys::Color>::bold_yellow{"\033[1m\033[33m"};
const std::string Traits<sys::Color>::bold_blue{"\033[1m\033[34m"};
const std::string Traits<sys::Color>::bold_magenta{"\033[1m\033[35m"};
const std::string Traits<sys::Color>::bold_cyan{"\033[1m\033[36m"};
const std::string Traits<sys::Color>::bold_white{"\033[1m\033[37m"};

/*================================================================================*/
/*               Debug Definitions: (Source: EPOS operation system)               */
/*================================================================================*/

/*--------------------------------------------------------------------------------*/
/*                              Base structures                                   */
/*--------------------------------------------------------------------------------*/

typedef std::basic_ostream<char, std::char_traits<char>> std_cout_type;
typedef std_cout_type& (*std_endl_type)(std_cout_type&);

class Debug
{
public:
    template<typename T>
    Debug & operator<<(const T & p) { std::cout << p; return *this; }

    Debug & operator<<(std_endl_type m) {
        m(std::cout);
        std::cout << Traits<sys::Color>::reset;
        return *this;
    }
}; 

class Null_Debug
{
public:
    template<typename T>
    Null_Debug & operator<<(const T & o) { return *this; }

    template<typename T>
    Null_Debug & operator<<(const T * o) { return *this; }

    Null_Debug & operator<<(std_endl_type m)
    {
        std::cout << Traits<sys::Color>::reset;
        return *this;
    }
};

template<bool debugged>
class Select_Debug: public Debug {};
template<>
class Select_Debug<false>: public Null_Debug {};

enum Debug_Error   {ERR = 1};
enum Debug_Warning {WRN = 2};
enum Debug_Info    {INF = 3};
enum Debug_Trace   {TRC = 4};

/*--------------------------------------------------------------------------------*/
/*                                     Error                                      */
/*--------------------------------------------------------------------------------*/

template<typename T>
inline Select_Debug<(Traits<T>::debugged && Traits<Debug>::error)> 
db(Debug_Error l)
{
    Select_Debug<(Traits<T>::debugged && Traits<Debug>::error)>() << Traits<sys::Color>::red;
    return Select_Debug<(Traits<T>::debugged && Traits<Debug>::error)>(); 
}

template<typename T1, typename T2>
inline Select_Debug<((Traits<T1>::debugged || Traits<T2>::debugged) && Traits<Debug>::error)> 
db(Debug_Error l)
{
    Select_Debug<((Traits<T1>::debugged || Traits<T2>::debugged) && Traits<Debug>::error)>() << Traits<sys::Color>::red;
    return Select_Debug<((Traits<T1>::debugged || Traits<T2>::debugged) && Traits<Debug>::error)>(); 
}

/*--------------------------------------------------------------------------------*/
/*                                    Warning                                     */
/*--------------------------------------------------------------------------------*/

template<typename T>
inline Select_Debug<(Traits<T>::debugged && Traits<Debug>::warning)> 
db(Debug_Warning l)
{
    Select_Debug<(Traits<T>::debugged && Traits<Debug>::warning)>() << Traits<sys::Color>::yellow;
    return Select_Debug<(Traits<T>::debugged && Traits<Debug>::warning)>(); 
}

template<typename T1, typename T2>
inline Select_Debug<((Traits<T1>::debugged || Traits<T2>::debugged) && Traits<Debug>::warning)>
db(Debug_Warning l)
{
    Select_Debug<((Traits<T1>::debugged || Traits<T2>::debugged) && Traits<Debug>::warning)>() << Traits<sys::Color>::yellow;
    return Select_Debug<((Traits<T1>::debugged || Traits<T2>::debugged) && Traits<Debug>::warning)>();
}

/*--------------------------------------------------------------------------------*/
/*                                      Info                                      */
/*--------------------------------------------------------------------------------*/

template<typename T>
inline Select_Debug<(Traits<T>::debugged && Traits<Debug>::info)> 
db(Debug_Info l)
{
    Select_Debug<(Traits<T>::debugged && Traits<Debug>::info)>() << Traits<sys::Color>::cyan;
    return Select_Debug<(Traits<T>::debugged && Traits<Debug>::info)>(); 
}

template<typename T1, typename T2>
inline Select_Debug<((Traits<T1>::debugged || Traits<T2>::debugged) && Traits<Debug>::info)>
db(Debug_Info l)
{
    Select_Debug<((Traits<T1>::debugged || Traits<T2>::debugged) && Traits<Debug>::info)>() << Traits<sys::Color>::cyan;
    return Select_Debug<((Traits<T1>::debugged || Traits<T2>::debugged) && Traits<Debug>::info)>();
}

/*--------------------------------------------------------------------------------*/
/*                                     Trace                                      */
/*--------------------------------------------------------------------------------*/

template<typename T>
inline Select_Debug<(Traits<T>::debugged && Traits<Debug>::trace)> 
db(Debug_Trace l)
{
    Select_Debug<(Traits<T>::debugged && Traits<Debug>::trace)>() << Traits<sys::Color>::white;
    return Select_Debug<(Traits<T>::debugged && Traits<Debug>::trace)>(); 
}

template<typename T1, typename T2>
inline Select_Debug<((Traits<T1>::debugged || Traits<T2>::debugged) && Traits<Debug>::trace)>
db(Debug_Trace l)
{
    Select_Debug<((Traits<T1>::debugged || Traits<T2>::debugged) && Traits<Debug>::trace)>() << Traits<sys::Color>::white;
    return Select_Debug<((Traits<T1>::debugged || Traits<T2>::debugged) && Traits<Debug>::trace)>();
}

#endif  // TRAITS_HPP
