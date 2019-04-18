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

		enum Region
		{
			Left  = 0x1, /*< 0001 */
			Right = 0x2, /*< 0010 */
			Down  = 0x4, /*< 0100 */
			Up    = 0x8, /*< 1000 */
		};

		Line(std::string name, const Vector& world_v1, const Vector& world_v2) :
			Shape(name, {world_v1, world_v2})
		{}

		~Line() = default;

		virtual void clipping(const Vector & min, const Vector & max);
		virtual std::string type();

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

		unsigned loc_a = 0, loc_b = 0;
		
		Vector & pa = _window_vectors[0];
		Vector & pb = _window_vectors[1];

		Vector new_pa;
		Vector new_pb;

		auto func = [&](const Vector & p)
		{
			unsigned loc = 0;

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

		loc_a = func(pa);
		loc_b = func(pb);

		if (loc_a & loc_b)
		{
			_window_vectors.clear();
			return;
		}

		// Clipping for 'pa'
		while (loc_a)
		{
			if (loc_a & Region::Up)
			{
				new_pa[0] = pa[0] + (pb[0] - pa[0]) * (max[1] - pa[1]) / (pb[1] - pa[1]);
				new_pa[1] = max[1];
			}
			
			else if (loc_a & Region::Down)
			{
				new_pa[0] = pa[0] + (pb[0] - pa[0]) * (min[1] - pa[1]) / (pb[1] - pa[1]);
				new_pa[1] = min[1];
			}

			else if (loc_a & Region::Right)
			{
				new_pa[0] = max[0];
				new_pa[1] = pa[1] + (pb[1] - pa[1]) * (max[0] - pa[0]) / (pb[0] - pa[0]);
			}

			else if (loc_a & Region::Left)
			{
				new_pa[0] = min[0];
				new_pa[1] = pa[1] + (pb[1] - pa[1]) * (min[0] - pa[0]) / (pb[0] - pa[0]);
			}

			loc_a = func(new_pa);

			pa = new_pa;
		}
		
		// Clipping for 'pb'
		while (loc_b)
		{
			if (loc_b & Region::Up)
			{
				new_pb[0] = pa[0] + (pb[0] - pa[0]) * (max[1] - pa[1]) / (pb[1] - pa[1]);
				new_pb[1] = max[1];
			}
			
			else if (loc_b & Region::Down)
			{
				new_pb[0] = pa[0] + (pb[0] - pa[0]) * (min[1] - pa[1]) / (pb[1] - pa[1]);
				new_pb[1] = min[1];
			}

			else if (loc_b & Region::Right)
			{
				new_pb[0] = max[0];
				new_pb[1] = pa[1] + (pb[1] - pa[1]) * (max[0] - pa[0]) / (pb[0] - pa[0]);
			}

			else if (loc_b & Region::Left)
			{
				new_pb[0] = min[0];
				new_pb[1] = pa[1] + (pb[1] - pa[1]) * (min[0] - pa[0]) / (pb[0] - pa[0]);
			}

			loc_b = func(new_pb);
			
			pb = new_pb;
		}

		_window_vectors[0] = pa;
		_window_vectors[1] = pb;
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
