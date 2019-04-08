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
		Point(std::string name, const Vector& world_v, const Vector& window_v) :
			Shape(name, {world_v}, {window_v})
		{}

		Point(std::string name, double world_x, double world_y, double window_x, double window_y, double world_z = Vector::z,
								double world_w = Vector::w, double window_z = Vector::z, double window_w = Vector::w) :
			Shape(name, Vector(world_x, world_y, world_z, world_w), Vector(window_x, window_y, window_z, window_w))
		{}

		~Point() = default;

		virtual std::string type();
	};

/*================================================================================*/
/*                                 Implementaions                                 */
/*================================================================================*/

	std::string Point::type()
	{
		return "Point";
	}

} //! namespace model

#endif  // MODEL_POINT_HPP
