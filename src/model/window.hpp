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
			_visible_area(Rectangle{"window", min, max, normalization({min}), normalization({max})}),
			_min{min},
			_max{max}
		{
			Vector l0(1, 0, 0, 0);
			Vector l1(0, 1, 0, 0);
			Vector l2;
			Vector l3;

			if (Vector::dimension == 3)
			{
				l2 = Vector(-1, -1, 1, 0);
				l3 = Vector(0, 0, 0, 1);
			}
			else
			{
				l2 = Vector(0, 0, 1, 0);
				l3 = Vector(-1, -1, -1, 1);
			}

			_dimensions = Matrix(l0, l1, l2, l3);
		}

		~Window() = default;

		const double width();
		const double height();

		virtual void transformation(const Matrix& T);
		virtual void draw(const Cairo::RefPtr<Cairo::Context>& cr, const Matrix & T);

		virtual Vector& normalization(const std::vector<Vector>& v);
		
		const Matrix& transformation() const;
		const Vector& min() const;
		const Vector& max() const;
		const Rectangle drawable();

	private:
		Rectangle _visible_area;
		Vector _min, _max;
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
		_visible_area.draw(cr, T);
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
	
	const Rectangle Window::drawable()
	{
		return _visible_area;
	}
	
	Vector& Window::normalization(const std::vector<Vector>& v)
	{
		// xw_max (Window where x = max) and xw_min (Window where x = min)
		// are always 1 and -1, respectively
		// xvw min and max are the coordinates of visible world on window, respectively
		// xw = (xw_max - xw_min) / (xvw_max - xvw_min)
		// xw = xw * (xvw - xvw_min);
		// xw = xw + xw_min;
		// for yw the calculus is analogous

		Matrix B, C;
		Vector A;

		double x = 2 / (_max[0] - _min[0]);
		double y = 2 / (_max[1] - _min[1]);

		if (Vector::dimension == 3)
		{
			A = Vector(x, y, 1);

			B = Matrix( 
				{1, 		0, 			0, 0},
				{0, 		1, 			0, 0},
				{-_min[0],	-_min[1],	1, 0},
				{0, 		0, 			0, 0}
			);

			C = Matrix( 
				{ 1,  0,  0, 0},
				{ 0,  1,  0, 0},
				{-1, -1,  1, 0},
				{ 0,  0,  0, 0}
			);
		}
		else
		{
			double z = 2 / (_max[2] - _min[2]);

			A = Vector(x, y, z, 1);

			B = Matrix( 
				{1, 		0, 			0, 		0},
				{0, 		1, 			0, 		0},
				{0,			0,			1, 		0},
				{-_min[0], -_min[1], -_min[2], 	1}
			);

			C = Matrix( 
				{ 1,  0,  0, 0},
				{ 0,  1,  0, 0},
				{ 0,  0,  1, 0},
				{-1, -1, -1, 1}
			);
		}

		std::vector<Vector>* window_coord = new std::vector<Vector>();

		for (auto vector: v)
			window_coord->push_back((A * (vector * B)) * C);

		return window_coord;
	}

} //! namespace model

#endif  // MODEL_WINDOW_HPP
