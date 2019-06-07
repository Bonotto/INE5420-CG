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

#ifndef MODEL_POINT_HPP
#define MODEL_POINT_HPP

/* External includes */

/* Local includes */
#include "../config/traits.hpp"
#include "shape.hpp"

namespace model
{

/*================================================================================*/
/*                                   Definitions                                  */
/*================================================================================*/

	class Point : public Shape
	{
	public:
		Point(std::string name, const Vector& v) :
			Shape(name, v)
		{}

		/* Use world coordinates to create a point */
		Point(std::string name, double x, double y, double z = Vector::z, double w = Vector::w) :
			Shape(name, Vector(x, y, z, w))
		{}

		~Point() = default;

		void clipping(const Vector & min, const Vector & max) override;
		std::string type() override;
	};

/*================================================================================*/
/*                                 Implementaions                                 */
/*================================================================================*/

	void Point::clipping(const Vector & min, const Vector & max)
	{
		db<Point>(INF) << "[" << this << "] Clipping Point" << std::endl;

		Vector & p = _window_vectors[0];

		bool x_is_outside = p[0] < min[0] || p[0] > max[0];
		bool y_is_outside = p[1] < min[1] || p[1] > max[1];
		
		if (x_is_outside || y_is_outside)
			_window_vectors.clear();
	}

	std::string Point::type()
	{
		return "Point";
	}

} //! namespace model

#endif  // MODEL_POINT_HPP
