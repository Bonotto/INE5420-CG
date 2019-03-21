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

#ifndef MODEL_SHAPE_HPP
#define MODEL_SHAPE_HPP

#include <string>
#include <gtkmm/drawingarea.h>
#include "geometry.hpp"

namespace model
{
	
	class Shape
	{
	public:
		Shape()  = default;

		Shape(std::string name) :
			_name{name}
		{}

		Shape(std::string name, const Vector& v) :
			_name{name},
			_vectors{{v}}
		{}
		
		Shape(std::string name, double x, double y, double z = Vector::x, double w = Vector::w) :
			_name{name},
			_vectors{{x, y, z, w}}
		{}
		
		Shape(std::string name, std::initializer_list<Vector>& vs) :
			_name{name},
			_vectors{vs}
		{}

		Shape(std::string name, std::initializer_list<Vector>&& vs) :
			_name{name},
			_vectors{vs}
		{}

		~Shape() = default;

        virtual void draw(const Cairo::RefPtr<Cairo::Context>& cr, Matrix & T);

        std::string name();
        virtual std::string type();
	
	protected:
		std::string _name{"Shape"};
		std::vector<Vector> _vectors{{0, 0}};
	};

    void Shape::draw(const Cairo::RefPtr<Cairo::Context>& cr, Matrix & T)
    {
    	Vector v0 = _vectors[0] * T;

		/* First point */
		cr->move_to(v0[0], v0[1]);

		// Draw all other points
		for (Vector& v : _vectors)
		{
			Vector vi = v * T;
			cr->line_to(vi[0], vi[1]);
		}
    }

    std::string Shape::name()
    {
    	return _name;
    }

    std::string Shape::type()
    {
    	return "Shape_t";
    }

} //! namespace model

#endif  // MODEL_SHAPE_HPP
