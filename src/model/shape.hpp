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

		Shape(std::string name, const Vector& v, bool close_path = false) :
			_name(name),
			_world_vectors({v}),
			_close_path(close_path)
		{
			auto mass = mass_center();
			std::cout << std::endl << "Mass: (" << mass[0] << ", " << mass[1] << ", " << mass[2] << ")" << std::endl;
			std::cout << std::endl << "Normal before: (" << _normal[0] << ", " << _normal[1] << ", " << _normal[2] << ")" << std::endl;
			_normal = _normal + mass;
			std::cout << std::endl << "Normal after: (" << _normal[0] << ", " << _normal[1] << ", " << _normal[2] << ")" << std::endl;
		}

		Shape(std::string name, const std::initializer_list<Vector>& vs, bool close_path = false) :
			_name(name),
			_world_vectors(vs),
			_close_path(close_path)
		{
			auto mass = mass_center();
			std::cout << std::endl << "Mass: (" << mass[0] << ", " << mass[1] << ", " << mass[2] << ")" << std::endl;
			std::cout << std::endl << "Normal before: (" << _normal[0] << ", " << _normal[1] << ", " << _normal[2] << ")" << std::endl;
			_normal = _normal + mass;
			std::cout << std::endl << "Normal after: (" << _normal[0] << ", " << _normal[1] << ", " << _normal[2] << ")" << std::endl;
		}

		Shape(std::string name, const std::vector<Vector>& vs, bool close_path = false) :
			_name(name),
			_world_vectors(vs),
			_close_path(close_path)
		{
			auto mass = mass_center();
			std::cout << std::endl << "Mass: (" << mass[0] << ", " << mass[1] << ", " << mass[2] << ")" << std::endl;
			std::cout << std::endl << "Normal before: (" << _normal[0] << ", " << _normal[1] << ", " << _normal[2] << ")" << std::endl;
			_normal = _normal + mass;
			std::cout << std::endl << "Normal after: (" << _normal[0] << ", " << _normal[1] << ", " << _normal[2] << ")" << std::endl;
		}

		virtual ~Shape() = default;

		virtual Vector mass_center() const;
		virtual Vector normal() const;

		virtual void clipping(const Vector & min, const Vector & max);

		virtual void w_transformation(const Matrix & window_T);
		virtual void transformation(const Matrix & world_T);
		virtual void draw(const Cairo::RefPtr<Cairo::Context>& cr, const Matrix & viewport_T);

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
		Vector _normal{0, 0, 1};
		const bool _close_path;
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

	Vector Shape::normal() const
	{
		return _normal;
	}

	void Shape::w_transformation(const Matrix & window_T)
	{
		std::vector<Vector> vectors;

		for (auto & v : _world_vectors)
			vectors.emplace_back(v * window_T);

		_window_vectors = std::move(vectors);
	}

	void Shape::transformation(const Matrix & world_T)
	{	
		for (auto & v : _world_vectors)
			v = v * world_T;

		_normal = _normal * world_T;
	}

	void Shape::draw(const Cairo::RefPtr<Cairo::Context>& cr, const Matrix & viewport_T)
	{
		if (_window_vectors.empty())
			return;

		std::cout << name() << std::endl;
		
		for (auto v: _window_vectors)
			std::cout << "(" << v[0] << ", " << v[1] << ", " << v[2] << ")" << std::endl;

		Vector v0 = _window_vectors[0] * viewport_T;

		/* First point */
		cr->move_to(v0[0], v0[1]);

		// Draw all other points
		for (Vector& v : _window_vectors)
		{
			Vector vi = v * viewport_T;
			cr->line_to(vi[0], vi[1]);
		}
		
		//! Complete path
		if (_close_path)
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

	void Shape::clipping(const Vector & min, const Vector & max)
	{
		db<Shape>(INF) << "[" << this << "] Clipping: I'm only a Shape dude!" << std::endl;
	}

} //! namespace model

#endif  // MODEL_SHAPE_HPP
