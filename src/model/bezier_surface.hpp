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

#ifndef MODEL_BEZIER_SURFACE_HPP
#define MODEL_BEZIER_SURFACE_HPP

/* External includes */

/* Local includes */
#include "../config/traits.hpp"
#include "shape.hpp"

namespace model
{

/*================================================================================*/
/*                                   Definitions                                  */
/*================================================================================*/

	class BezierSurface : public Shape
	{
		enum COORD {x, y, z};

	public:
		BezierSurface(std::string name, const std::vector<std::vector<Vector>>& vs) :
			Shape(name),
			_control_vectors(vs)
		{
			_normal = _normal + mass_center();
		}

		~BezierSurface() = default;

		virtual void clipping(const Vector & min, const Vector & max);
		
		void transformation(const Matrix & world_T);
		void w_transformation(const Matrix & window_T);

		void draw(const Cairo::RefPtr<Cairo::Context>& cr, const Matrix & viewport_T);

		virtual Vector mass_center() const;

		virtual void perspective();

		virtual std::string type();

		static const double precision;
		static const double world_max_size;
		static const double window_max_size;

	private:
		std::vector<std::vector<Vector>> _control_vectors;
		std::vector<std::vector<Vector>> _surface_vectors;

		bool over_perpendicular_edges(const Vector & pa, const Vector & pb);
		Matrix build_snip(COORD coord, int i, int j, const Matrix & W);
	};

/*================================================================================*/
/*                                 Implementaions                                 */
/*================================================================================*/

	const double BezierSurface::precision = 0.05;
	const double BezierSurface::world_max_size = 800;
	const double BezierSurface::window_max_size = 10;

	void BezierSurface::transformation(const Matrix & world_T)
	{
		if (_control_vectors.size() < 4 || _control_vectors.begin()->size() < 4)
			return;

		for (auto & line : _control_vectors)
			for (auto & v : line)
				v = v * world_T;

		_normal = _normal * world_T;
	}

	void BezierSurface::w_transformation(const Matrix & window_T)
	{
		if (_control_vectors.size() < 4)
			return;
		
		static const Matrix M{
			{-1.0,  3.0, -3.0, 1.0},
			{ 3.0, -6.0,  3.0, 0.0},
			{-3.0,  3.0,  0.0, 0.0},
			{ 1.0,  0.0,  0.0, 0.0}
		};

		std::vector<std::vector<Vector>> lines;

		/* Amount of anothers bezier curves interconnected */
		size_t bezier_surfaces = (_control_vectors.size() - 4) / 3;

		for (size_t m = 0, si = 0; m <= bezier_surfaces; m += 3)
		{
			for (size_t n = 0; n <= bezier_surfaces; n += 3)
			{
				const auto Mx = build_snip(COORD::x, m, n, window_T) * M;
				const auto My = build_snip(COORD::y, m, n, window_T) * M;
				const auto Mz = build_snip(COORD::z, m, n, window_T) * M;

				for (double s = 0; s <= 1.0; s += precision, ++si)
				{
					lines.push_back({});

					const Vector sx = Vector{s*s*s, s*s, s, 1}.multiply<4>(M) * Mx;
					const Vector sy = Vector{s*s*s, s*s, s, 1}.multiply<4>(M) * My;
					const Vector sz = Vector{s*s*s, s*s, s, 1}.multiply<4>(M) * Mz;

					for (double t = 0; t <= 1.0; t += precision)
					{
						const std::vector<double> vt{t*t*t, t*t, t, 1};

						double x = sx * vt;
						double y = sy * vt;
						double z = sz * vt;

			            lines[si].emplace_back(x, y, z);
					}
				}
			}
		}

		std::vector<std::vector<Vector>> columns(lines[0].size());

		for (size_t j = 0; j < lines[0].size(); ++j)
			for (size_t i = 0; i < lines.size(); ++i)
				columns[j].push_back(lines[i][j]);

		_surface_vectors = std::move(lines);

		for (auto line: columns)
			_surface_vectors.push_back(line);
	}

	void BezierSurface::clipping(const Vector & min, const Vector & max)
	{
		if (_surface_vectors.size() < 4)
			return;

		for (auto &line: _surface_vectors)
		{
			std::vector<Vector> vectors;

			Vector aux;

			for (size_t a = 0; a < line.size() - 1; ++a)
			{
				Vector pa = line[a];
				Vector pb = line[a + 1];

				double p4 = pb[1] - pa[1];
				double p3 = -p4;
				double p2 = pb[0] - pa[0];
				double p1 = -p2;

				double q1 = pa[0] - min[0];
				double q2 = max[0] - pa[0];
				double q3 = pa[1] - min[1];
				double q4 = max[1] - pa[1];

				double positive[5], negative[5];

				int pos_index = 1, neg_index = 1;

				positive[0] = 1;
				negative[0] = 0;

				if ((p1 == 0 && q1 < 0) || (p3 == 0 && q3 < 0))
					continue;

				if (p1 != 0)
				{
					double r1 = q1 / p1;
					double r2 = q2 / p2;

					negative[neg_index++] = p1 < 0 ? r1 : r2;
					positive[pos_index++] = p1 < 0 ? r2 : r1;
				}

				if (p3 != 0)
				{
					double r3 = q3 / p3;
					double r4 = q4 / p4;

					negative[neg_index++] = p3 < 0 ? r3: r4;
					positive[pos_index++] = p3 < 0 ? r4: r3;
				}

				double rn1 = negative[0];
				double rn2 = positive[0];

				for (int i = 1; i < neg_index; ++i)
					if (rn1 < negative[i])
						rn1 = negative[i];

				for (int i = 1; i < pos_index; ++i)
					if (rn2 > positive[i])
						rn2 = positive[i];

				if (rn1 > rn2)
					continue;

				auto new_xa = pa[0] + p2 * rn1;
				auto new_xb = pa[0] + p2 * rn2;

				if (new_xa > max[0] && new_xb > max[0])
					continue;

				vectors.emplace_back(new_xa, pa[1] + p4 * rn1);
				vectors.emplace_back(new_xb, pa[1] + p4 * rn2);
			}

			line = vectors;
		}
	}

	Matrix BezierSurface::build_snip(COORD coord, int i, int j, const Matrix & W)
	{
		Matrix R( //! Result
			{0.0, 0.0, 0.0, 0.0},
			{0.0, 0.0, 0.0, 0.0},
			{0.0, 0.0, 0.0, 0.0},
			{0.0, 0.0, 0.0, 0.0}
		);

		for (int m = i; m < i + 4; ++m)
			for (int n = j; n < j + 4; ++n)
				R[m - i][n - j] = (_control_vectors[m][n] * W)[coord];

		return std::move(R);
	}

	bool BezierSurface::over_perpendicular_edges(const Vector & pa, const Vector & pb)
	{
		static const double TOP    =  0.95;
		static const double RIGHT  =  0.95;
		static const double BOTTOM = -0.95;
		static const double LEFT   = -0.95;

		if (pa[1] == TOP || pa[1] == BOTTOM)
			return (pb[0] == LEFT || pb[0] == RIGHT);

		else if (pa[0] == LEFT || pa[0] == RIGHT)
			return (pb[1] == TOP || pb[1] == BOTTOM);

		return false;
	}

	void BezierSurface::draw(const Cairo::RefPtr<Cairo::Context>& cr, const Matrix & viewport_T)
	{
		if (_surface_vectors.empty())
			return;

		for (const auto &curve: _surface_vectors)
		{
			/* First point to verify coordinates */
			Vector v0 = curve[0] * viewport_T;
			
			cr->move_to(v0[0], v0[1]);

			/* Draw all other points */
			for (const Vector& v : curve)
			{
				Vector vi = v * viewport_T;

				if (over_perpendicular_edges(v, v0))
					cr->move_to(vi[0], vi[1]);
				else
					cr->line_to(vi[0], vi[1]);

				v0 = v;
			}
		}
	}

	Vector BezierSurface::mass_center() const
	{
		double total = _control_vectors.size() * _control_vectors.begin()->size();
		double x = 0, y = 0, z = 0, w = 0;

		for (const auto &l : _control_vectors)
			for (const auto &v : l)
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

	void BezierSurface::perspective()
	{
		const double d = Traits<model::Window>::perspective_factor;

		Matrix M(
			{1, 0, 0, 0},
			{0, 1, 0, 0},
			{0, 0, 1, 0},
			{0, 0, d, 1}
		);

		for (auto &line: _surface_vectors)
			for (auto &v: line)
			{
				v = v * M;

				std::cout << v[2] << std::endl << std::endl << std::endl;
				
				if (!v[2])
					continue;

				v[0] = v[0] * d / v[2];
				v[1] = v[1] * d / v[2];
				v[2] = d;
			}
	}

	std::string BezierSurface::type()
	{
		return "Bezier Surface";
	}


} //! namespace model

#endif  // MODEL_BEZIER_SURFACE_HPP
