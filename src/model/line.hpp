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
	        Liang_Barsky,
	        Nicholl_Lee_Nicholl
	    };

		Line(std::string name, const Vector& world_v1, const Vector& world_v2) :
			Shape(name, {world_v1, world_v2})
		{}

		~Line() = default;

		void cohen_sutherland(const Vector & min, const Vector & max);
		void liang_barsky(const Vector & min, const Vector & max);
		void nicholl_lee_nicholl(const Vector & min, const Vector & max);

		virtual void clipping(const Vector & min, const Vector & max);
		virtual std::string type();

	private:
		ClippingMethod _clipping_method{ClippingMethod::Cohen_Sutherland};
	};

/*================================================================================*/
/*                                 Implementaions                                 */
/*================================================================================*/

	void Line::cohen_sutherland(const Vector & min, const Vector & max)
	{
		db<Line>(INF) << "[" << this << "] Cohen Sutherland" << std::endl;
		
		enum Region
		{
			Left  = 0x1, /*< 0001 */
			Right = 0x2, /*< 0010 */
			Down  = 0x4, /*< 0100 */
			Up    = 0x8, /*< 1000 */
		};

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
		
		// Clipping for 'pa'
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

		db<Line>(INF) << "[" << this << "] Clipping area: " << min << " x " << max << std::endl;
		db<Line>(INF) << "[" << this << "] Line: " << pa << " <-> " << pb << std::endl;
	}

	void Line::liang_barsky(const Vector & min, const Vector & max)
	{
		db<Line>(INF) << "[" << this << "] Liang Barsky" << std::endl;	
	}

	void Line::nicholl_lee_nicholl(const Vector & min, const Vector & max)
	{
		db<Line>(INF) << "[" << this << "] Nicholl Lee Nicholl" << std::endl;	
	}

	void Line::clipping(const Vector & min, const Vector & max)
	{
		// db<Line>(INF) << "[" << this << "] Line: " << _window_vectors[0] << " x " << _window_vectors[1] << std::endl;
		// db<Line>(INF) << "[" << this << "] Clipping area: " << min << " x " << max << std::endl;

		switch (_clipping_method)
		{
			case ClippingMethod::Cohen_Sutherland:
				cohen_sutherland(min, max);
				break;

			case ClippingMethod::Liang_Barsky:
				liang_barsky(min, max);
				break;

			case ClippingMethod::Nicholl_Lee_Nicholl:
				nicholl_lee_nicholl(min, max);
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
