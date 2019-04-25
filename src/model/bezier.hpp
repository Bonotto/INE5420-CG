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
		
		void w_transformation(const Matrix & window_T);

		virtual std::string type();
	};

/*================================================================================*/
/*                                 Implementaions                                 */
/*================================================================================*/

	void Bezier::w_transformation(const Matrix & window_T)
	{


		std::vector<Vector> vectors;

		Vector p1 = _world_vectors[0] * window_T;
		Vector p2 = _world_vectors[1] * window_T;
		Vector p3 = _world_vectors[2] * window_T;
		Vector p4 = _world_vectors[3] * window_T;
	
		db<Vector>(TRC) << p1 << " | " << p2 << " | " << p3 << " | " << p4 << std::endl; 

		for (double t = 0; t <= 1.0; t += 0.01)
		{
			Vector p =               std::pow(1 - t, 3) * p1
			         + 3 * t       * std::pow(1 - t, 2) * p2
			         + 3 * (1 - t) * std::pow(t, 2)     * p3
                     +               std::pow(t, 3)     * p4;

            vectors.push_back(std::move(p));
		}

		_window_vectors = std::move(vectors);
	}

	std::string Bezier::type()
	{
		return "Bezier Curve";
	}


} //! namespace model

#endif  // MODEL_BEZIER_HPP
