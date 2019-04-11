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
#include <utility>

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
		{}

		~Window() = default;

		const double width();
		const double height();

		virtual void transformation(const Matrix& T);
		virtual void draw(const Cairo::RefPtr<Cairo::Context>& cr, const Matrix & T);

		virtual Vector normalization(const Vector& v);
		virtual std::pair<Matrix&, Matrix&>* normalization();

		const Matrix& transformation() const;
		const Vector& min() const;
		const Vector& max() const;
		const Rectangle& drawable();

	private:
		Rectangle _visible_world;
		Vector _min, _max;
		Matrix _history;
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

	void Window::draw(const Cairo::RefPtr<Cairo::Context>& cr, const Matrix & T)
	{
		_visible_world.draw(cr, T);
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

	std::pair<Matrix&, Matrix&>* Window::normalization()
	{
		Matrix A, B;

		double x = 2 / (_max[0] - _min[0]);
		double y = 2 / (_max[1] - _min[1]);

		double a = (2 * _min[0]) / (_max[0] - _min[0]) + 1;
		double b = (2 * _min[1]) / (_max[1] - _min[1]) + 1;

		if (Vector::dimension == 3)
		{
			A = Matrix(
					{x, 0, 0, 0},
					{0, y, 0, 0},
					{0, 0, 1, 0},
					{0, 0, 0, 0}
				);

			B = Matrix(
					{ 1,  0, 0, 0},
					{ 0,  1, 0, 0},
					{-a, -b, 1, 0},
					{ 0,  0, 0, 0}
				);
		}
		else
		{
			double z = 2 / (_max[2] - _min[2]);
			double c = (2 * _min[2]) / (_max[2] - _min[2]) + 1;

			A = Matrix(
					{x, 0, 0, 0},
					{0, y, 0, 0},
					{0, 0, z, 0},
					{0, 0, 0, 0}
				);

			B = Matrix(
					{ 1,  0,  0, 0},
					{ 0,  1,  0, 0},
					{ 0,  0,  1, 0},
					{-a, -b, -c, 0}
				);
		}

		return new std::pair<Matrix&, Matrix&>(A, B);
	}
	
	Vector Window::normalization(const Vector& v)
	{
		auto norm_Ts = normalization();
		
		return (v * norm_Ts->first) * norm_Ts->second;
	}

} //! namespace model

#endif  // MODEL_WINDOW_HPP
