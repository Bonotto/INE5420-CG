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

#ifndef MODEL_COMPLEX_SHAPE_HPP
#define MODEL_COMPLEX_SHAPE_HPP

/* Local includes */
#include "../config/traits.hpp"
#include "geometry.hpp"
#include "shape.hpp"

namespace model
{

/*================================================================================*/
/*                                   Definitions                                  */
/*================================================================================*/

	class ComplexShape
	{
	public:
		ComplexShape()  = default;

		ComplexShape(std::string name, const std::initializer_list<std::shared_ptr<Shape>>& ss) :
			_name(name),
			_shapes(ss)
		{}

		ComplexShape(std::string name, const std::vector<std::shared_ptr<Shape>>& ssize_t) :
			_name(name),
			_shapes(ss)
		{}

		virtual ~ComplexShape() = default;

		virtual Vector mass_center() const;

		virtual void clipping(const Vector & min, const Vector & max);

		virtual void w_transformation(const Matrix & window_T);
		virtual void transformation(const Matrix & world_T);
		virtual void draw(const Cairo::RefPtr<Cairo::Context>& cr, const Matrix & viewport_T);

		std::string name();
		virtual std::string type();

		friend Debug & operator<<(Debug & db, const ComplexShape & s)
		{
			for (const Vector & v : s._world_vectors)
				db << v << std::endl;

			return db;
		}

	protected:
		std::string _name{"ComplexShape"};
		std::vector<std::shared_ptr<Shape>> _shapes;
	};

/*================================================================================*/
/*                                 Implementaions                                 */
/*================================================================================*/

	Vector ComplexShape::mass_center() const
	{
		double total = _shapes.size();
		double x = 0, y = 0, z = 0, w = 0;

		for (const auto &s : _shapes)
		{
            const auto v = s->mass_center();

			x += v[0];
			y += v[1];
			z += v[2];
			w += v[3];
		}

		return Vector(
			x/total,
			y/total,
			z/total,
			w/total
		);
	}

	void ComplexShape::w_transformation(const Matrix & window_T)
	{
		for (auto & s : _shapes)
			s->w_transformation(window_T);
	}

	void ComplexShape::transformation(const Matrix & world_T)
	{	
		for (auto & s : _shapes)
			s->transformation(world_T);
	}

	void ComplexShape::draw(const Cairo::RefPtr<Cairo::Context>& cr, const Matrix & viewport_T)
	{
        for (auto & s : _shapes)
			s->draw(cr, viewport_T);
	}

	std::string ComplexShape::name()
	{
		return _name;
	}

	std::string ComplexShape::type()
	{
		return "ComplexShape_t";
	}

	void ComplexShape::clipping(const Vector & min, const Vector & max)
	{
		db<ComplexShape>(INF) << "[" << this << "] Clipping: I'm only a ComplexShape dude!" << std::endl;
	}

} //! namespace model

#endif  // MODEL_COMPLEX_SHAPE_HPP
