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
		Window(const Vector & min, const Vector & max) :
			_visible_world(Rectangle{"window", min, max}),
			_min{min},
			_max{max}
		{
			_visible_world.transformation(_history, normalization());
		}

		~Window() = default;

		const double width();
		const double height();

		virtual void transformation(const Matrix& T);

		virtual Vector normalization(const Vector& v);
		virtual Matrix& normalization();
		virtual Matrix& anormalization();

		const Matrix& transformation() const;
		const Vector& min() const;
		const Vector& max() const;
		const Rectangle& drawable();

	private:
		Rectangle _visible_world;
		Vector _min, _max;
		Matrix _history;
		Matrix _anorm;
		Matrix _norm;
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
		Matrix identity;

		if (_history == identity)
			_history = T;
		else
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
	
	const Rectangle& Window::drawable()
	{
		return _visible_world;
	}

	Matrix& Window::anormalization()
	{
		double x = (_max[0] - _min[0]) / 2;
		double y = (_max[1] - _min[1]) / 2;

		double a = x + _min[0];
		double b = y + _min[1];

		if (Vector::dimension == 3)
		{
			_anorm = Matrix(
					{x, 0, 0, 0},
					{0, y, 0, 0},
					{a, b, 1, 0},
					{0, 0, 0, 0}
				);
		}
		else
		{
			double z = (_max[2] - _min[2]) / 2;
			double c = z + _min[2];

			_anorm = Matrix(
					{x, 0, 0, 0},
					{0, y, 0, 0},
					{0, 0, z, 0},
					{a, b, c, 0}
				);
		}

		return _anorm;
	}

	Matrix& Window::normalization()
	{
		double x = 2 / (_max[0] - _min[0]);
		double y = 2 / (_max[1] - _min[1]);

		double a = (2 * _min[0]) / (_max[0] - _min[0]) + 1;
		double b = (2 * _min[1]) / (_max[1] - _min[1]) + 1;

		if (Vector::dimension == 3)
		{
			_norm = Matrix(
					{ x,  0, 0, 0},
					{ 0,  y, 0, 0},
					{-a, -b, 1, 0},
					{ 0,  0, 0, 0}
				);
		}
		else
		{
			double z = 2 / (_max[2] - _min[2]);
			double c = (2 * _min[2]) / (_max[2] - _min[2]) + 1;

			_norm = Matrix(
					{ x,  0,  0, 0},
					{ 0,  y,  0, 0},
					{ 0,  0,  z, 0},
					{-a, -b, -c, 0}
				);
		}

		return _norm;
	}
	
	Vector Window::normalization(const Vector& v)
	{
		return v * normalization();
	}

} //! namespace model

#endif  // MODEL_WINDOW_HPP
