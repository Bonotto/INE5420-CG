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

		double norm() const;
		double angle(const Vector& w) const;

		Vector projection(const Vector &w1, const Vector &w2) const;

		template<int D>
		Vector multiply(const Matrix &M) const;

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

		std::vector<double> operator*(const std::vector<double>& v) const;
		Vector operator*(const Vector& v) const;
		Matrix operator*(const Matrix& M) const;
		bool operator==(const Matrix& M) const;

		template<int D>
		Matrix multiply(const Matrix &M) const;

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

		Matrix rotation(const double radians, const Vector& mass_center, const Vector& normal);

		Matrix viewport_transformation(
			const Vector& vp_min,
			const Vector& vp_max,
			const Vector& win_min,
			const Vector& win_max
		);

		/* Anonymous namespace: This does not export the following features */
		namespace
		{
			enum Axis
			{
				X = 0x1,
				Y = 0x2,
				Z = 0x4,
				XY = 0x3,
				XZ = 0x5,
				YZ = 0x6,
				XYZ = 0x7
			};

			Matrix rotation(const double radians, const Axis ax = Axis::Z)
			{
				const double cos = std::cos(radians);
				const double sin = std::sin(radians);

				switch (ax)
				{
				case Axis::X :
					return Matrix(
						{1,    0,    0, 0},
						{0,  cos, -sin, 0},
						{0,  sin,  cos, 0},
						{0,    0,    0, 1}
					);
				
				case Axis::Y :
					return Matrix(
						{ cos, 0,  sin, 0},
						{   0, 1,    0, 0},
						{-sin, 0,  cos, 0},
						{   0, 0,    0, 1}
					);
				
				case Axis::Z :
					return Matrix(
						{ cos, -sin, 0, 0},
						{ sin,  cos, 0, 0},
						{   0,    0, 1, 0},
						{   0,    0, 0, 1}
					);
					break;
				
				default:
					return Matrix();
				}
			}
		}
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
			 + _coordinates[2] * v[2];
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
		Vector v(0, 0, 0, 0);

		for (int j = 0; j < dimension; ++j)
			for (int i = 0; i < dimension; ++i)
				v[j] += _coordinates.at(i) * M[i][j];

		return v;
	}

	double Vector::norm() const
	{
		double sum = 0;

		for (int i = 0; i < dimension-1; ++i)
			sum += std::pow(_coordinates[i], 2);

		return std::sqrt(sum);
	}

	double Vector::angle(const Vector& w) const
	{
		const Vector& v = *this;
		const double norms = v.norm() * w.norm();

		if (!norms)
			return (0);

		return std::acos(
			(v * w) / norms
		);
	}

	Vector Vector::projection(const Vector &w1, const Vector &w2) const
	{
		const Vector &y = *this;

		return (((y * w1) / (w1 * w1)) * w1) + (((y * w2) / (w2 * w2)) * w2);
	}

	template<int D>
	Vector Vector::multiply(const Matrix& M) const
	{
		Vector v(0, 0, 0, 0);

		for (int j = 0; j < D; ++j)
			for (int i = 0; i < D; ++i)
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

	std::vector<double> Matrix::operator*(const std::vector<double>& v) const
	{
		std::vector<double> R(
			{0, 0, 0, 0}
		);
		
		if (v.size() != _vectors.size())
			return R;
		
		for (size_t i = 0; i < v.size(); ++i)
			for (size_t j = 0; j < v.size(); ++j)
				R[i] += _vectors[i][j] * v[j];

		return R;
	}

	Vector Matrix::operator*(const Vector& v) const
	{
		Vector R( //! Result
			{0, 0, 0, 0}
		);

		for (int i = 0; i < dimension; ++i)
			for (int j = 0; j < dimension; ++j)
				R[i] += _vectors[i][j] * v[j];

		return R;
	}

	Matrix Matrix::operator*(const Matrix& M) const
	{
		Matrix R( //! Result
			{0.0, 0.0, 0.0, 0.0},
			{0.0, 0.0, 0.0, 0.0},
			{0.0, 0.0, 0.0, 0.0},
			{0.0, 0.0, 0.0, 0.0}
		);

		for (int i = 0; i < dimension; i++)
			for (int j = 0; j < dimension; j++)
				for (int k = 0; k < dimension; k++)
					R[i][j] += (_vectors[i][k] * M[k][j]);

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

	template<int D>
	Matrix Matrix::multiply(const Matrix& M) const
	{
		Matrix R( //! Result
			{0.0, 0.0, 0.0, 0.0},
			{0.0, 0.0, 0.0, 0.0},
			{0.0, 0.0, 0.0, 0.0},
			{0.0, 0.0, 0.0, 0.0}
		);

		for (int i = 0; i < D; i++)
			for (int j = 0; j < D; j++)
				for (int k = 0; k < D; k++)
					R[i][j] += (_vectors[i][k] * M[k][j]);

		return R;
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

	Matrix transformation::rotation(const double radians, const Vector& mass_center, const Vector& n)
	{
		if (Vector::dimension == 3)
		{
			const auto to_origin = translation(-1 * mass_center);
			const auto rotating  = rotation(radians, Axis::Z);
			const auto go_back   = translation(mass_center);

			return (to_origin * rotating) * go_back;
		}


		/* Vector::dimention = 4 */
		else
		{
			auto normal = n - mass_center;

			int hash = 0;

			hash |= (normal[0] != 0) << 0;
			hash |= (normal[1] != 0) << 1;
			hash |= (normal[2] != 0) << 2;

			const auto to_origin = translation(-1 * mass_center);
			const auto go_back   = translation(mass_center);

			switch (hash)
			{
				case X:
					return to_origin
						* rotation(radians, Axis::X)
						* go_back;

				case Y:
					return to_origin
						* rotation(radians, Axis::Y)
						* go_back;

				case Z:
					return to_origin
						* rotation(radians, Axis::Z)
						* go_back;

				case XY: {
					const double radians_z = normal.angle({1, 0, 0});

					return to_origin
						* rotation( radians_z, Axis::Z)
						* rotation( radians,   Axis::X)
						* rotation(-radians_z, Axis::Z)
						* go_back;
				}
				
				case XZ: {
					const double radians_y = normal.angle({1, 0, 0});

					return to_origin
						* rotation(-radians_y, Axis::Y)
						* rotation( radians,   Axis::X)
						* rotation( radians_y, Axis::Y)
						* go_back;
				}

				case YZ: {
					const double radians_x = normal.angle({0, 0, 1});

					return to_origin
						* rotation(-radians_x, Axis::X)
						* rotation( radians,   Axis::Z)
						* rotation( radians_x, Axis::X)
						* go_back;
				}

				case XYZ: {
					
					/* Go to XZ */
					auto proj_xz = normal.projection({1, 0, 0}, {0, 0, 1});
					double radians_z = proj_xz.angle({0, 0, 1});

					if (normal[0] < 0)
						radians_z *= -1;

					const auto do_ry   = rotation( radians_z, Axis::Y);
					const auto undo_ry = rotation(-radians_z, Axis::Y);

					auto temp_norm = normal * do_ry;

					double radians_x = temp_norm.angle({0, 0, 1});

					if (normal[1] < 0)
						radians_x *= -1;

					const auto do_rx   = rotation(-radians_x, Axis::X);
					const auto undo_rx = rotation( radians_x, Axis::X);

					return to_origin
					     * do_ry
					     * do_rx
					     * rotation(radians, Axis::Z)
					     * undo_rx
					     * undo_ry
					     * go_back;
				}

				default:
					return Matrix();
			}	
		}
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
