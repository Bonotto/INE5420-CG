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

/* External includes */
#include <vector>

/* Local includes */
#include "../config/traits.hpp"

namespace model
{

/*================================================================================*/
/*                                   Definitions                                  */
/*================================================================================*/

/*--------------------------------------------------------------------------------*/
/*                                     Vector                                     */
/*--------------------------------------------------------------------------------*/

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

		Vector &operator=(const Vector &) = default;
		Vector &operator=(Vector &&) = default;

		~Vector() = default;

		double& operator[](const int position);
		const double& operator[](int position) const;

		Vector operator+(const Vector& v) const;
		Vector operator-(const Vector& v) const;

		double operator*(const Vector& v) const;
		Vector operator*(const double scalar) const;
		Vector operator*(const Matrix& M) const;

		friend Debug & operator<<(Debug & db, const Vector & v)
		{
			db << "[" << v._coordinates[0];

			for (auto i = 1; i < Vector::dimension; ++i)
				db << ", " << v._coordinates[i];

			db << "]";

			return db;
		}

	private:
		std::vector<double> _coordinates;
	};

/*--------------------------------------------------------------------------------*/
/*                                    Matrix                                      */
/*--------------------------------------------------------------------------------*/

	class Matrix
	{
	public:
		using MatrixLine = Vector;

		const static int dimension = Vector::dimension;

	public:

		Matrix(const MatrixLine& l0 = {1, 0, 0, 0},
			   const MatrixLine& l1 = {0, 1, 0, 0},
			   const MatrixLine& l2 = {0, 0, 1, 0},
			   const MatrixLine& l3 = {0, 0, 0, 1}) :
			_vectors{l0, l1, l2, l3}
		{
		}

		Matrix(MatrixLine&& l0,
			   MatrixLine&& l1,
			   MatrixLine&& l2,
			   MatrixLine&& l3) :
			_vectors{l0, l1, l2, l3}
		{
		}

		~Matrix() = default;

		Matrix column_oriented() const;

		MatrixLine& operator[](const int position);
		const MatrixLine& operator[](const int position) const;

		Matrix operator*(const Matrix& M) const;
		bool operator==(const Matrix& M) const;

		friend Debug & operator<<(Debug & db, const Matrix & M)
		{
			for (auto i = 0; i < Vector::dimension; ++i)
				db << M._vectors[i] << std::endl;

			return db;
		}

	private:
		std::vector<MatrixLine> _vectors;
	};

/*--------------------------------------------------------------------------------*/
/*                                calculation                                 */
/*--------------------------------------------------------------------------------*/

	namespace calculation
	{
		double euclidean_distance(const Vector& v1, const Vector& v2);
	} //! namespace calculation

/*--------------------------------------------------------------------------------*/
/*                                transformation                                 */
/*--------------------------------------------------------------------------------*/

	namespace transformation
	{
		Matrix translation(const Vector& step);

		Matrix scaling(const double scalar, const Vector& mass_center);

		Matrix rotation(const double radians, const Vector& mass_center);

		Matrix viewport_transformation(
			const Vector& vp_min,
			const Vector& vp_max,
			const Vector& win_min,
			const Vector& win_max
		);
	} //! namespace transformation

/*================================================================================*/
/*                                 Implementaions                                 */
/*================================================================================*/

/*--------------------------------------------------------------------------------*/
/*                                    Vector                                      */
/*--------------------------------------------------------------------------------*/

	double& Vector::operator[](const int position)
	{
		return _coordinates[position];
	}

	const double& Vector::operator[](const int position) const
	{
		return _coordinates.at(position);
	}

	Vector Vector::operator+(const Vector& v) const
	{
		if (dimension == 4)
			return Vector(
				_coordinates[0] + v[0],
				_coordinates[1] + v[1],
				_coordinates[2] + v[2]
		);

		return Vector(
			_coordinates[0] + v[0],
			_coordinates[1] + v[1]
		);
	}
	
	Vector Vector::operator-(const Vector& v) const
	{
		return (*this) + (v * -1);
	}

	double Vector::operator*(const Vector& v) const
	{
		return _coordinates[0] * v[0]
			 + _coordinates[1] * v[1]
			 + _coordinates[2] * v[2]
			 + _coordinates[3] * v[3];
	}

	Vector Vector::operator*(const double scalar) const
	{
		Vector v(0, 0);

		for (int i = 0; i < dimension-1; ++i)
			v[i] = scalar * _coordinates.at(i);

		return v;
	}

	//! Alow scalar * Vector operation
	static Vector operator*(const double scalar, const Vector v)
	{
		return v * scalar;
	}

	Vector Vector::operator*(const Matrix& M) const
	{
		Vector v(0, 0, 0, 1);

		for (int j = 0; j < dimension; ++j)
			for (int i = 0; i < dimension; ++i)
				v[j] += _coordinates.at(i) * M[i][j];

		return v;
	}

/*--------------------------------------------------------------------------------*/
/*                                    Matrix                                      */
/*--------------------------------------------------------------------------------*/

	Matrix Matrix::column_oriented() const
	{
		return Matrix(
			{_vectors[0][0], _vectors[1][0], _vectors[2][0], _vectors[3][0]},
			{_vectors[0][1], _vectors[1][1], _vectors[2][1], _vectors[3][1]},
			{_vectors[0][2], _vectors[1][2], _vectors[2][2], _vectors[3][2]},
			{_vectors[0][3], _vectors[1][3], _vectors[2][3], _vectors[3][3]}
		);
	}

	Matrix::MatrixLine& Matrix::operator[](const int position)
	{
		return _vectors[position];
	}

	const Matrix::MatrixLine& Matrix::operator[](const int position) const
	{
		return _vectors.at(position);
	}

	Matrix Matrix::operator*(const Matrix& M) const
	{
		Matrix R( //! Result
			{0, 0, 0, 0},
			{0, 0, 0, 0},
			{0, 0, 0, 0},
			{0, 0, 0, 0}
		);

		for (int h = 0; h < dimension; ++h)
			for (int j = 0; j < dimension; ++j)
				for (int i = 0; i < dimension; ++i)
					R[h][j] += _vectors[h][i] * M[i][j];

		return R;
	}

	bool Matrix::operator==(const Matrix& M) const
	{
		for (int i = 0; i < dimension; ++i)
			for (int j = 0; j < dimension; ++j)
				if (M[i][j] != _vectors[i][j])
					return false;

		return true;
	}

/*--------------------------------------------------------------------------------*/
/*                                  calculation                                   */
/*--------------------------------------------------------------------------------*/

	double calculation::euclidean_distance(const Vector& v0, const Vector& v1)
	{
		double sum = 0;
		for (int i = 0; i < Vector::dimension; ++i)
			sum += std::pow((v0[i] - v1[i]), 2);

		return std::sqrt(sum);
	}

/*--------------------------------------------------------------------------------*/
/*                                transformation                                  */
/*--------------------------------------------------------------------------------*/

	Matrix transformation::translation(const Vector& step)
	{
		Vector l0(      1,       0, 0, 0);
		Vector l1(      0,       1, 0, 0);
		Vector l2(step[0], step[1], 1, 0);
		Vector l3(      0,       0, 0, 1);

		if (Vector::dimension == 4) {
			l2 = Vector(      0,       0,       1, 0);
			l3 = Vector(step[0], step[1], step[2], 1);
		}

		return Matrix(l0, l1, l2, l3);
	}

	Matrix transformation::scaling(const double scalar, const Vector& mass_center)
	{
		double scalar_z = Vector::dimension == 3 ? 1 : scalar;

		auto to_origin = translation(-1 * mass_center);
		auto go_back   = translation(mass_center);

		Matrix scalling(
			{scalar,      0,        0, 0},
			{     0, scalar,        0, 0},
			{     0,      0, scalar_z, 0},
			{     0,      0,        0, 1}
		);

		return (to_origin * scalling) * go_back;
	}

	Matrix transformation::rotation(const double radians, const Vector& mass_center)
	{
		auto to_origin = translation(-1 * mass_center);
		auto go_back   = translation(mass_center);

		Matrix rotating(
			{ std::cos(radians), std::sin(radians), 0, 0},
			{-std::sin(radians), std::cos(radians), 0, 0},
			{               0,               0, 1, 0},
			{               0,               0, 0, 1}
		);

		return (to_origin * rotating) * go_back;
	}

	Matrix transformation::viewport_transformation(
		const Vector& vp_min,
		const Vector& vp_max,
		const Vector& win_min,
		const Vector& win_max
	)
	{
		double x_vp_div_win = (vp_max[0] - vp_min[0]) / (win_max[0] - win_min[0]);
		double ix = -(win_min[0] * x_vp_div_win);
		double dx = x_vp_div_win;

		double y_vp_div_win = (vp_max[1] - vp_min[1]) / (win_max[1] - win_min[1]);
		double iy = vp_max[1] - vp_min[1] + win_min[1] * y_vp_div_win;
		double dy = (-y_vp_div_win);

		Vector l0(dx,  0, 0, 0);
		Vector l1( 0, dy, 0, 0);
		Vector l2(ix, iy, 1, 0);
		Vector l3( 0,  0, 0, 1);

		if (Vector::dimension == 4)
		{
			l2 = Vector( 0,  0, 1, 0);
			l3 = Vector(ix, iy, 0, 1);
		}

		return Matrix(l0, l1, l2, l3);
	}

} //! namespace model

#endif  // MODEL_GEOMETRY_HPP
