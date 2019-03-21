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

#ifndef MODEL_GEOMETRY_HPP
#define MODEL_GEOMETRY_HPP

#include <vector>
#include "../config/traits.hpp"

namespace model
{
	
/*================================================================================*/
/*                                   Vector                                       */
/*================================================================================*/

	class Vector
	{
	public:
		static const int x = Traits<Vector>::x;
		static const int y = Traits<Vector>::y;
		static const int z = Traits<Vector>::z;
		static const int w = Traits<Vector>::w;
	
		const static int dimension = Traits<Vector>::dimension;

		Vector() :
			_coordinates{x, y, z, w}
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
		
		Vector(double x0, double y0, double z0 = z, double w0 = w) :
			_coordinates{x0, y0, z0, w0}
		{
		}
		
		~Vector() = default;

		double& operator[](int position);

		Vector&& operator*(Matrix& M);
	
	private:
		std::vector<double> _coordinates;
	};

/*================================================================================*/
/*                                   Matrix                                       */
/*================================================================================*/

	class Matrix
	{
	public:
        using MatrixLine = Vector;
		const static int dimension = Traits<Vector>::dimension;

	public:

		Matrix(const MatrixLine& l0 = {1, 0, 0, 0},
               const MatrixLine& l1 = {0, 1, 0, 0},
               const MatrixLine& l2 = {0, 0, 1, 0},
               const MatrixLine& l3 = {0, 0, 0, 1}) :
			_vectors{l0, l1, l2, l3}
		{
		}
		
		~Matrix() = default;

		MatrixLine& operator[](int position);

		Matrix&& operator*(Matrix& M);
	
	private:
		std::vector<MatrixLine> _vectors;
	};

/*================================================================================*/
/*                                   Vector                                       */
/*================================================================================*/

	double& Vector::operator[](int position)
	{
		return _coordinates[position];
	}

	Vector&& Vector::operator*(Matrix& M)
	{
		Vector v;

		for (int j = 0; j < dimension; ++j)
			for (int i = 0; i < dimension; ++i)
				v[i] += _coordinates[i] * M[i][j];
		
		return std::move(v);
	}

/*================================================================================*/
/*                                   Matrix                                       */
/*================================================================================*/

	Matrix::MatrixLine& Matrix::operator[](int position)
	{
		return _vectors[position];
	}

    Matrix&& Matrix::operator*(Matrix& M)
    {
        Matrix R; //! Result

        for (int h = 0; h < dimension; ++h)
            for (int j = 0; j < dimension; ++j)
                for (int i = 0; i < dimension; ++i)
                    R[h][j] += _vectors[h][i] * M[i][j];

        return std::move(R);
    }

} //! namespace model

#endif  // MODEL_GEOMETRY_HPP
