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

#include "../config/traits.hpp"
#include "shape.hpp"

namespace model
{
	
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

		virtual Vector mass_center() const;
        virtual void draw(const Cairo::RefPtr<Cairo::Context>& cr, const Matrix & T, const Vector& vp_min, const Vector& vp_max, const Vector& win_min, const Vector& win_max);

        virtual std::string type();
	};

	Vector Rectangle::mass_center() const
	{
		return Vector(
            (_vectors[0][0] + _vectors[1][0])/2,
            (_vectors[0][1] + _vectors[1][1])/2
        );
	}

    void Rectangle::draw(const Cairo::RefPtr<Cairo::Context>& cr, const Matrix & T, const Vector& vp_min, const Vector& vp_max, const Vector& win_min, const Vector& win_max)
    {

    	std::cout << _name << std::endl;

    	Vector v0 = _vectors[0] * T;
    	Vector v1 = _vectors[1] * T;

    	double v0_x = (v0[0] - win_min[0]) / (win_max[0] - win_min[0]) * (vp_max[0] - vp_min[0]);
    	double v0_y = (1 - (v0[1] - win_min[1]) / (win_max[1] - win_min[1])) * (vp_max[1] - vp_min[1]);
    	double v1_x = (v1[0] - win_min[0]) / (win_max[0] - win_min[0]) * (vp_max[0] - vp_min[0]);
    	double v1_y = (1 - (v1[1] - win_min[1]) / (win_max[1] - win_min[1])) * (vp_max[1] - vp_min[1]);

   //  	std::cout << "Ret" << std::endl;
   //  	for (int i = 0; i < T.dimension; i++) {
   //  		for (int j = 0; j < T.dimension; j++)
   //  			std::cout << T[i][j] << " ";
			// std::cout << std::endl;
   //  	}

		/* First point */
		cr->move_to(
			(v0[0] - win_min[0]) / (win_max[0] - win_min[0]) * (vp_max[0] - vp_min[0]),
			(1 - (v0[1] - win_min[1]) / (win_max[1] - win_min[1])) * (vp_max[1] - vp_min[1])
		);

		// Draw all other points
		cr->line_to(v0_x, v0_y);
		cr->line_to(v0_x, v1_y);
		cr->line_to(v1_x, v1_y);
		cr->line_to(v1_x, v0_y);
		cr->line_to(v0_x, v0_y);
    }

    std::string Rectangle::type()
    {
    	return "Rectangle";
    }

} //! namespace model

#endif  // MODEL_RECTANGLE_HPP
