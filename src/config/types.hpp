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

#ifndef TYPES_HPP
#define TYPES_HPP

/* General */
class Debug;

namespace sys
{
    enum Color {CLR = 1};
} //! namespace sys

namespace gui
{
    enum Color {CLR = 1};
} //! namespace gui

namespace control
{
    class MainControl;
    class ObjectLoader;
} //! namespace control

namespace model
{
    class Vector;
    class Matrix;
    class Shape;
    class Point;
    class Line;
    class Rectangle;
    class Polygon;
    class Bezier;
    class BSpline;
    class Window;
    class Viewport;
    class ComplexShape;
} //! namespace model

namespace view
{
    /* Empty */
} //! namespace view

#endif //! TYPES_HPP
