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

/* External includes */
#include <string>
#include <gtkmm/drawingarea.h>

/* Local includes */
#include "geometry.hpp"

namespace model
{

/*================================================================================*/
/*                                   Definitions                                  */
/*================================================================================*/

	class Shape
	{
	public:
		Shape()  = default;

		Shape(std::string name, const Vector& world_v, const Vector& window_v) :
			_name{name},
			_world_vectors{{world_v}},
			_window_vectors{{window_v}}
		{}

		Shape(std::string name, double world_x, double world_y, double window_x, double window_y, double world_z = Vector::z,
								double world_w = Vector::w, double window_z = Vector::z, double window_w = Vector::w) :
			_name{name},
			_world_vectors{{world_x, world_y, world_z, world_w}},
			_window_vectors{{window_x, window_y, window_z, window_w}}
		{}

		Shape(std::string name, const std::initializer_list<Vector>& world_vs, const std::initializer_list<Vector>& window_vs) :
			_name{name},
			_world_vectors{world_vs},
			_window_vectors{window_vs}
		{}

		Shape(std::string name, const std::vector<Vector>& world_vs, const std::vector<Vector>& window_vs) :
			_name{name},
			_world_vectors{world_vs},
			_window_vectors{window_vs}
		{}

		virtual ~Shape() = default;

		virtual Vector mass_center() const;
		
		virtual void transformation(const Matrix & T);
		virtual void draw(const Cairo::RefPtr<Cairo::Context>& cr, const Matrix & T);

		std::string name();
		virtual std::string type();

		friend Debug & operator<<(Debug & db, const Shape & s)
		{
			for (const Vector & v : s._world_vectors)
				db << v << std::endl;

			return db;
		}

	protected:
		std::string _name{"Shape"};
		std::vector<Vector> _world_vectors{{0, 0}};
		std::vector<Vector> _window_vectors{{0, 0}};
	};

/*================================================================================*/
/*                                 Implementaions                                 */
/*================================================================================*/

	Vector Shape::mass_center() const
	{
		double total = _world_vectors.size();
		double x = 0, y = 0, z = 0, w = 0;

		for (const auto &v : _world_vectors)
		{
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

	void Shape::transformation(const Matrix & T)
	{
		std::cout << "Shape before:" << std::endl;
		
		for (const Vector & v : _world_vectors)
		{
			std::cout << "[" << v[0];

			for (auto i = 1; i < Vector::dimension; ++i)
				std::cout << ", " << v[i];

			std::cout << "]";

		}

		
		for (auto & v : _world_vectors)
			v = v * T;


		std::cout  << "Shape after:" << std::endl;
	
		
		for (const Vector & v : _world_vectors)
		{
			std::cout << "[" << v[0];

			for (auto i = 1; i < Vector::dimension; ++i)
				std::cout << ", " << v[i];

			std::cout << "]";

		}
	}

	void Shape::draw(const Cairo::RefPtr<Cairo::Context>& cr, const Matrix & T)
	{
		Vector v0 = _world_vectors[0] * T;

		/* First point */
		cr->move_to(v0[0], v0[1]);

		// Draw all other points
		for (Vector& v : _world_vectors)
		{
			Vector vi = v * T;
			cr->line_to(vi[0], vi[1]);
		}
		
		//! Complete path
		cr->close_path();
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
