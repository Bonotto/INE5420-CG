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

#ifndef MODEL_POLYGON_HPP
#define MODEL_POLYGON_HPP

#include "../config/traits.hpp"
#include "shape.hpp"

namespace model
{
	
	class Polygon : public Shape
	{
	public:
		Polygon(std::string name) :
			Shape(name)
		{}

		Polygon(std::string name, std::initializer_list<Vector>& vs) :
			Shape(name, vs)
		{}

		Polygon(std::string name, std::initializer_list<Vector>&& vs) :
			Shape(name, vs)
		{}

		~Polygon() = default;

        virtual std::string type();

        virtual void draw(const Cairo::RefPtr<Cairo::Context>& cr, Matrix & T);
	};

    void Polygon::draw(const Cairo::RefPtr<Cairo::Context>& cr, Matrix & T)
    {
    	Vector vo = _vectors[0] * T;

		/* First point */
		cr->move_to(v0[0], v0[1]);

		// Draw all other points
		for (Vector& v : _vectors) {
			Vector vi = v * T;
			cr->line_to(vi[0], vi[1]);
		}

		cr->line_to(v0[0], v0[1]);
    }

    std::string Polygon::type()
    {
    	return "Polygon";
    }

} //! namespace model

#endif  // MODEL_POLYGON_HPP
