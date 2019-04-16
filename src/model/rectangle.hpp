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

#ifndef MODEL_RECTANGLE_HPP
#define MODEL_RECTANGLE_HPP

/* External includes */

/* Local includes */
#include "../config/traits.hpp"
#include "shape.hpp"

namespace model
{

/*================================================================================*/
/*                                   Definitions                                  */
/*================================================================================*/

	class Rectangle : public Shape
	{
	public:
		Rectangle(std::string name, const Vector& world_v1, const Vector& world_v2) :
			Shape(name, {world_v1, Vector(world_v1[0], world_v2[1]), world_v2, Vector(world_v2[0], world_v1[1])})
		{}

		~Rectangle() = default;

		virtual std::string type();
	};

/*================================================================================*/
/*                                 Implementaions                                 */
/*================================================================================*/

	std::string Rectangle::type()
	{
		return "Rectangle";
	}

} //! namespace model

#endif  // MODEL_RECTANGLE_HPP
