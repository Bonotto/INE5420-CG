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

#ifndef MODEL_VECTOR_HPP
#define MODEL_VECTOR_HPP

#include <vector>
#include "../config/traits.hpp"
#include "matrix.hpp"

namespace model
{
	
	class Vector
	{
	public:
		const static int _x = Traits<Vector>::x;
		const static int _y = Traits<Vector>::y;
		const static int _z = Traits<Vector>::z;
		const static int _w = Traits<Vector>::w;
	
		const static int dimension = Traits<Vector>::dimension;

		Vector() :
			_coordinates{_x, _y, _z, _w}
		{
		}

		Vector(const Vector& v) :
			_coordinates{v._coordinates}
		{
		}

		Vector(Vector&& v) :
			_coordinates{v._coordinates}
		{
		}
		
		Vector(double x, double y, double z = _z, double w = _w) :
			_coordinates{x, y, z, w}
		{
		}
		
		~Vector() = default;

		double& operator[](int position);

		Vector&& operator*(const Matrix& M);
	
	private:
		std::vector<double> _coordinates;
	};

	double& Vector::operator[](int position)
	{
		return _coordinates[position];
	}

	Vector&& operator*(const Matrix& M)
	{
		Vector v;

		for (int j = 0; j < dimension; ++j)
			for (int i = 0; i < dimension; ++i)
				v[i] += _coordinates[i] * M[i][j];
		
		return v;
	}

} //! namespace model

#endif  // MODEL_VECTOR_HPP
