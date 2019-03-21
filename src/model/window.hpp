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

#ifndef MODEL_WINDOW_HPP
#define MODEL_WINDOW_HPP

#include "geometry.hpp"

namespace model
{

	class Window
	{
	public:
		Window(const Vector & lower, const Vector & upper, Gtk::DrawingArea& draw_area) :
			_lower(lower),
            _upper(upper),
            _draw_area(draw_area)
		{
            if (Traits<model::Vector>::dimension == 3)
            {
                _dimensions = {
                    {1, 0, 0, 0},
                    {0, 1, 0, 0},
                    {_lower    },
                    {0, 0, 0, 1}
                };
            }
            else
            {
                _dimensions = {
                    {1, 0, 0, 0},
                    {0, 1, 0, 0},
                    {0, 0, 1, 0},
                    {_lower    }
                };
            }
		}

		~Window() = default;

        const double width();
        const double height();

        void transformation(const Matrix& M);
        Matrix transformation() const;

	private:
		Vector _lower, _upper;
        Matrix _dimensions;
        Gtk::DrawingArea& _draw_area;
	};

    const double Window::width()
    {
        return _upper[0] - _lower[0]; //! Need Euclidean distance!
    }

    const double Window::height()
    {
        return _upper[1] - _lower[1]; //! Need Euclidean distance!
    }

    void Window::transformation(const Matrix& matrix)
    {
        _dimensions = _dimensions * matrix;
    }

    Matrix Window::transformation() const
    {
        auto alloc = _draw_area.get_allocation();
		auto width = alloc.get_width();
		auto height = alloc.get_height();

        // Matrix P(
        //     {(width/2),           0, 0, 0},
        //     {        0, (-height/2), 0, 0},
        //     {        0,           0, 1, 0},
        //     {(width/2),  (height/2), 0, 1}
        // );

        // Matrix P(
        //     {1, 0, 0, 0},
        //     {0, 1, 0, 0},
        //     {0, 0, 1, 0},
        //     {0, 0, 0, 1}
        // );

        return _dimensions;
    }

} //! namespace model

#endif  // MODEL_WINDOW_HPP
