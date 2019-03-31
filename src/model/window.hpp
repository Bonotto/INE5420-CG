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
#include "rectangle.hpp"

namespace model
{

/*================================================================================*/
/*                                   Definitions                                  */
/*================================================================================*/

	class Window : public Rectangle
	{
	public:
		Window(const Vector & min, const Vector & max) :
			Rectangle{"window", min, max},
			_min{_vectors[0]},
			_max{_vectors[1]}
		{
			Vector l0(1, 0, 0, 0);
			Vector l1(0, 1, 0, 0);
			Vector l2;
			Vector l3;

			if (Vector::dimension == 3)
			{
				l2 = Vector(_min[0], _min[1], 1, 0);
				l3 = Vector(0, 0, 0, 1);
			}
			else
			{
				l2 = Vector(0, 0, 1, 0);
				l3 = Vector(_min[0], _min[1], _min[2], 1);
			}

			_dimensions = Matrix(l0, l1, l2, l3);
		}

		~Window() = default;

		const double width();
		const double height();

		virtual void transformation(const Matrix& T);
		virtual void draw(const Cairo::RefPtr<Cairo::Context>& cr, const Matrix & T);
		const Matrix& transformation() const;
		const Vector& min() const;
		const Vector& max() const;

	private:
		Vector &_min, &_max;
		Matrix _dimensions;
	};

/*================================================================================*/
/*                                 Implementaions                                 */
/*================================================================================*/

	const double Window::width()
	{
		return calculation::euclidean_distance({_max[0], 0}, {_min[0], 0});
	}

	const double Window::height()
	{
		return calculation::euclidean_distance({0, _max[1]}, {0, _min[1]});
	}

	void Window::transformation(const Matrix& T)
	{
		_dimensions = _dimensions * T;
	}

	void Window::draw(const Cairo::RefPtr<Cairo::Context>& cr, const Matrix & T)
	{
		/* Do not draw the window for now. */
	}

	const Matrix& Window::transformation() const
	{
		return _dimensions;
	}

	const Vector& Window::min() const
	{
		return _min;
	}

	const Vector& Window::max() const
	{
		return _max;
	}

} //! namespace model

#endif  // MODEL_WINDOW_HPP
