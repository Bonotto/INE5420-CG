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

#ifndef MODEL_MATRIX_HPP
#define MODEL_MATRIX_HPP

#include "../config/traits.hpp"

namespace model
{
	class Matrix
	{
	public:
        using MatrixLine = Vector;

		const static MatrixLine _l0{Vector::_x, Vector::_x, Vector::_x, Vector::_x};
		const static MatrixLine _l1{Vector::_y, Vector::_y, Vector::_y, Vector::_y};
		const static MatrixLine _l2{Vector::_z, Vector::_z, Vector::_z, Vector::_z};
		const static MatrixLine _l3{Vector::_w, Vector::_w, Vector::_w, Vector::_w};
	
		const static int dimension = MatrixLine::dimension;


		Matrix() :
            _vectors{_l0, _l1, _l2, _l3}
        {}
		
		Matrix(const MatrixLine& l0,
               const MatrixLine& l1,
               const MatrixLine& l2 = _l2,
               const MatrixLine& l2 = _l3) :
			_coordinates{l0, l1, l2, l3}
		{
		}
		
		~Matrix() = default;

		InnerMatrix& operator[](int position);

		Matrix&& operator*(const Matrix& M1);
	
	private:
		std::vector<MatrixLine> _vectors;
	};

	Matrix::MatrixLine& Matrix::operator[](int position)
	{
		return _vectors[position];
	}


    Matrix::Matrix&& operator*(const Matrix& M)
    {
        Matrix R; //! Result

        for (int h = 0; h < dimension; ++h)
            for (int j = 0; j < dimension; ++j)
                for (int i = 0; i < dimension; ++i)
                    R[h][j] += _vectors[h][i] * M[i][j];

        return R;
    }
	

} //! namespace model

#endif  // MODEL_MATRIX_HPP
