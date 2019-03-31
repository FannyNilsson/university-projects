#pragma once
#include <iostream>
#define _USE_MATH_DEFINES
#include <math.h>
#include <stdexcept>
class Vector3D;
class Matrix3D
{
friend class Vector3D;
private:
	float matrix[4][4];
public:
	float get(int row, int column);
	void set(int row, int column, float value);
	Vector3D get_position();
	void set_position(const Vector3D & v);
	Matrix3D operator*(const Matrix3D rhs);
	static Matrix3D rotate_x(float angle);
	static Matrix3D rotate_y(float angle);
	static Matrix3D rotate_z(float angle);
	static Matrix3D scale(float, float, float);
	static Matrix3D rotate_vec(Vector3D vector, float angle);
	Matrix3D inverse(void);
	Matrix3D transpose(void);
	Matrix3D(float x, float y, float z, float w, float a, float b, float c, float d, float e, float m, float l, float n, float q, float v, float s, float t);
	Matrix3D(float x, float y, float z, float w, float a, float b, float c, float d, float e);
	Matrix3D();
	~Matrix3D(void);
	float* GetMatrix() { return (float*)matrix; }
	float* operator[](int i)
	{
		return matrix[i];
	}
};

class Vector3D
{
	friend class Matrix3D;
private:
	float vector[4];
public:
	Vector3D operator+(const Vector3D &rhs);
	Vector3D operator-(const Vector3D &rhs);
	Vector3D operator*(const float k);
	bool operator==(Vector3D &rhs);
	friend Vector3D operator*(const Matrix3D lhs, Vector3D rhs);
	float& operator[](unsigned int i);
	float dotproduct(Vector3D &vector);
	Vector3D cross_product(Vector3D &vector);
	Vector3D normal();
	float length();
	float getX() const;
	float getY() const;
	float getZ() const;
	float getW() const;
	void divideByW();
	float* getVec();
	void setX(float x);
	void setY(float y);
	void setZ(float z);
	Vector3D(float x, float y, float z);
	Vector3D(const Vector3D &rhs);
	Vector3D();
	~Vector3D(void);
};


inline Matrix3D::Matrix3D(float x, float y, float z, float w, float a, float b, float c, float d, float e, float m, float l, float n, float q, float v, float s, float t)
{
	matrix[0][0] = x;
	matrix[0][1] = y;
	matrix[0][2] = z;
	matrix[0][3] = w;
	matrix[1][0] = a;
	matrix[1][1] = b;
	matrix[1][2] = c;
	matrix[1][3] = d;
	matrix[2][0] = e;
	matrix[2][1] = m;
	matrix[2][2] = l;
	matrix[2][3] = n;
	matrix[3][0] = q;
	matrix[3][1] = v;
	matrix[3][2] = s;
	matrix[3][3] = t;
}

inline Matrix3D::Matrix3D(float x, float y, float z, float w, float a, float b, float c, float d, float e)
{
	matrix[0][0] = x;
	matrix[0][1] = y;
	matrix[0][2] = z;
	matrix[0][3] = 0;
	matrix[1][0] = w;
	matrix[1][1] = a;
	matrix[1][2] = b;
	matrix[1][3] = 0;
	matrix[2][0] = c;
	matrix[2][1] = d;
	matrix[2][2] = e;
	matrix[2][3] = 0;
	matrix[3][0] = 0;
	matrix[3][1] = 0;
	matrix[3][2] = 0;
	matrix[3][3] = 1.0f;
}

inline Matrix3D::Matrix3D()
{
	matrix[0][0] = 1.0f;
	matrix[0][1] = 0;
	matrix[0][2] = 0;
	matrix[0][3] = 0;
	matrix[1][0] = 0;
	matrix[1][1] = 1.0f;
	matrix[1][2] = 0;
	matrix[1][3] = 0;
	matrix[2][0] = 0;
	matrix[2][1] = 0;
	matrix[2][2] = 1.0f;
	matrix[2][3] = 0;
	matrix[3][0] = 0;
	matrix[3][1] = 0;
	matrix[3][2] = 0;
	matrix[3][3] = 1.0f;
}

inline Matrix3D::~Matrix3D(void)
{
}

inline float Matrix3D::get(int row, int column)
{
	return matrix[row][column];
}

inline void Matrix3D::set(int row, int column, float value)
{
	matrix[row][column] = value;
}


inline Vector3D Matrix3D::get_position()
{
	Vector3D v(matrix[0][3], matrix[1][3], matrix[2][3]);
	return v;
}

inline void Matrix3D::set_position(const Vector3D & v)
{
	for (int i = 0;i < 3;i++)
		matrix[i][3] = v.vector[i];
}

inline Matrix3D Matrix3D::operator*(Matrix3D rhs)
{
	float x, y, z, w, a, b, c, d, e, m, l, n, q, v, s, t;
	x = (matrix[0][0] * rhs.get(0, 0)) + (matrix[0][1] * rhs.get(1, 0)) + (matrix[0][2] * rhs.get(2, 0)) + (matrix[0][3] * rhs.get(3,0));
	y = (matrix[0][0] * rhs.get(0, 1)) + (matrix[0][1] * rhs.get(1, 1)) + (matrix[0][2] * rhs.get(2, 1)) + (matrix[0][3] * rhs.get(3,1));
	z = (matrix[0][0] * rhs.get(0, 2)) + (matrix[0][1] * rhs.get(1, 2)) + (matrix[0][2] * rhs.get(2, 2)) + (matrix[0][3] * rhs.get(3,2));
	m = (matrix[0][0] * rhs.get(0, 3)) + (matrix[0][1] * rhs.get(1, 3)) + (matrix[0][2] * rhs.get(2, 3)) + (matrix[0][3] * rhs.get(3,3));

	w = (matrix[1][0] * rhs.get(0, 0)) + (matrix[1][1] * rhs.get(1, 0)) + (matrix[1][2] * rhs.get(2, 0)) + (matrix[1][3] * rhs.get(3,0));
	a = (matrix[1][0] * rhs.get(0, 1)) + (matrix[1][1] * rhs.get(1, 1)) + (matrix[1][2] * rhs.get(2, 1)) + (matrix[1][3] * rhs.get(3,1));
	b = (matrix[1][0] * rhs.get(0, 2)) + (matrix[1][1] * rhs.get(1, 2)) + (matrix[1][2] * rhs.get(2, 2)) + (matrix[1][3] * rhs.get(3,2));
	l = (matrix[1][0] * rhs.get(0, 3)) + (matrix[1][1] * rhs.get(1, 3)) + (matrix[1][2] * rhs.get(2, 3)) + (matrix[1][3] * rhs.get(3,3));

	c = (matrix[2][0] * rhs.get(0, 0)) + (matrix[2][1] * rhs.get(1, 0)) + (matrix[2][2] * rhs.get(2, 0)) + (matrix[2][3] * rhs.get(3,0));
	d = (matrix[2][0] * rhs.get(0, 1)) + (matrix[2][1] * rhs.get(1, 1)) + (matrix[2][2] * rhs.get(2, 1)) + (matrix[2][3] * rhs.get(3,1));
	e = (matrix[2][0] * rhs.get(0, 2)) + (matrix[2][1] * rhs.get(1, 2)) + (matrix[2][2] * rhs.get(2, 2)) + (matrix[2][3] * rhs.get(3,2));
	n = (matrix[2][0] * rhs.get(0, 3)) + (matrix[2][1] * rhs.get(1, 3)) + (matrix[2][2] * rhs.get(2, 3)) + (matrix[2][3] * rhs.get(3,3));

	q = (matrix[3][0] * rhs.get(0, 0)) + (matrix[3][1] * rhs.get(1, 0)) + (matrix[3][2] * rhs.get(2, 0)) + (matrix[3][3] * rhs.get(3,0));
	v = (matrix[3][0] * rhs.get(0, 1)) + (matrix[3][1] * rhs.get(1, 1)) + (matrix[3][2] * rhs.get(2, 1)) + (matrix[3][3] * rhs.get(3,1));
	s = (matrix[3][0] * rhs.get(0, 2)) + (matrix[3][1] * rhs.get(1, 2)) + (matrix[3][2] * rhs.get(2, 2)) + (matrix[3][3] * rhs.get(3,2));
	t = (matrix[3][0] * rhs.get(0, 3)) + (matrix[3][1] * rhs.get(1, 3)) + (matrix[3][2] * rhs.get(2, 3)) + (matrix[3][3] * rhs.get(3,3));

	Matrix3D matrix_prod(x, y, z, m, w, a, b, l, c, d, e, n, q, v, s, t);
	return matrix_prod;
}

inline Matrix3D Matrix3D::rotate_x(float angle)
{
	float rad = angle * (M_PI / 180.0f);												
	float cv = cosf(rad);
	float sv = sinf(rad);
	Matrix3D rotation_matrix(1.0f, 0, 0, 0, cv, -sv, 0, sv, cv);					
	return rotation_matrix;
}

inline Matrix3D Matrix3D::rotate_y(float angle)
{
	float rad = angle * (M_PI / 180.0f);												
	float cv = cosf(rad);
	float sv = sinf(rad);
	Matrix3D rotation_matrix(cv,0,sv,0,1.0f,0,-sv,0,cv);								
	return rotation_matrix;
}

inline Matrix3D Matrix3D::rotate_z(float angle)
{
	float rad = angle * (M_PI / 180.0f);												
	float cv = cosf(rad);
	float sv = sinf(rad);
	Matrix3D rotation_matrix(cv,-sv,0,sv,cv,0,0,0,1.0f);								
	return rotation_matrix;
}

inline Matrix3D Matrix3D::rotate_vec(Vector3D vector, float angle)
{
	float rad = angle * (M_PI / 180.0f);												
	float cv = cosf(rad);
	float sv = sinf(rad);
	float a, b, c, d, e, f, g, h, i;
	float x = vector.getX();
	float y = vector.getY();
	float z = vector.getZ();
	a = ((x*x) + (1 - (x*x))*cv);
	b = (x*y*(1 - cv)) - (x*sv);
	c = ((x*z)*(1 - cv)) + (y*sv);
	d = (x*y*(1 - cv)) + (z*sv);
	e = (y*y) + ((1 - (y*y))* cv);
	f = ((x*z)*(1 - cv)) - (x*sv);
	g = ((x*z) *(1 - cv)) - (y*sv);
	h = ((y*z)*(1 - cv)) + (x*sv);
	i = (z*z) + ((1 - (z*z))*cv);
	Matrix3D rotation_matrix(a, b, c, d, e, f, g, h, i);
	return rotation_matrix;
}

inline Matrix3D Matrix3D::scale(float x, float y, float z)
{
	Matrix3D matr;
	matr[0][0] = x;
	matr[1][1] = y;
	matr[2][2] = z;

	return matr;
}

inline Matrix3D Matrix3D::inverse(void)
{
	//the determinant is calculated
	float l1 = (matrix[0][0] * matrix[1][1] * matrix[2][2] * matrix[3][3]) + (matrix[0][0] * matrix[1][2] * matrix[2][3] * matrix[3][1]) + (matrix[0][0] * matrix[1][3] * matrix[2][1] * matrix[3][2]);
	float l2 = (matrix[0][1] * matrix[1][0] * matrix[2][3] * matrix[3][2]) + (matrix[0][1] * matrix[1][2] * matrix[2][0] * matrix[3][3]) + (matrix[0][1] * matrix[1][3] * matrix[2][2] * matrix[3][0]);
	float l3 = (matrix[0][2] * matrix[1][0] * matrix[2][1] * matrix[3][3]) + (matrix[0][2] * matrix[1][1] * matrix[2][3] * matrix[3][0]) + (matrix[0][2] * matrix[1][3] * matrix[2][0] * matrix[3][1]);
	float l4 = (matrix[0][3] * matrix[1][0] * matrix[2][2] * matrix[3][1]) + (matrix[0][3] * matrix[1][1] * matrix[2][0] * matrix[3][2]) + (matrix[0][3] * matrix[1][2] * matrix[2][1] * matrix[3][0]);
	float l5 = (matrix[0][0] * matrix[1][1] * matrix[2][3] * matrix[3][2]) + (matrix[0][0] * matrix[1][2] * matrix[2][1] * matrix[3][3]) + (matrix[0][0] * matrix[1][3] * matrix[2][2] * matrix[3][1]);
	float l6 = (matrix[0][1] * matrix[1][0] * matrix[2][2] * matrix[3][3]) + (matrix[0][1] * matrix[1][2] * matrix[2][3] * matrix[3][0]) + (matrix[0][1] * matrix[1][3] * matrix[2][0] * matrix[3][2]);
	float l7 = (matrix[0][2] * matrix[1][0] * matrix[2][3] * matrix[3][1]) + (matrix[0][2] * matrix[1][1] * matrix[2][0] * matrix[3][3]) + (matrix[0][2] * matrix[1][3] * matrix[2][1] * matrix[3][0]);
	float l8 = (matrix[0][3] * matrix[1][0] * matrix[2][1] * matrix[3][2]) + (matrix[0][3] * matrix[1][1] * matrix[2][2] * matrix[3][0]) + (matrix[0][3] * matrix[1][2] * matrix[2][0] * matrix[3][1]);

	float det = l1 + l2 + l3 + l4 - l5 - l6 - l7 - l8;
	if (det == 0)
	{
		std::cout << "The matrix does not have a corresponding inverse" << std::endl;
	}
	else
	{
		//if the determinant is 0, no inverse matrix exists
		float p00 = (matrix[1][1] * matrix[2][2] * matrix[3][3]) + (matrix[1][2] * matrix[2][3] * matrix[3][1]) + (matrix[1][3] * matrix[2][1] * matrix[3][2]) - (matrix[1][1] * matrix[2][3] * matrix[3][2]) - (matrix[1][2] * matrix[2][1] * matrix[3][3]) - (matrix[1][3] * matrix[2][2] * matrix[3][1]);
		float p01 = (matrix[0][1] * matrix[2][3] * matrix[3][2]) + (matrix[0][2] * matrix[2][1] * matrix[3][3]) + (matrix[0][3] * matrix[2][2] * matrix[3][1]) - (matrix[0][1] * matrix[2][2] * matrix[3][3]) - (matrix[0][2] * matrix[2][3] * matrix[3][1]) - (matrix[0][3] * matrix[2][1] * matrix[3][2]);
		float p02 = (matrix[0][1] * matrix[1][2] * matrix[3][3]) + (matrix[0][2] * matrix[1][3] * matrix[3][1]) + (matrix[0][3] * matrix[1][1] * matrix[3][2]) - (matrix[0][1] * matrix[1][3] * matrix[3][2]) - (matrix[0][2] * matrix[1][1] * matrix[3][3]) - (matrix[0][3] * matrix[1][2] * matrix[3][1]);
		float p03 = (matrix[0][1] * matrix[1][3] * matrix[2][2]) + (matrix[0][2] * matrix[1][1] * matrix[2][3]) + (matrix[0][3] * matrix[1][2] * matrix[2][1]) - (matrix[0][1] * matrix[1][2] * matrix[2][3]) - (matrix[0][2] * matrix[1][3] * matrix[2][1]) - (matrix[0][3] * matrix[1][1] * matrix[2][2]);
		
		float p10 = (matrix[1][0] * matrix[2][3] * matrix[3][2]) + (matrix[1][2] * matrix[2][1] * matrix[3][3]) + (matrix[1][3] * matrix[2][2] * matrix[3][0]) - (matrix[1][0] * matrix[2][2] * matrix[3][3]) - (matrix[1][2] * matrix[2][3] * matrix[3][0]) - (matrix[1][3] * matrix[2][0] * matrix[3][2]);
		float p11 = (matrix[0][0] * matrix[2][2] * matrix[3][3]) + (matrix[0][2] * matrix[2][3] * matrix[3][0]) + (matrix[0][3] * matrix[2][0] * matrix[3][2]) - (matrix[0][0] * matrix[2][3] * matrix[3][2]) - (matrix[0][2] * matrix[2][0] * matrix[3][3]) - (matrix[0][3] * matrix[2][2] * matrix[3][0]);
		float p12 = (matrix[0][0] * matrix[1][3] * matrix[3][2]) + (matrix[0][2] * matrix[1][0] * matrix[3][3]) + (matrix[0][3] * matrix[1][2] * matrix[3][0]) - (matrix[0][0] * matrix[1][2] * matrix[3][3]) - (matrix[0][2] * matrix[1][3] * matrix[3][0]) - (matrix[0][3] * matrix[1][0] * matrix[3][2]);
		float p13 = (matrix[0][0] * matrix[1][2] * matrix[2][3]) + (matrix[0][2] * matrix[1][3] * matrix[2][0]) + (matrix[0][3] * matrix[1][0] * matrix[2][2]) - (matrix[0][0] * matrix[1][3] * matrix[2][2]) - (matrix[0][2] * matrix[1][0] * matrix[2][3]) - (matrix[0][3] * matrix[1][2] * matrix[2][0]);

		float p20 = (matrix[1][0] * matrix[2][1] * matrix[3][3]) + (matrix[1][1] * matrix[2][3] * matrix[3][0]) + (matrix[1][3] * matrix[2][0] * matrix[3][1]) - (matrix[1][0] * matrix[2][3] * matrix[3][1]) - (matrix[1][1] * matrix[2][0] * matrix[3][3]) - (matrix[1][3] * matrix[2][1] * matrix[3][0]);
		float p21 = (matrix[0][0] * matrix[2][3] * matrix[3][1]) + (matrix[0][1] * matrix[2][0] * matrix[3][3]) + (matrix[0][3] * matrix[2][1] * matrix[3][0]) - (matrix[0][0] * matrix[2][1] * matrix[3][3]) - (matrix[0][1] * matrix[2][3] * matrix[3][0]) - (matrix[0][3] * matrix[2][0] * matrix[3][1]);
		float p22 = (matrix[0][0] * matrix[1][1] * matrix[3][3]) + (matrix[0][1] * matrix[1][3] * matrix[3][0]) + (matrix[0][3] * matrix[1][0] * matrix[3][1]) - (matrix[0][0] * matrix[1][3] * matrix[3][1]) - (matrix[0][1] * matrix[1][0] * matrix[3][3]) - (matrix[0][3] * matrix[1][1] * matrix[3][0]);
		float p23 = (matrix[0][0] * matrix[1][3] * matrix[2][1]) + (matrix[0][1] * matrix[1][0] * matrix[2][3]) + (matrix[0][3] * matrix[1][1] * matrix[2][0]) - (matrix[0][0] * matrix[1][1] * matrix[2][3]) - (matrix[0][1] * matrix[1][3] * matrix[2][0]) - (matrix[0][3] * matrix[1][0] * matrix[2][1]);

		float p30 = (matrix[1][0] * matrix[2][2] * matrix[3][1]) + (matrix[1][1] * matrix[2][0] * matrix[3][2]) + (matrix[1][2] * matrix[2][1] * matrix[3][0]) - (matrix[1][0] * matrix[2][1] * matrix[3][2]) - (matrix[1][1] * matrix[2][2] * matrix[3][0]) - (matrix[1][2] * matrix[2][0] * matrix[3][1]);
		float p31 = (matrix[0][0] * matrix[2][1] * matrix[3][2]) + (matrix[0][1] * matrix[2][2] * matrix[3][0]) + (matrix[0][2] * matrix[2][0] * matrix[3][1]) - (matrix[0][0] * matrix[2][2] * matrix[3][1]) - (matrix[0][1] * matrix[2][0] * matrix[3][2]) - (matrix[0][2] * matrix[2][1] * matrix[3][0]);
		float p32 = (matrix[0][0] * matrix[1][2] * matrix[3][1]) + (matrix[0][1] * matrix[1][0] * matrix[3][2]) + (matrix[0][2] * matrix[1][1] * matrix[3][0]) - (matrix[0][0] * matrix[1][1] * matrix[3][2]) - (matrix[0][1] * matrix[1][2] * matrix[3][0]) - (matrix[0][2] * matrix[1][0] * matrix[3][1]);
		float p33 = (matrix[0][0] * matrix[1][1] * matrix[2][2]) + (matrix[0][1] * matrix[1][2] * matrix[2][0]) + (matrix[0][2] * matrix[1][0] * matrix[2][1]) - (matrix[0][0] * matrix[1][2] * matrix[2][1]) - (matrix[0][1] * matrix[1][0] * matrix[2][2]) - (matrix[0][2] * matrix[1][1] * matrix[2][0]);

		float ny_det = 1 / det;
		Matrix3D inverse_mat(p00, p01, p02, p03, p10, p11, p12, p13, p20, p21, p22, p23, p30, p31, p32, p33);
		
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				inverse_mat.matrix[i][j] = inverse_mat.matrix[i][j] * ny_det;
			}
		}

		return inverse_mat;
	}
}

inline Matrix3D Matrix3D::transpose(void)
{
	Matrix3D temp;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			temp.matrix[i][j] = matrix[j][i];
		}
	}
	return temp;
}

inline Vector3D::Vector3D()
{
	vector[0] = 0;
	vector[1] = 0;
	vector[2] = 0;
	vector[3] = 1.0f;					
}

inline Vector3D::Vector3D(float x, float y, float z)
{
	vector[0] = x;
	vector[1] = y;
	vector[2] = z;
	vector[3] = 1.0f;					
}

inline Vector3D::Vector3D(const Vector3D &rhs)
{
	this->vector[0] = rhs.vector[0];
	this->vector[1] = rhs.vector[1];
	this->vector[2] = rhs.vector[2];
	this->vector[3] = 1.0f;					
}


inline Vector3D::~Vector3D(void)
{
}

inline float Vector3D::getX() const
{
	float x = vector[0];
	return x;
}

inline float Vector3D::getY() const
{
	float y = vector[1];
	return y;
}

inline float Vector3D::getZ() const
{
	float z = vector[2];
	return z;
}

inline float Vector3D::getW() const
{
	float w = vector[3];
	return w;
}

inline void Vector3D::divideByW()
{
	vector[0] = vector[0] / vector[3];
	vector[1] = vector[1] / vector[3];
	vector[2] = vector[2] / vector[3];
	vector[3] = vector[3] / vector[3];
}

inline void Vector3D::setX(float x)
{
	vector[0] = x;
}

inline void Vector3D::setY(float y)
{
	vector[1] = y;
}

inline void Vector3D::setZ(float z)
{
	vector[2] = z;
}

inline float* Vector3D::getVec()
{
	return vector;
}

inline Vector3D Vector3D::operator+(const Vector3D &rhs)
{
	float x, y, z;
	x = this->vector[0] + rhs.getX();
	y = this->vector[1] + rhs.getY();
	z = this->vector[2] + rhs.getZ();
	Vector3D summa(x, y, z);
	return summa;
}

inline Vector3D Vector3D::operator-(const Vector3D &rhs)
{
	float x, y, z;
	x = this->vector[0] - rhs.getX();
	y = this->vector[1] - rhs.getY();
	z = this->vector[2] - rhs.getZ();
	Vector3D differens(x, y, z);
	return differens;
}

inline Vector3D Vector3D::operator*(const float k)
{
	float x, y, z;
	x = this->vector[0] * k;
	y = this->vector[1] * k;
	z = this->vector[2] * k;
	Vector3D ny_vector(x, y, z);
	return ny_vector;
}

inline float& Vector3D::operator[](unsigned int i)
{
	return vector[i];
}

inline float Vector3D::length()
{
	float tot_length, x, y, z;
	x = getX();
	y = getY();
	z = getZ();
	tot_length = sqrt(x*x + y*y + z*z);
	return tot_length;
}

inline Vector3D Vector3D::normal()
{
	float x, y, z, temp_x, temp_y, temp_z;
	temp_x = getX() / length();
	temp_y = getY() / length();
	temp_z = getZ() / length();
	Vector3D norm_vector(temp_x, temp_y, temp_z);
	return norm_vector;
}

inline float Vector3D::dotproduct(Vector3D &vector)
{
	float dot_prod;
	float xsum = getX() * vector.getX();
	float ysum = getY() * vector.getY();
	float zsum = getZ() * vector.getZ();
	dot_prod = xsum + ysum + zsum;
	return dot_prod;
}

inline Vector3D Vector3D::cross_product(Vector3D &vector)
{
	float temp_x, temp_y, temp_z;
	temp_x = (getY() * vector.getZ()) - (getZ() * vector.getY());
	temp_y = (getX() * vector.getZ()) - (getZ() * vector.getX());
	temp_z = (getX() * vector.getY()) - (getY() * vector.getX());
	Vector3D cross_p(temp_x, temp_y, temp_z);
	return cross_p;
}

inline Vector3D operator*( Matrix3D lhs, Vector3D rhs)
{
	float x, y, z, w;
	x = (lhs.get(0,0) * rhs.getX()) + (lhs.get(0,1) * rhs.getY()) + (lhs.get(0,2) * rhs.getZ()) + (lhs.get(0,3) * rhs.getW());
	y = (lhs.get(1,0) * rhs.getX()) + (lhs.get(1,1) * rhs.getY()) + (lhs.get(1,2) * rhs.getZ()) + (lhs.get(1,3) * rhs.getW());
	z = (lhs.get(2,0) * rhs.getX()) + (lhs.get(2,1) * rhs.getY()) + (lhs.get(2,2) * rhs.getZ()) + (lhs.get(2,3) * rhs.getW());
	w = (lhs.get(3,0) * rhs.getX()) + (lhs.get(3,1) * rhs.getY()) + (lhs.get(3,2) * rhs.getZ()) + (lhs.get(3,3) * rhs.getW());

	Vector3D vec(x, y, z);
	return vec;
}

inline bool Vector3D::operator==(Vector3D &rhs)
{
    if(getX() == rhs.getX() && getY() == rhs.getY() && getZ() == rhs.getZ() && getW() == rhs.getW())
    {
        return true;
    }
    else
    {
        return false;
    }
}
