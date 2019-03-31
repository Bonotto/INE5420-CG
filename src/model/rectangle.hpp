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
		Rectangle(std::string name) :
			Shape(name, {Vector(), Vector()})
		{}

		Rectangle(std::string name, const Vector& v1, const Vector& v2) :
			Shape(name, {v1, v2})
		{}

		~Rectangle() = default;

		virtual void draw(const Cairo::RefPtr<Cairo::Context>& cr, const Matrix & T);

		virtual std::string type();
	};

/*================================================================================*/
/*                                 Implementaions                                 */
/*================================================================================*/

	void Rectangle::draw(const Cairo::RefPtr<Cairo::Context>& cr, const Matrix & T)
	{
		Vector v0 = _vectors[0] * T;
		Vector v1 = _vectors[1] * T;

		/* First point */
		cr->move_to(v0[0], v0[1]);

		// Draw all other points
		cr->line_to(v0[0], v0[1]);
		cr->line_to(v0[0], v1[1]);
		cr->line_to(v1[0], v1[1]);
		cr->line_to(v1[0], v0[1]);
		cr->line_to(v0[0], v0[1]);
	}

	std::string Rectangle::type()
	{
		return "Rectangle";
	}

} //! namespace model

#endif  // MODEL_RECTANGLE_HPP
