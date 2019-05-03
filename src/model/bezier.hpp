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

#ifndef MODEL_BEZIER_HPP
#define MODEL_BEZIER_HPP

/* External includes */

/* Local includes */
#include "../config/traits.hpp"
#include "shape.hpp"

namespace model
{

/*================================================================================*/
/*                                   Definitions                                  */
/*================================================================================*/

	class Bezier : public Shape
	{
	public:
		Bezier(std::string name, const std::initializer_list<Vector>& vs) :
			Shape(name, vs)
		{}

		Bezier(std::string name, const std::vector<Vector>& vs) :
			Shape(name, vs)
		{}

		~Bezier() = default;

		virtual void clipping(const Vector & min, const Vector & max);
		
		void w_transformation(const Matrix & window_T);

		void draw(const Cairo::RefPtr<Cairo::Context>& cr, const Matrix & viewport_T);

		virtual std::string type();

		static const double precision;
	};

/*================================================================================*/
/*                                 Implementaions                                 */
/*================================================================================*/

	const double Bezier::precision = 0.01;

	void Bezier::w_transformation(const Matrix & window_T)
	{
		std::vector<Vector> vectors;

		Vector p1 = _world_vectors[0] * window_T;
		Vector p2 = _world_vectors[1] * window_T;
		Vector p3 = _world_vectors[2] * window_T;
		Vector p4 = _world_vectors[3] * window_T;

		/* Calculates the second point in window coordinates (the first point is the p1) */
		Vector pa = 0.970299 * p1 + 0.029403 * p2 + 0.000297 * p3 + 0.000001 * p4;
		
		/* Calculates the points generation precision where 0.0153106 is de maximum distance between two points
		   When the object is scaled then the distance between points grow, therefore, this distance under the
		   maximum distance allows calculating the proportion to decrease 0.01 precision and make the curve
		   maintains the same smooth */
		double gen_prec = precision / (calculation::euclidean_distance(p1, pa) / 0.0153106);

		/* Amount of anothers bezier curves interconnected */
		int bezier_curves = (_world_vectors.size() - 4) / 3;

		for (int k = 0; k <= bezier_curves; ++k)
		{
			for (double t = 0; t <= 1.0; t += gen_prec)
			{			
				Vector p =               std::pow(1 - t, 3) * p1
				         + 3 * t       * std::pow(1 - t, 2) * p2
				         + 3 * (1 - t) * std::pow(t, 2)     * p3
	                     +               std::pow(t, 3)     * p4;

	            vectors.push_back(std::move(p));
			}

			/* Get the points of next bezier curve (if has next) */
			if (k < bezier_curves)
			{
				p1 = p4;
				p2 = _world_vectors[3 * k + 4] * window_T;
				p3 = _world_vectors[3 * k + 5] * window_T;
				p4 = _world_vectors[3 * k + 6] * window_T;
			}
		}

		_window_vectors = std::move(vectors);
	}

	void Bezier::clipping(const Vector & min, const Vector & max)
	{
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

			vectors.emplace_back(std::move(Vector({pa[0] + p2 * rn1, pa[1] + p4 * rn1})));
			vectors.emplace_back(std::move(Vector({pa[0] + p2 * rn2, pa[1] + p4 * rn2})));
		}

		_window_vectors = vectors;
	}

	void Bezier::draw(const Cairo::RefPtr<Cairo::Context>& cr, const Matrix & viewport_T)
	{
		if (_window_vectors.empty())
			return;

		Vector v0 = _window_vectors[0] * viewport_T;

		/* First point */
		cr->move_to(v0[0], v0[1]);

		/* First point to verify coordinates */
		v0 = _window_vectors[0];

		// Draw all other points
		for (Vector& v : _window_vectors)
		{
			Vector vi = v * viewport_T;
			
			if (calculation::euclidean_distance(v, v0) > 2 * precision)
				cr->move_to(vi[0], vi[1]);
			else
				cr->line_to(vi[0], vi[1]);

			v0 = v;
		}
	}

	std::string Bezier::type()
	{
		return "Bezier Curve";
	}


} //! namespace model

#endif  // MODEL_BEZIER_HPP
