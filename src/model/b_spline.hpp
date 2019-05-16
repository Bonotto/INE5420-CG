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

#ifndef MODEL_BSPLINE_HPP
#define MODEL_BSPLINE_HPP

/* External includes */

/* Local includes */
#include "../config/traits.hpp"
#include "shape.hpp"

namespace model
{

/*================================================================================*/
/*                                   Definitions                                  */
/*================================================================================*/

	class BSpline : public Shape
	{
	public:
		BSpline(std::string name, const std::initializer_list<Vector>& vs) :
			Shape(name, vs)
		{}

		BSpline(std::string name, const std::vector<Vector>& vs) :
			Shape(name, vs)
		{}

		~BSpline() = default;

		virtual void clipping(const Vector & min, const Vector & max);
		
		void transformation(const Matrix & world_T);
		void w_transformation(const Matrix & window_T);

		void draw(const Cairo::RefPtr<Cairo::Context>& cr, const Matrix & viewport_T);

		virtual std::string type();

		static const double precision;
		static const double world_max_size;
		static const double window_max_size;

	private:
		bool over_perpendicular_edges(const Vector & pa, const Vector & pb);
		
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

	const double BSpline::precision = 0.01;
	const double BSpline::world_max_size = 800;
	const double BSpline::window_max_size = 10;

	void BSpline::transformation(const Matrix & world_T)
	{
		if (_world_vectors.size() < 4)
			return;

		for (auto & v : _world_vectors)
			v = v * world_T;
	}

	void BSpline::forward_differences(
		std::vector<double> & dX,
		std::vector<double> & dY,
		std::vector<double> & dZ,
		std::vector<Vector> & vectors
	)
	{
		if (!vectors.empty())
			vectors.emplace_back(dX[0], dY[0], dZ[0]);

		for (double k = precision; k <= 1; k += precision)
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

	void BSpline::w_transformation(const Matrix & window_T)
	{
		if (_world_vectors.size() < 4)
			return;

		static const Matrix D{
			{                    0,                     0,         0, 1},
			{    pow(precision, 3),     pow(precision, 2), precision, 0},
			{6 * pow(precision, 3), 2 * pow(precision, 2),         0, 0},
			{6 * pow(precision, 3),                     0,         0, 0}
		};

		static const Matrix IMbs{
			{-1.0/6.0, 1.0/2.0, -1.0/2.0, 1.0/6.0},
			{ 1.0/2.0,    -1.0,  1.0/2.0,       0},
			{-1.0/2.0,     0.0,  1.0/2.0,       0},
			{ 1.0/6.0, 2.0/3.0,  1.0/6.0,       0}
		};

		static const Matrix D_IMbs = D.multiply<4>(IMbs);

		std::vector<Vector> vectors;

		for (size_t k = 0; k < (_world_vectors.size() - 3); ++k)
		{
			Vector p1 = _world_vectors[k    ] * window_T;
			Vector p2 = _world_vectors[k + 1] * window_T;
			Vector p3 = _world_vectors[k + 2] * window_T;
			Vector p4 = _world_vectors[k + 3] * window_T;

			std::vector<double> pX{p1[0], p2[0], p3[0], p4[0]};
			std::vector<double> pY{p1[1], p2[1], p3[1], p4[1]};
			std::vector<double> pZ{p1[2], p2[2], p3[2], p4[2]};

			std::vector<double> dX = D_IMbs * pX;
			std::vector<double> dY = D_IMbs * pY;
			std::vector<double> dZ = D_IMbs * pZ;

			forward_differences(dX, dY, dZ, vectors);
		}

		_window_vectors = std::move(vectors);
	}

	void BSpline::clipping(const Vector & min, const Vector & max)
	{
		if (_window_vectors.size() < 4)
			return;

		std::vector<Vector> vectors;

		Vector aux;

		for (size_t a = 0; a < _window_vectors.size() - 1; ++a)
		{
			Vector pa = _window_vectors[a];
			Vector pb = _window_vectors[a + 1];

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

			vectors.emplace_back(pa[0] + p2 * rn1, pa[1] + p4 * rn1);
			vectors.emplace_back(pa[0] + p2 * rn2, pa[1] + p4 * rn2);
		}

		_window_vectors = vectors;
	}

	bool BSpline::over_perpendicular_edges(const Vector & pa, const Vector & pb)
	{
		static const double TOP    =  0.95;
		static const double RIGHT  =  0.95;
		static const double BOTTOM = -0.95;
		static const double LEFT   = -0.95;

		if (pa[1] == TOP || pa[1] == BOTTOM)
		{
			if (pb[0] == LEFT || pb[0] == RIGHT)
				return true;

			return pb[1] == TOP || pb[1] == BOTTOM;
		}

		else if (pa[0] == LEFT || pa[0] == RIGHT)
		{
			if (pb[1] == TOP || pb[1] == BOTTOM)
				return true;

			return pb[0] == LEFT || pb[0] == RIGHT;
		}

		return false;
	}

	void BSpline::draw(const Cairo::RefPtr<Cairo::Context>& cr, const Matrix & viewport_T)
	{
		if (_window_vectors.empty())
			return;

		Vector v0 = _window_vectors[0] * viewport_T;

		/* First point */
		cr->move_to(v0[0], v0[1]);

		/* First point to verify coordinates */
		v0 = _window_vectors[0];

		/* Draw all other points */
		for (Vector& v : _window_vectors)
		{
			Vector vi = v * viewport_T;

			if (over_perpendicular_edges(v, v0))
				cr->move_to(vi[0], vi[1]);
			else
				cr->line_to(vi[0], vi[1]);

			v0 = v;
		}
	}

	std::string BSpline::type()
	{
		return "B-Spline Curve";
	}


} //! namespace model

#endif  // MODEL_BSPLINE_HPP
