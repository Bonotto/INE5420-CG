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

	class Window
	{
	public:
		const static Vector fixed_min;
		const static Vector fixed_max;

		Window(const Vector & min, const Vector & max) :
			_min(min),
			_max(max),
			_visible_world(
				Rectangle(
					"window",
					Vector(min[0] - 0.05*min[0], min[1] - 0.05*min[1], min[0] - 0.05*min[0]),
					Vector(max[0] - 0.05*max[0], max[1] - 0.05*max[1], max[0] - 0.05*max[0])
				)
			)
		{
			_min[2] = -100;
			_max[2] = 100;

			_visible_world.w_transformation(normalization());
		}

		~Window() = default;

		const double width();
		const double height();

		void transformation(const Matrix& T);

		Matrix normalization();

		const Matrix& transformation() const;
		const Vector& min() const;
		const Vector& max() const;
		
		Rectangle& drawable();

	private:
		Matrix _history;
		Vector _min, _max;
		Rectangle _visible_world;
	};

/*================================================================================*/
/*                                 Implementaions                                 */
/*================================================================================*/

	const Vector Window::fixed_min{-1, -1, -1};
	const Vector Window::fixed_max{ 1,  1,  1};

	const double Window::width()
	{
		return calculation::euclidean_distance({_min[0], 0}, {_max[0], 0});
	}

	const double Window::height()
	{
		return calculation::euclidean_distance({0, _min[1]}, {0, _max[1]});
	}

	void Window::transformation(const Matrix& T)
	{
		_history = _history * T;
	}

	const Matrix& Window::transformation() const
	{
		return _history;
	}

	const Vector& Window::min() const
	{
		return _min;
	}

	const Vector& Window::max() const
	{
		return _max;
	}
	
	Rectangle& Window::drawable()
	{
		return _visible_world;
	}

	Matrix Window::normalization()
	{
		double x = 2 / (_max[0] - _min[0]);
		double y = 2 / (_max[1] - _min[1]);

		double a = (2 * _min[0]) / (_max[0] - _min[0]) + 1;
		double b = (2 * _min[1]) / (_max[1] - _min[1]) + 1;

		Vector l0{ x,  0, 0, 0};
		Vector l1{ 0,  y, 0, 0};
		Vector l2{-a, -b, 1, 0};
		Vector l3{ 0,  0, 0, 1};

		// if (Vector::dimension == 4)
		// {
		// 	double z = 2 / (_max[2] - _min[2]);
		// 	double c = (2 * _min[2]) / (_max[2] - _min[2]) + 1;

		// 	l2 = Vector{ 0,  0,  z, 0};
		// 	l3 = Vector{-a, -b, -c, 1};
		// }

		return Matrix(l0, l1, l2, l3);
	}

} //! namespace model

#endif  // MODEL_WINDOW_HPP
