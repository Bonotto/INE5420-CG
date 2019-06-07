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

#ifndef MODEL_B_SPLINE_SURFACE_HPP
#define MODEL_B_SPLINE_SURFACE_HPP

/* External includes */

/* Local includes */
#include "../config/traits.hpp"
#include "shape.hpp"

namespace model
{

/*================================================================================*/
/*                                   Definitions                                  */
/*================================================================================*/

	class BSplineSurface : public Shape
	{
		enum COORD {x = 0, y = 1, z = 2};

	public:
		BSplineSurface(std::string name, const std::vector<std::vector<Vector>>& vs) :
			Shape(name),
			_control_vectors(vs)
		{
			_normal = _normal + mass_center();
		}

		~BSplineSurface() = default;

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

		Matrix build_snip(COORD coord, int i, int j);

		bool over_perpendicular_edges(const Vector & pa, const Vector & pb);

		void foward_update(Matrix & Gx, Matrix & Gy, Matrix & Gz);
		void forward_differences(
			std::vector<double> & dX,
			std::vector<double> & dY,
			std::vector<double> & dZ,
			std::vector<Vector> & vectors
		);
	};

/*================================================================================*/
/*                                 Implementaions                                 */
/*================================================================================*/

	const double BSplineSurface::precision = 0.05;
	const double BSplineSurface::world_max_size = 800;
	const double BSplineSurface::window_max_size = 10;

	void BSplineSurface::transformation(const Matrix & world_T)
	{
		if (_control_vectors.size() < 4 || _control_vectors.begin()->size() < 4)
			return;

		for (auto & line : _control_vectors)
			for (auto & v : line)
				v = v * world_T;

		_normal = _normal * world_T;
	}

	void BSplineSurface::w_transformation(const Matrix & window_T)
	{
		if (_control_vectors.size() < 4)
			return;

		_surface_vectors.clear();

		static const Matrix Ds{
			{                    0.0,                     0.0,         0.0, 1.0},
			{1.0 * pow(precision, 3), 1.0 * pow(precision, 2),   precision, 0.0},
			{6.0 * pow(precision, 3), 2.0 * pow(precision, 2),         0.0, 0.0},
			{6.0 * pow(precision, 3),                     0.0,         0.0, 0.0}
		};

		static const Matrix Dt{
			{0.0, 1.0 * pow(precision, 3), 6.0 * pow(precision, 3), 6.0 * pow(precision, 3)},
			{0.0, 1.0 * pow(precision, 2), 2.0 * pow(precision, 2),                     0.0},
			{0.0,               precision,                     0.0,                     0.0},
			{1.0,                     0.0,                     0.0,                     0.0}
		};

		static const Matrix IMbs{
			{-1.0/6.0, 1.0/2.0, -1.0/2.0, 1.0/6.0},
			{ 1.0/2.0,    -1.0,  1.0/2.0,     0.0},
			{-1.0/2.0,     0.0,  1.0/2.0,     0.0},
			{ 1.0/6.0, 2.0/3.0,  1.0/6.0,     0.0}
		};

		static const Matrix S = Ds.multiply<4>(IMbs);
		static const Matrix T = IMbs.multiply<4>(Dt);

		for (size_t m = 0; m < _control_vectors.size() - 3; ++m)
		{
			for (size_t n = 0; n < _control_vectors[0].size() - 3; ++n)
			{
				auto Gx = S.multiply<4>(build_snip(COORD::x, m, n).multiply<4>(T));
				auto Gy = S.multiply<4>(build_snip(COORD::y, m, n).multiply<4>(T));
				auto Gz = S.multiply<4>(build_snip(COORD::z, m, n).multiply<4>(T));

				std::vector<Vector> vectors;

				for (double k = 0; k < 1; k += precision)
				{
					vectors.clear();

					if (k > 0)
					{
						_surface_vectors.push_back({});
						
						std::vector<double> dX{Gx[0][0], Gx[0][1], Gx[0][2], Gx[0][3]};
						std::vector<double> dY{Gy[0][0], Gy[0][1], Gy[0][2], Gy[0][3]};
						std::vector<double> dZ{Gz[0][0], Gz[0][1], Gz[0][2], Gz[0][3]};

						forward_differences(dX, dY, dZ, vectors);

						for (auto &v: vectors)
							_surface_vectors.back().push_back(v * window_T);
					}
				
					foward_update(Gx, Gy, Gz);
				}

				Gx = (S * build_snip(COORD::x, m, n) * T).transpose();
				Gy = (S * build_snip(COORD::y, m, n) * T).transpose();
				Gz = (S * build_snip(COORD::z, m, n) * T).transpose();

				for (double k = 0; k < 1; k += precision)
				{
					vectors.clear();

					if (k > 0)
					{
						_surface_vectors.push_back({});
						
						std::vector<double> dX{Gx[0][0], Gx[0][1], Gx[0][2], Gx[0][3]};
						std::vector<double> dY{Gy[0][0], Gy[0][1], Gy[0][2], Gy[0][3]};
						std::vector<double> dZ{Gz[0][0], Gz[0][1], Gz[0][2], Gz[0][3]};

						forward_differences(dX, dY, dZ, vectors);

						for (auto &v: vectors)
							_surface_vectors.back().push_back(v * window_T);
					}
				
					foward_update(Gx, Gy, Gz);
				}
			}
		}
	}

	// void BSplineSurface::foward_update(Matrix &Gx, Matrix &Gy, Matrix &Gz)
	// {
	// 	for (int i = 0; i < 3; ++i)
	// 	{
	// 		Gx[i] = Gx[i] + Gx[i + 1];
	// 		Gy[i] = Gy[i] + Gy[i + 1];
	// 		Gz[i] = Gz[i] + Gz[i + 1];
	// 	}
	// }

	void BSplineSurface::forward_differences(
		std::vector<double> & dX,
		std::vector<double> & dY,
		std::vector<double> & dZ,
		std::vector<Vector> & vectors
	)
	{
		if (!vectors.empty())
			vectors.emplace_back(dX[0], dY[0], dZ[0]);

		for (double k = precision; k < 1; k += precision)
		{
			dX[0] += dX[1];
			dY[0] += dY[1];
			dZ[0] += dZ[1];

			dX[1] += dX[2];
			dY[1] += dY[2];
			dZ[1] += dZ[2];

			dX[2] += dX[3];
			dY[2] += dY[3];
			dZ[2] += dZ[3];

			vectors.emplace_back(dX[0], dY[0], dZ[0]);
		}
	}

	void BSplineSurface::foward_update(Matrix & Gx, Matrix & Gy, Matrix & Gz)
	{
		Gx[0][0] += Gx[1][0];
		Gx[0][1] += Gx[1][1];
		Gx[0][2] += Gx[1][2];
		Gx[0][3] += Gx[1][3];

		Gy[0][0] += Gy[1][0];
		Gy[0][1] += Gy[1][1];
		Gy[0][2] += Gy[1][2];
		Gy[0][3] += Gy[1][3];
		
		Gz[0][0] += Gz[1][0];
		Gz[0][1] += Gz[1][1];
		Gz[0][2] += Gz[1][2];
		Gz[0][3] += Gz[1][3];

		//row2 <- row2 + row3
		Gx[1][0] += Gx[2][0];
		Gx[1][1] += Gx[2][1];
		Gx[1][2] += Gx[2][2];
		Gx[1][3] += Gx[2][3];

		Gy[1][0] += Gy[2][0];
		Gy[1][1] += Gy[2][1];
		Gy[1][2] += Gy[2][2];
		Gy[1][3] += Gy[2][3];

		Gz[1][0] += Gz[2][0];
		Gz[1][1] += Gz[2][1];
		Gz[1][2] += Gz[2][2];
		Gz[1][3] += Gz[2][3];

		//row2 <- row2 + row3
		Gx[2][0] += Gx[3][0];
		Gx[2][1] += Gx[3][1];
		Gx[2][2] += Gx[3][2];
		Gx[2][3] += Gx[3][3];

		Gy[2][0] += Gy[3][0];
		Gy[2][1] += Gy[3][1];
		Gy[2][2] += Gy[3][2];
		Gy[2][3] += Gy[3][3];

		Gz[2][0] += Gz[3][0];
		Gz[2][1] += Gz[3][1];
		Gz[2][2] += Gz[3][2];
		Gz[2][3] += Gz[3][3];

	}

	void BSplineSurface::clipping(const Vector & min, const Vector & max)
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

				auto new_ya = pa[1] + p4 * rn1;
				auto new_yb = pa[1] + p4 * rn2;

				if (new_ya > max[1] && new_yb > max[1])
					continue;

				vectors.emplace_back(new_xa, new_ya);
				vectors.emplace_back(new_xb, new_yb);
			}

			line = vectors;
		}
	}

	Matrix BSplineSurface::build_snip(COORD coord, int i, int j)
	{
		Matrix R( //! Result
			{0.0, 0.0, 0.0, 0.0},
			{0.0, 0.0, 0.0, 0.0},
			{0.0, 0.0, 0.0, 0.0},
			{0.0, 0.0, 0.0, 0.0}
		);

		for (int m = i; m < i + 4; ++m)
			for (int n = j; n < j + 4; ++n)
				R[m - i][n - j] = _control_vectors[m][n][coord];

		return std::move(R);
	}

	bool BSplineSurface::over_perpendicular_edges(const Vector & pa, const Vector & pb)
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

	void BSplineSurface::draw(const Cairo::RefPtr<Cairo::Context>& cr, const Matrix & viewport_T)
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

	Vector BSplineSurface::mass_center() const
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

	void BSplineSurface::perspective()
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
				
				if (!v[2])
					continue;

				v[0] = v[0] * d / v[2];
				v[1] = v[1] * d / v[2];
				v[2] = d;
			}
	}

	std::string BSplineSurface::type()
	{
		return "Bezier Surface";
	}


} //! namespace model

#endif  // MODEL_B_SPLINE_SURFACE_HPP
