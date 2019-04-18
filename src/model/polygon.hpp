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

#ifndef MODEL_POLYGON_HPP
#define MODEL_POLYGON_HPP

/* External includes */

/* Local includes */
#include "../config/traits.hpp"
#include "shape.hpp"

namespace model
{

/*================================================================================*/
/*                                   Definitions                                  */
/*================================================================================*/

	class Polygon : public Shape
	{
	public:
		Polygon(std::string name, const std::initializer_list<Vector>& world_vs) :
			Shape(name, world_vs)
		{}

		Polygon(std::string name, const std::vector<Vector>& world_vs) :
			Shape(name, world_vs)
		{}

		~Polygon() = default;


		virtual void clipping(const Vector & min, const Vector & max);
		virtual std::string type();

	private:
		void sutherland_hodgeman(double x1, double y1, double x2, double y2);
	};

/*================================================================================*/
/*                                 Implementaions                                 */
/*================================================================================*/

	void Polygon::sutherland_hodgeman(double x1, double y1, double x2, double y2)
	{
		std::vector<Vector> new_vectors;

		auto x_intersect = [=](double x3, double y3, double x4, double y4)
		{
			double num = (x1*y2 - y1*x2) * (x3-x4) - (x1-x2) * (x3*y4 - y3*x4);
			double den = (x1-x2) * (y3-y4) - (y1-y2) * (x3-x4);

			return num / den;
		};

		auto y_intersect = [=](double x3, double y3, double x4, double y4)
		{
			double num = (x1*y2 - y1*x2) * (y3-y4) - (y1-y2) * (x3*y4 - y3*x4);
			double den = (x1-x2) * (y3-y4) - (y1-y2) * (x3-x4);

			return num / den;
		};

		for (size_t i = 0, k = 1; i < _window_vectors.size(); i++, k = (k + 1) % _window_vectors.size())
		{
			double ix = _window_vectors[i][0], iy = _window_vectors[i][1];
			double kx = _window_vectors[k][0], ky = _window_vectors[k][1];

			double i_pos = (x2-x1) * (iy-y1) - (y2-y1) * (ix-x1);
			double k_pos = (x2-x1) * (ky-y1) - (y2-y1) * (kx-x1);

			if (i_pos < 0  && k_pos < 0) 
				new_vectors.emplace_back(kx, ky);

			else if (i_pos >= 0  && k_pos < 0)
			{
				new_vectors.emplace_back(
					x_intersect(ix, iy, kx, ky),
					y_intersect(ix, iy, kx, ky)
				);

				new_vectors.emplace_back(kx, ky);
			}

			else if (i_pos < 0  && k_pos >= 0) 
			{
				new_vectors.emplace_back(
					x_intersect(ix, iy, kx, ky),
					y_intersect(ix, iy, kx, ky)
				);
			}
		}

		_window_vectors = new_vectors;
	}

	void Polygon::clipping(const Vector & min, const Vector & max)
	{
		double vertices[4][2] = {
			{min[0], min[1]},
			{min[0], max[1]},
			{max[0], max[1]},
			{max[0], min[1]}
		};

		for (int i = 0, k = 1; i < 4; i++, k = (k + 1) % 4) 
			sutherland_hodgeman(vertices[i][0], vertices[i][1], vertices[k][0], vertices[k][1]);
	}

	std::string Polygon::type()
	{
		return "Polygon";
	}

} //! namespace model

#endif  // MODEL_POLYGON_HPP
