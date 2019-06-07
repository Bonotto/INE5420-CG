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

#ifndef MODEL_LINE_HPP
#define MODEL_LINE_HPP

/* External includes */

/* Local includes */
#include "../config/traits.hpp"
#include "shape.hpp"

namespace model
{

/*================================================================================*/
/*                                   Definitions                                  */
/*================================================================================*/

	class Line : public Shape
	{
	public:
	    enum class ClippingMethod
	    {
	        Cohen_Sutherland,
	        Liang_Barsky
	    };

		static ClippingMethod clipping_method;

		Line(std::string name, const Vector& world_v1, const Vector& world_v2) :
			Shape(name, {world_v1, world_v2})
		{}

		~Line() = default;

		void clipping(const Vector & min, const Vector & max) override;
		std::string type() override;

	private:
		void cohen_sutherland(const Vector & min, const Vector & max);
		void liang_barsky(const Vector & min, const Vector & max);
	};

/*================================================================================*/
/*                                 Implementaions                                 */
/*================================================================================*/

	Line::ClippingMethod Line::clipping_method{Line::ClippingMethod::Cohen_Sutherland};

	void Line::cohen_sutherland(const Vector & min, const Vector & max)
	{
		db<Line>(INF) << "[" << this << "] Cohen Sutherland" << std::endl;

		enum Region
		{
			Center  = 0x0, /*< 0000 */
			Left    = 0x1, /*< 0001 */
			Right   = 0x2, /*< 0010 */
			Down    = 0x4, /*< 0100 */
			Up      = 0x8, /*< 1000 */
		};

		auto get_location = [&](const Vector & p)
		{
			unsigned loc = Region::Center;

			if (p[1] > max[1])
				loc |= Region::Up;

			else if (p[1] < min[1])
				loc |= Region::Down;

			if (p[0] > max[0])
				loc |= Region::Right;

			else if (p[0] < min[0])
				loc |= Region::Left;

			return loc;
		};

		Vector paux;
		Vector & pa = _window_vectors[0];
		Vector & pb = _window_vectors[1];

		unsigned region_of_a = get_location(pa);
		unsigned region_of_b = get_location(pb);

		/* Are they outside? */
		if (region_of_a & region_of_b)
		{
			_window_vectors.clear();
			return;
		}

		for (int attempts = 0; attempts < 10 && (region_of_a || region_of_b); ++attempts)
		{
			Vector & p = region_of_a ? pa : pb;
			unsigned & region_of_p = region_of_a ? region_of_a : region_of_b;

			if (region_of_p & Region::Up)
			{
				paux[0] = pa[0] + (pb[0] - pa[0]) * (max[1] - pa[1]) / (pb[1] - pa[1]);
				paux[1] = max[1];
			}

			else if (region_of_p & Region::Down)
			{
				paux[0] = pa[0] + (pb[0] - pa[0]) * (min[1] - pa[1]) / (pb[1] - pa[1]);
				paux[1] = min[1];
			}

			else if (region_of_p & Region::Right)
			{
				paux[0] = max[0];
				paux[1] = pa[1] + (pb[1] - pa[1]) * (max[0] - pa[0]) / (pb[0] - pa[0]);
			}

			else if (region_of_p & Region::Left)
			{
				paux[0] = min[0];
				paux[1] = pa[1] + (pb[1] - pa[1]) * (min[0] - pa[0]) / (pb[0] - pa[0]);
			}

			p = paux;
			region_of_p = get_location(p);
		}

		/* Are they outside yet? */
		if (region_of_a || region_of_b)
			_window_vectors.clear();
	}

	void Line::liang_barsky(const Vector & min, const Vector & max)
	{
		db<Line>(INF) << "[" << this << "] Liang Barsky" << std::endl;

		Vector pa = _window_vectors[0];
		Vector pb = _window_vectors[1];

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
		{
			_window_vectors.clear();
			return;
		}

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
		{
			_window_vectors.clear();
			return;
		}

		_window_vectors[0][0] = pa[0] + p2 * rn1;
		_window_vectors[0][1] = pa[1] + p4 * rn1;

		_window_vectors[1][0] = pa[0] + p2 * rn2;
		_window_vectors[1][1] = pa[1] + p4 * rn2;
	}

	void Line::clipping(const Vector & min, const Vector & max)
	{
		switch (clipping_method)
		{
			case ClippingMethod::Cohen_Sutherland:
				cohen_sutherland(min, max);
				break;

			case ClippingMethod::Liang_Barsky:
				liang_barsky(min, max);
				break;

			default:
				break;
		}
	}

	std::string Line::type()
	{
		return "Line";
	}

} //! namespace model

#endif  // MODEL_LINE_HPP
