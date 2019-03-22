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

/* External includes */

/* Local includes */
#include "geometry.hpp"

namespace model
{

/*================================================================================*/
/*                                   Definitions                                  */
/*================================================================================*/

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
		const Matrix& transformation() const;
		Vector mass_center() const;
		const Vector& min() const;
		const Vector& max() const;

	private:
		Vector _lower, _upper;
		Matrix _dimensions;
		Gtk::DrawingArea& _draw_area;
	};

/*================================================================================*/
/*                                  Implementaion                                 */
/*================================================================================*/

	const double Window::width()
	{
		return transformations::euclidean_distance({_upper[0], 0}, {_lower[0], 0});
	}

	const double Window::height()
	{
		return transformations::euclidean_distance({0, _upper[1]}, {0, _lower[1]});
	}

	void Window::transformation(const Matrix& T)
	{
		_dimensions = _dimensions * T;
	}

	const Matrix& Window::transformation() const
	{
		return _dimensions;
	}

	Vector Window::mass_center() const
	{
		return Vector(
			(_lower[0] + _upper[0])/2,
			(_lower[1] + _upper[1])/2
		);
	}

	const Vector& Window::min() const
	{
		return _lower;
	}

	const Vector& Window::max() const
	{
		return _upper;
	}

} //! namespace model

#endif  // MODEL_WINDOW_HPP
