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
		Line(std::string name, const Vector& world_v1, const Vector& world_v2) :
			Shape(name, {world_v1, world_v2})
		{}

		~Line() = default;

		void cohen_sutherland(const Vector & min, const Vector & max);
		void liang_barsky(const Vector & min, const Vector & max);
		void nicholl_lee_nicholl(const Vector & min, const Vector & max);

		virtual void clipping(const Vector & min, const Vector & max);
		virtual std::string type();
	};

/*================================================================================*/
/*                                 Implementaions                                 */
/*================================================================================*/

	void Line::cohen_sutherland(const Vector & min, const Vector & max)
	{
		db<Line>(INF) << "[" << this << "] Cohen Sutherland" << std::endl;
		
		enum Location
		{
			Left  = 0x1  /*< 0001 */
			Right = 0x2, /*< 0010 */
			Down  = 0x4, /*< 0100 */
			Up    = 0x8, /*< 1000 */
		}

		unsigned loc_a = 0, loc_b = 0;
		const Vector & pa = _window_vectors[0];
		const Vector & pb = _window_vectors[0];

		if 
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
		db<Line>(INF) << "[" << this << "] Line: " << _window_vectors[0] << " x " << _window_vectors[1] << std::endl;
		db<Line>(INF) << "[" << this << "] Clipping area: " << min << " x " << max << std::endl;

		if (Traits<Line>::clipping_method == Traits<Line>::ClippingMethod::Cohen_Sutherland)
			cohen_sutherland(min, max);
		else if (Traits<Line>::clipping_method == Traits<Line>::ClippingMethod::Liang_Barsky)
			liang_barsky(min, max);
		else if (Traits<Line>::clipping_method == Traits<Line>::ClippingMethod::Nicholl_Lee_Nicholl)
			nicholl_lee_nicholl(min, max);
	}

	std::string Line::type()
	{
		return "Line";
	}

} //! namespace model

#endif  // MODEL_LINE_HPP
