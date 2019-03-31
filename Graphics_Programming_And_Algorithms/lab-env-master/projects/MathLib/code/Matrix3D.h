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
	float matris[4][4];					//en array med arrays (4x4 matris som fungerar som 3x3)
public:
	float get(int rad, int kolumn);
	void set(int rad, int kolumn, float varde);
	Vector3D get_position();
	void set_position(const Vector3D & v);
    void operator=(Matrix3D rhs);
	Matrix3D operator*(const Matrix3D rhs);
    Vector3D operator*(Vector3D rhs);
    Matrix3D operator*(float factor);
	static Matrix3D rotate_x(float vinkel);
	static Matrix3D rotate_y(float vinkel);
	static Matrix3D rotate_z(float vinkel);
	static Matrix3D scale(float, float, float);
	static Matrix3D rotate_vec(Vector3D vector, float vinkel);
    Matrix3D inverse(void);
    Matrix3D transpose(Matrix3D mat);
    Matrix3D transpose();
    static Matrix3D translate(Matrix3D mat, Vector3D vec);
    static Matrix3D QuaternionToMatrix(Vector3D vec);
    static float* MatrixConvertion(Matrix3D mat);
    static void ViewProjMat(Vector3D eye, Vector3D target, Vector3D up, Matrix3D& proj, Matrix3D& view);

	Matrix3D(float x, float y, float z, float w, float a, float b, float c, float d, float e, float m, float l, float n, float q, float v, float s, float t);
	Matrix3D(float x, float y, float z, float w, float a, float b, float c, float d, float e);
	Matrix3D();
	~Matrix3D(void);
	float* GetMatrix() { return (float*)matris; }
	float* operator[](int i)
	{
		return matris[i];
	}
};

class Vector3D
{
	friend class Matrix3D;
private:
	float vektor[4];				//en 3D array (med ett extra element s� eg 4D vektor)
public:
	Vector3D operator+(const Vector3D &rhs);
	Vector3D operator-(const Vector3D &rhs);
	Vector3D operator*(const float k);
    Vector3D scalarProduct(float k);
    Vector3D operator*(Vector3D rhs);
    Vector3D plusOperatorQuat(Vector3D);
    Vector3D minusOperatorQuat(Vector3D);
	friend Vector3D operator*(const Matrix3D lhs, Vector3D rhs);
	float& operator[](unsigned int i);
	float dotproduct(Vector3D &en_vektor);
    float QuatDotproduct(Vector3D &en_vektor);
	Vector3D cross_product(Vector3D &en_vektor);
	Vector3D normal();
    Vector3D QuaternionNormal();
	float langd();
    float QuatLangd();
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
    Vector3D(float x, float y, float z, float w);
	Vector3D(const Vector3D &rhs);
    static Vector3D lerp(Vector3D vec1, Vector3D vec2, float dt);
    Vector3D Slerp(Vector3D start, Vector3D end, float percent);
	Vector3D();
	~Vector3D(void);
};

/*! Konstruktor som s�tter v�rden p� hela matrisen*/
inline Matrix3D::Matrix3D(float x, float y, float z, float w, float a, float b, float c, float d, float e, float m, float l, float n, float q, float v, float s, float t)
{
	matris[0][0] = x;
	matris[0][1] = y;
	matris[0][2] = z;
	matris[0][3] = w;
	matris[1][0] = a;
	matris[1][1] = b;
	matris[1][2] = c;
	matris[1][3] = d;
	matris[2][0] = e;
	matris[2][1] = m;
	matris[2][2] = l;
	matris[2][3] = n;
	matris[3][0] = q;
	matris[3][1] = v;
	matris[3][2] = s;
	matris[3][3] = t;
}

/*! Konstruktor som s�tter v�rden p� alla utom sista raden och sista kolumnen*/
inline Matrix3D::Matrix3D(float x, float y, float z, float w, float a, float b, float c, float d, float e)
{
	matris[0][0] = x;
	matris[0][1] = y;
	matris[0][2] = z;
	matris[0][3] = 0;
	matris[1][0] = w;
	matris[1][1] = a;
	matris[1][2] = b;
	matris[1][3] = 0;
	matris[2][0] = c;
	matris[2][1] = d;
	matris[2][2] = e;
	matris[2][3] = 0;
	matris[3][0] = 0;
	matris[3][1] = 0;
	matris[3][2] = 0;
	matris[3][3] = 1.0f;
}

/*! Konstruktor som ger oss en identitetsmatris*/
inline Matrix3D::Matrix3D()			//om en matris skapas utan att v�rden anges, skapas en identitetsmatris
{
	matris[0][0] = 1.0f;
	matris[0][1] = 0;
	matris[0][2] = 0;
	matris[0][3] = 0;
	matris[1][0] = 0;
	matris[1][1] = 1.0f;
	matris[1][2] = 0;
	matris[1][3] = 0;
	matris[2][0] = 0;
	matris[2][1] = 0;
	matris[2][2] = 1.0f;
	matris[2][3] = 0;
	matris[3][0] = 0;
	matris[3][1] = 0;
	matris[3][2] = 0;
	matris[3][3] = 1.0f;
}

inline Matrix3D::~Matrix3D(void)
{
}

/*!Ger oss v�rdet fr�n en viss position i matrisen, arg �r rad, kolumn*/
inline float Matrix3D::get(int rad, int kolumn)
{
	return matris[rad][kolumn];
}

inline void Matrix3D::set(int rad, int kolumn, float varde)
{
	matris[rad][kolumn] = varde;
}


inline Vector3D Matrix3D::get_position()
{
	Vector3D v(matris[0][3], matris[1][3], matris[2][3]);
	return v;
}

inline void Matrix3D::set_position(const Vector3D & v)
{
	for (int i = 0;i < 3;i++)
		matris[i][3] = v.vektor[i];
}

/*! Operator som hanterar multiplikation mellan matriser*/
inline Matrix3D Matrix3D::operator*(Matrix3D rhs)
{
	float x, y, z, w, a, b, c, d, e, m, l, n, q, v, s, t;
	x = (matris[0][0] * rhs.get(0, 0)) + (matris[0][1] * rhs.get(1, 0)) + (matris[0][2] * rhs.get(2, 0)) + (matris[0][3] * rhs.get(3,0));
	y = (matris[0][0] * rhs.get(0, 1)) + (matris[0][1] * rhs.get(1, 1)) + (matris[0][2] * rhs.get(2, 1)) + (matris[0][3] * rhs.get(3,1));
	z = (matris[0][0] * rhs.get(0, 2)) + (matris[0][1] * rhs.get(1, 2)) + (matris[0][2] * rhs.get(2, 2)) + (matris[0][3] * rhs.get(3,2));
	m = (matris[0][0] * rhs.get(0, 3)) + (matris[0][1] * rhs.get(1, 3)) + (matris[0][2] * rhs.get(2, 3)) + (matris[0][3] * rhs.get(3,3));

	w = (matris[1][0] * rhs.get(0, 0)) + (matris[1][1] * rhs.get(1, 0)) + (matris[1][2] * rhs.get(2, 0)) + (matris[1][3] * rhs.get(3,0));
	a = (matris[1][0] * rhs.get(0, 1)) + (matris[1][1] * rhs.get(1, 1)) + (matris[1][2] * rhs.get(2, 1)) + (matris[1][3] * rhs.get(3,1));
	b = (matris[1][0] * rhs.get(0, 2)) + (matris[1][1] * rhs.get(1, 2)) + (matris[1][2] * rhs.get(2, 2)) + (matris[1][3] * rhs.get(3,2));
	l = (matris[1][0] * rhs.get(0, 3)) + (matris[1][1] * rhs.get(1, 3)) + (matris[1][2] * rhs.get(2, 3)) + (matris[1][3] * rhs.get(3,3));

	c = (matris[2][0] * rhs.get(0, 0)) + (matris[2][1] * rhs.get(1, 0)) + (matris[2][2] * rhs.get(2, 0)) + (matris[2][3] * rhs.get(3,0));
	d = (matris[2][0] * rhs.get(0, 1)) + (matris[2][1] * rhs.get(1, 1)) + (matris[2][2] * rhs.get(2, 1)) + (matris[2][3] * rhs.get(3,1));
	e = (matris[2][0] * rhs.get(0, 2)) + (matris[2][1] * rhs.get(1, 2)) + (matris[2][2] * rhs.get(2, 2)) + (matris[2][3] * rhs.get(3,2));
	n = (matris[2][0] * rhs.get(0, 3)) + (matris[2][1] * rhs.get(1, 3)) + (matris[2][2] * rhs.get(2, 3)) + (matris[2][3] * rhs.get(3,3));

	q = (matris[3][0] * rhs.get(0, 0)) + (matris[3][1] * rhs.get(1, 0)) + (matris[3][2] * rhs.get(2, 0)) + (matris[3][3] * rhs.get(3,0));
	v = (matris[3][0] * rhs.get(0, 1)) + (matris[3][1] * rhs.get(1, 1)) + (matris[3][2] * rhs.get(2, 1)) + (matris[3][3] * rhs.get(3,1));
	s = (matris[3][0] * rhs.get(0, 2)) + (matris[3][1] * rhs.get(1, 2)) + (matris[3][2] * rhs.get(2, 2)) + (matris[3][3] * rhs.get(3,2));
	t = (matris[3][0] * rhs.get(0, 3)) + (matris[3][1] * rhs.get(1, 3)) + (matris[3][2] * rhs.get(2, 3)) + (matris[3][3] * rhs.get(3,3));

	Matrix3D matris_prod(x, y, z, m, w, a, b, l, c, d, e, n, q, v, s, t);
	return matris_prod;
}

inline Matrix3D Matrix3D::operator*(float scale)
{
    float a = matris[0][0] * scale;
    float b = matris[0][1] * scale;
    float c = matris[0][2] * scale;
    float d = matris[0][3] * scale;

    float a2 = matris[1][0] * scale;
    float b2 = matris[1][1] * scale;
    float c2 = matris[1][2] * scale;
    float d2 = matris[1][3] * scale;

    float a3 = matris[2][0] * scale;
    float b3 = matris[2][1] * scale;
    float c3 = matris[2][2] * scale;
    float d3 = matris[2][3] * scale;

    float a4 = matris[3][0] * scale;
    float b4 = matris[3][1] * scale;
    float c4 = matris[3][2] * scale;
    float d4 = matris[3][3] * scale;

    Matrix3D scaleMatrix(a, b, c, d, a2, b2, c2, d2, a3, b3, c3, d3, a4, b4, c4, d4);
    return scaleMatrix;
}

inline void Matrix3D::operator=(Matrix3D rhs)
{
    matris[0][0] = rhs[0][0];
    matris[0][1] = rhs[0][1];
    matris[0][2] = rhs[0][2];
    matris[0][3] = rhs[0][3];

    matris[1][0] = rhs[1][0];
    matris[1][1] = rhs[1][1];
    matris[1][2] = rhs[1][2];
    matris[1][3] = rhs[1][3];

    matris[2][0] = rhs[2][0];
    matris[2][1] = rhs[2][1];
    matris[2][2] = rhs[2][2];
    matris[2][3] = rhs[2][3];

    matris[3][0] = rhs[3][0];
    matris[3][1] = rhs[3][1];
    matris[3][2] = rhs[3][2];
    matris[3][3] = rhs[3][3];
}

inline void Matrix3D::ViewProjMat(Vector3D eye, Vector3D target, Vector3D up, Matrix3D& proj, Matrix3D& view)
{
    //vektorn "eye" = vart kameran tittar fr�n
    //vektorn "target" = det kameran tittar p�
    //vektorn "up" f�rklarar f�r kameran vad som �r upp och vad som �r ner
    //matriserna som skickas in är de som ska ändras

    Matrix3D matProj;
    float fov = 90;
    float S = 1 / (tan((fov / 2) * (M_PI / 180)));
    float f = 100.0f;													//farclip plane (allt som �r bakom detta ska INTE renderas)
    float n = 0.1f;														//nearclip plane (allt som �r framf�r detta ska INTE renderas)
    matProj[0][0] = S;
    matProj[1][1] = S;
    matProj[2][2] = -(f / (f - n));
    matProj[2][3] = -((f*n) / (f - n));
    matProj[3][2] = -1;
    matProj[3][3] = 0;


    Vector3D zaxis = (eye - target).normal();							// The "forward" vector.
    Vector3D xaxis = (up.cross_product(zaxis)).normal();				// The "right" vector.
    Vector3D yaxis = zaxis.cross_product(xaxis);						// The "up" vector.


    Matrix3D viewMatrix;												// skapar view-matrix
    viewMatrix[0][0] = xaxis[0];
    viewMatrix[0][1] = yaxis[0];
    viewMatrix[0][2] = zaxis[0];
    viewMatrix[0][3] = 0;

    viewMatrix[1][0] = xaxis[1];
    viewMatrix[1][1] = yaxis[1];
    viewMatrix[1][2] = zaxis[1];
    viewMatrix[1][3] = 0;

    viewMatrix[2][0] = xaxis[2];
    viewMatrix[2][1] = yaxis[2];
    viewMatrix[2][2] = zaxis[2];
    viewMatrix[2][3] = 0;

    viewMatrix[3][0] = -(eye.dotproduct(xaxis));
    viewMatrix[3][1] = -(eye.dotproduct(yaxis));
    viewMatrix[3][2] = -(eye.dotproduct(zaxis));
    viewMatrix[3][3] = 1;
    viewMatrix = viewMatrix.transpose();

    view = viewMatrix;													//sparar mina matriser i mina matriserna jag skickar in
    proj = matProj;


    //Vector3D vec(0, 0, -10);
    //vec = matProj * vec;
    //vec.divideByW();
    //std::cout << vec.getZ();
}

/*! Roterar en matris med x antal grader runt x-axeln. Arg. �r vinkeln i grader*/
inline Matrix3D Matrix3D::rotate_x(float vinkel)
{
	float rad = vinkel * (M_PI / 180.0f);												//omvandlar vinkeln i grader till radianer. M_PI �r pi fast importerat 
	float cv = cosf(rad);
	float sv = sinf(rad);
	Matrix3D rotationsmatris(1.0f, 0, 0, 0, cv, -sv, 0, sv, cv);					//rotationsmatrisen vi sen multiplicerar med skapas
	return rotationsmatris;
}

/*! Roterar en matris med x antal grader runt y-axeln. Arg. �r vinkeln i grader*/
inline Matrix3D Matrix3D::rotate_y(float vinkel)
{
	float rad = vinkel * (M_PI / 180.0f);												//omvandlar vinkeln i grader till radianer. M_PI �r pi fast importerat 
	float cv = cosf(rad);
	float sv = sinf(rad);
	Matrix3D rotationsmatris(cv,0,sv,0,1.0f,0,-sv,0,cv);								//rotationsmatrisen vi sen multiplicerar med skapas
	return rotationsmatris;
}

/*! Roterar en matris med x antal grader runt z-axeln. Arg. �r vinkeln i grader*/
inline Matrix3D Matrix3D::rotate_z(float vinkel)
{
	float rad = vinkel * (M_PI / 180.0f);												//omvandlar vinkeln i grader till radianer. M_PI �r pi fast importerat 
	float cv = cosf(rad);
	float sv = sinf(rad);
	Matrix3D rotationsmatris(cv,-sv,0,sv,cv,0,0,0,1.0f);								//rotationsmatrisen vi sen multiplicerar med skapas
	return rotationsmatris;
}

/*! Roterar en matris med x antal grader runt en vektor. Arg. �r vinkeln i grader och vektorn som matrisen ska roteras runt*/
inline Matrix3D Matrix3D::rotate_vec(Vector3D vector, float vinkel)
{
	float rad = vinkel * (M_PI / 180.0f);												//omvandlar vinkeln i grader till radianer. M_PI �r pi fast importerat 
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
	Matrix3D rotationsmatris(a, b, c, d, e, f, g, h, i);
	return rotationsmatris;
}

inline Matrix3D Matrix3D::scale(float x, float y, float z)
{
	Matrix3D matr;
	matr[0][0] = x;
	matr[1][1] = y;
	matr[2][2] = z;

	return matr;
}

/*! Returnerar matrisens invers*/
inline Matrix3D Matrix3D::inverse(void)
{
	//determinanten r�knas ut
	float l1 = (matris[0][0] * matris[1][1] * matris[2][2] * matris[3][3]) + (matris[0][0] * matris[1][2] * matris[2][3] * matris[3][1]) + (matris[0][0] * matris[1][3] * matris[2][1] * matris[3][2]);
	float l2 = (matris[0][1] * matris[1][0] * matris[2][3] * matris[3][2]) + (matris[0][1] * matris[1][2] * matris[2][0] * matris[3][3]) + (matris[0][1] * matris[1][3] * matris[2][2] * matris[3][0]);
	float l3 = (matris[0][2] * matris[1][0] * matris[2][1] * matris[3][3]) + (matris[0][2] * matris[1][1] * matris[2][3] * matris[3][0]) + (matris[0][2] * matris[1][3] * matris[2][0] * matris[3][1]);
	float l4 = (matris[0][3] * matris[1][0] * matris[2][2] * matris[3][1]) + (matris[0][3] * matris[1][1] * matris[2][0] * matris[3][2]) + (matris[0][3] * matris[1][2] * matris[2][1] * matris[3][0]);
	float l5 = (matris[0][0] * matris[1][1] * matris[2][3] * matris[3][2]) + (matris[0][0] * matris[1][2] * matris[2][1] * matris[3][3]) + (matris[0][0] * matris[1][3] * matris[2][2] * matris[3][1]);
	float l6 = (matris[0][1] * matris[1][0] * matris[2][2] * matris[3][3]) + (matris[0][1] * matris[1][2] * matris[2][3] * matris[3][0]) + (matris[0][1] * matris[1][3] * matris[2][0] * matris[3][2]);
	float l7 = (matris[0][2] * matris[1][0] * matris[2][3] * matris[3][1]) + (matris[0][2] * matris[1][1] * matris[2][0] * matris[3][3]) + (matris[0][2] * matris[1][3] * matris[2][1] * matris[3][0]);
	float l8 = (matris[0][3] * matris[1][0] * matris[2][1] * matris[3][2]) + (matris[0][3] * matris[1][1] * matris[2][2] * matris[3][0]) + (matris[0][3] * matris[1][2] * matris[2][0] * matris[3][1]);

	float det = l1 + l2 + l3 + l4 - l5 - l6 - l7 - l8;
	if (det == 0)
	{
		std::cout << "The matrix does not have a corresponding inverse" << std::endl;
	}
	else
	{
		//om determinanten inte blir 0 finns en invers. Den r�knas ut h�r.
		float p00 = (matris[1][1] * matris[2][2] * matris[3][3]) + (matris[1][2] * matris[2][3] * matris[3][1]) + (matris[1][3] * matris[2][1] * matris[3][2]) - (matris[1][1] * matris[2][3] * matris[3][2]) - (matris[1][2] * matris[2][1] * matris[3][3]) - (matris[1][3] * matris[2][2] * matris[3][1]);
		float p01 = (matris[0][1] * matris[2][3] * matris[3][2]) + (matris[0][2] * matris[2][1] * matris[3][3]) + (matris[0][3] * matris[2][2] * matris[3][1]) - (matris[0][1] * matris[2][2] * matris[3][3]) - (matris[0][2] * matris[2][3] * matris[3][1]) - (matris[0][3] * matris[2][1] * matris[3][2]);
		float p02 = (matris[0][1] * matris[1][2] * matris[3][3]) + (matris[0][2] * matris[1][3] * matris[3][1]) + (matris[0][3] * matris[1][1] * matris[3][2]) - (matris[0][1] * matris[1][3] * matris[3][2]) - (matris[0][2] * matris[1][1] * matris[3][3]) - (matris[0][3] * matris[1][2] * matris[3][1]);
		float p03 = (matris[0][1] * matris[1][3] * matris[2][2]) + (matris[0][2] * matris[1][1] * matris[2][3]) + (matris[0][3] * matris[1][2] * matris[2][1]) - (matris[0][1] * matris[1][2] * matris[2][3]) - (matris[0][2] * matris[1][3] * matris[2][1]) - (matris[0][3] * matris[1][1] * matris[2][2]);
		
		float p10 = (matris[1][0] * matris[2][3] * matris[3][2]) + (matris[1][2] * matris[2][1] * matris[3][3]) + (matris[1][3] * matris[2][2] * matris[3][0]) - (matris[1][0] * matris[2][2] * matris[3][3]) - (matris[1][2] * matris[2][3] * matris[3][0]) - (matris[1][3] * matris[2][0] * matris[3][2]);
		float p11 = (matris[0][0] * matris[2][2] * matris[3][3]) + (matris[0][2] * matris[2][3] * matris[3][0]) + (matris[0][3] * matris[2][0] * matris[3][2]) - (matris[0][0] * matris[2][3] * matris[3][2]) - (matris[0][2] * matris[2][0] * matris[3][3]) - (matris[0][3] * matris[2][2] * matris[3][0]);
		float p12 = (matris[0][0] * matris[1][3] * matris[3][2]) + (matris[0][2] * matris[1][0] * matris[3][3]) + (matris[0][3] * matris[1][2] * matris[3][0]) - (matris[0][0] * matris[1][2] * matris[3][3]) - (matris[0][2] * matris[1][3] * matris[3][0]) - (matris[0][3] * matris[1][0] * matris[3][2]);
		float p13 = (matris[0][0] * matris[1][2] * matris[2][3]) + (matris[0][2] * matris[1][3] * matris[2][0]) + (matris[0][3] * matris[1][0] * matris[2][2]) - (matris[0][0] * matris[1][3] * matris[2][2]) - (matris[0][2] * matris[1][0] * matris[2][3]) - (matris[0][3] * matris[1][2] * matris[2][0]);

		float p20 = (matris[1][0] * matris[2][1] * matris[3][3]) + (matris[1][1] * matris[2][3] * matris[3][0]) + (matris[1][3] * matris[2][0] * matris[3][1]) - (matris[1][0] * matris[2][3] * matris[3][1]) - (matris[1][1] * matris[2][0] * matris[3][3]) - (matris[1][3] * matris[2][1] * matris[3][0]);
		float p21 = (matris[0][0] * matris[2][3] * matris[3][1]) + (matris[0][1] * matris[2][0] * matris[3][3]) + (matris[0][3] * matris[2][1] * matris[3][0]) - (matris[0][0] * matris[2][1] * matris[3][3]) - (matris[0][1] * matris[2][3] * matris[3][0]) - (matris[0][3] * matris[2][0] * matris[3][1]);
		float p22 = (matris[0][0] * matris[1][1] * matris[3][3]) + (matris[0][1] * matris[1][3] * matris[3][0]) + (matris[0][3] * matris[1][0] * matris[3][1]) - (matris[0][0] * matris[1][3] * matris[3][1]) - (matris[0][1] * matris[1][0] * matris[3][3]) - (matris[0][3] * matris[1][1] * matris[3][0]);
		float p23 = (matris[0][0] * matris[1][3] * matris[2][1]) + (matris[0][1] * matris[1][0] * matris[2][3]) + (matris[0][3] * matris[1][1] * matris[2][0]) - (matris[0][0] * matris[1][1] * matris[2][3]) - (matris[0][1] * matris[1][3] * matris[2][0]) - (matris[0][3] * matris[1][0] * matris[2][1]);

		float p30 = (matris[1][0] * matris[2][2] * matris[3][1]) + (matris[1][1] * matris[2][0] * matris[3][2]) + (matris[1][2] * matris[2][1] * matris[3][0]) - (matris[1][0] * matris[2][1] * matris[3][2]) - (matris[1][1] * matris[2][2] * matris[3][0]) - (matris[1][2] * matris[2][0] * matris[3][1]);
		float p31 = (matris[0][0] * matris[2][1] * matris[3][2]) + (matris[0][1] * matris[2][2] * matris[3][0]) + (matris[0][2] * matris[2][0] * matris[3][1]) - (matris[0][0] * matris[2][2] * matris[3][1]) - (matris[0][1] * matris[2][0] * matris[3][2]) - (matris[0][2] * matris[2][1] * matris[3][0]);
		float p32 = (matris[0][0] * matris[1][2] * matris[3][1]) + (matris[0][1] * matris[1][0] * matris[3][2]) + (matris[0][2] * matris[1][1] * matris[3][0]) - (matris[0][0] * matris[1][1] * matris[3][2]) - (matris[0][1] * matris[1][2] * matris[3][0]) - (matris[0][2] * matris[1][0] * matris[3][1]);
		float p33 = (matris[0][0] * matris[1][1] * matris[2][2]) + (matris[0][1] * matris[1][2] * matris[2][0]) + (matris[0][2] * matris[1][0] * matris[2][1]) - (matris[0][0] * matris[1][2] * matris[2][1]) - (matris[0][1] * matris[1][0] * matris[2][2]) - (matris[0][2] * matris[1][1] * matris[2][0]);

		float ny_det = 1 / det;
		Matrix3D inverse_mat(p00, p01, p02, p03, p10, p11, p12, p13, p20, p21, p22, p23, p30, p31, p32, p33);
		
		//matrisen loopas igenom och varje v�rde multipliceras med determinanten
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				inverse_mat.matris[i][j] = inverse_mat.matris[i][j] * ny_det;
			}
		}

		return inverse_mat;
	}
}

/*! Returnerar matrisens transponat*/
inline Matrix3D Matrix3D::transpose(void)
{
	Matrix3D temp;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			temp.matris[i][j] = matris[j][i];
		}
	}
	return temp;
}

/*! Returnerar matrisens transponat*/
inline Matrix3D Matrix3D::transpose(Matrix3D mat)
{
    Matrix3D temp = mat;
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            temp.matris[i][j] = matris[j][i];
        }
    }
    return temp;
}

inline Matrix3D Matrix3D::QuaternionToMatrix(Vector3D vec)
{
    Matrix3D matrix;
    float x, y, z, w;
    x = vec[0];
    y = vec[1];
    z = vec[2];
    w = vec[3];

    matrix[0][0] = (1 - (2*(pow(y, 2))) - (2*(pow(z,2))));
    matrix[0][1] = ((2*x*y) + (2*w*z));
    matrix[0][2] = ((2*x*z) - (2*w*y));

    matrix[1][0] = ((2*x*y) -(2*w*z));
    matrix[1][1] = (1 -(2*(pow(x,2))) - (2*(pow(z,2))));
    matrix[1][2] = ((2*y*z) + (2*w*x));

    matrix[2][0] = ((2*x*z) + (2*w*y));
    matrix[2][1] = ((2*y*z) - (2*w*x));
    matrix[2][2] = (1 - (2*(pow(x, 2))) - (2*(pow(y,2))));


    return matrix;
}

inline Matrix3D Matrix3D::translate(Matrix3D mat, Vector3D vec)
{
    mat[0][3] = vec.getX();
    mat[1][3] = vec.getY();
    mat[2][3] = vec.getZ();
    mat[3][3] = vec.getW();
    return mat;
}

//convert the matrix from [4][4] to [16] and return the array
inline float* Matrix3D::MatrixConvertion(Matrix3D mat)
{
    float* floatArr = new float[16];

    floatArr[0] = mat[0][0];
    floatArr[1] = mat[0][1];
    floatArr[2] = mat[0][2];
    floatArr[3] = mat[0][3];

    floatArr[4] = mat[1][0];
    floatArr[5] = mat[1][1];
    floatArr[6] = mat[1][2];
    floatArr[7] = mat[1][3];

    floatArr[8] = mat[2][0];
    floatArr[9] = mat[2][1];
    floatArr[10] = mat[2][2];
    floatArr[11] = mat[2][3];

    floatArr[12] = mat[3][0];
    floatArr[13] = mat[3][1];
    floatArr[14] = mat[3][2];
    floatArr[15] = mat[3][3];

    return floatArr;
}

/*! Operator som hanterar multiplikation mellan vektor och matris*/
inline Vector3D Matrix3D::operator*(Vector3D rhs)
{
    float x = (matris[0][0] * rhs.getX()) + (matris[0][1] * rhs.getY()) + (matris[0][2] * rhs.getZ()) + (matris[0][3] * rhs.getW());
    float y = (matris[1][0] * rhs.getX()) + (matris[1][1] * rhs.getY()) + (matris[1][2] * rhs.getZ()) + (matris[1][3] * rhs.getW());
    float z = (matris[2][0] * rhs.getX()) + (matris[2][1] * rhs.getY()) + (matris[2][2] * rhs.getZ()) + (matris[2][3] * rhs.getW());
    float w = (matris[3][0] * rhs.getX()) + (matris[3][1] * rhs.getY()) + (matris[3][2] * rhs.getZ()) + (matris[3][3] * rhs.getW());
    Vector3D prodVec(x, y, z, w);
    return prodVec;
}

/*! Konstruktor som s�tter default-v�rden*/
inline Vector3D::Vector3D()
{
	vektor[0] = 0;
	vektor[1] = 0;
	vektor[2] = 0;
	vektor[3] = 1.0f;					//sista elementet ska alltid vara 1.
}

/*! Konstruktor som s�tter v�rden beroende p� vad som matas in. Endast x,y,z �r arg.*/
inline Vector3D::Vector3D(float x, float y, float z)
{
	vektor[0] = x;
	vektor[1] = y;
	vektor[2] = z;
	vektor[3] = 1.0f;					//sista elementet ska alltid vara 1.
}

inline Vector3D::Vector3D(float x, float y, float z, float w)
{
    vektor[0] = x;
    vektor[1] = y;
    vektor[2] = z;
    vektor[3] = w;					//sista elementet ska alltid vara 1.
}

/*! Konstruktor som s�tter en vektor till en annan vektors v�rden*/
inline Vector3D::Vector3D(const Vector3D &rhs)
{
	this->vektor[0] = rhs.vektor[0];
	this->vektor[1] = rhs.vektor[1];
	this->vektor[2] = rhs.vektor[2];
	this->vektor[3] = rhs.vektor[3];					//sista elementet ska alltid vara 1.;
}


inline Vector3D::~Vector3D(void)
{
}

/*! Returnerar x-v�rdet fr�n vald vektor*/
inline float Vector3D::getX() const
{
	float x = vektor[0];
	return x;
}

/*! Returnerar y-v�rdet fr�n vald vektor*/
inline float Vector3D::getY() const
{
	float y = vektor[1];
	return y;
}

/*! Returnerar z-v�rdet fr�n vald vektor*/
inline float Vector3D::getZ() const
{
	float z = vektor[2];
	return z;
}

/*! Returnerar w-v�rdet fr�n vald vektor*/
inline float Vector3D::getW() const
{
	float w = vektor[3];
	return w;
}

inline void Vector3D::divideByW()
{
	vektor[0] = vektor[0] / vektor[3];
	vektor[1] = vektor[1] / vektor[3];
	vektor[2] = vektor[2] / vektor[3];
	vektor[3] = vektor[3] / vektor[3];
}

/*! S�tter x-v�rdet till vald vektor*/
inline void Vector3D::setX(float x)
{
	vektor[0] = x;
}

/*! S�tter y-v�rdet till vald vektor*/
inline void Vector3D::setY(float y)
{
	vektor[1] = y;
}

/*! S�tter z-v�rdet till vald vektor*/
inline void Vector3D::setZ(float z)
{
	vektor[2] = z;
}

inline float* Vector3D::getVec()
{
	return vektor;
}

/*! Operator som hanterar addition mellan vektorer*/
inline Vector3D Vector3D::operator+(const Vector3D &rhs)
{
	float x, y, z;
	x = this->vektor[0] + rhs.getX();
	y = this->vektor[1] + rhs.getY();
	z = this->vektor[2] + rhs.getZ();
	Vector3D summa(x, y, z);
	return summa;
}

inline Vector3D Vector3D::plusOperatorQuat(Vector3D rhs)
{
    float x, y, z, w;
    x = this->vektor[0] + rhs.getX();
    y = this->vektor[1] + rhs.getY();
    z = this->vektor[2] + rhs.getZ();
    w = this->vektor[3] + rhs.getW();
    Vector3D summa(x, y, z, w);
    return summa;
}
inline Vector3D Vector3D::minusOperatorQuat(Vector3D rhs)
{
    float x, y, z, w;
    x = this->vektor[0] - rhs.getX();
    y = this->vektor[1] - rhs.getY();
    z = this->vektor[2] - rhs.getZ();
    w = this->vektor[3] - rhs.getW();
    Vector3D differens(x, y, z, w);
    return differens;
}

inline Vector3D Vector3D::operator*(Vector3D en_vektor)
{
    float nytt_x, nytt_y, nytt_z;
    nytt_x = (getY() * en_vektor.getZ()) - (getZ() * en_vektor.getY());
    nytt_y = (getZ() * en_vektor.getX()) - (getX() * en_vektor.getZ());
    nytt_z = (getX() * en_vektor.getY()) - (getY() * en_vektor.getX());
    Vector3D cross_p(nytt_x, nytt_y, nytt_z);
    return cross_p;
}

/*! Operator som hanterar subtraktion mellan vektorer*/
inline Vector3D Vector3D::operator-(const Vector3D &rhs)
{
	float x, y, z;
	x = this->vektor[0] - rhs.getX();
	y = this->vektor[1] - rhs.getY();
	z = this->vektor[2] - rhs.getZ();
	Vector3D differens(x, y, z);
	return differens;
}

/*! Operator som hanterar multiplikation mellan vektorer*/
inline Vector3D Vector3D::operator*(const float k)
{
	float x, y, z;
	x = this->vektor[0] * k;
	y = this->vektor[1] * k;
	z = this->vektor[2] * k;

	Vector3D ny_vektor(x, y, z);
	return ny_vektor;
	//vi g�r inget med element w eftersom det alltid ska vara 1.
}

inline Vector3D Vector3D::scalarProduct(float k)
{
    float x, y, z, w;
    x = this->vektor[0] * k;
    y = this->vektor[1] * k;
    z = this->vektor[2] * k;
    w = this->vektor[3] * k;

    Vector3D ny_vektor(x, y, z, w);
    return ny_vektor;

}

inline float& Vector3D::operator[](unsigned int i)
{
	return vektor[i];
}

/*! Funktion som r�knar ut l�ngden p� vektorn*/
inline float Vector3D::langd()
{
	float tot_langd, x, y, z;
	x = getX();
	y = getY();
	z = getZ();
	tot_langd = sqrt(x*x + y*y + z*z);
	return tot_langd;
}

/*! Funktion som returnerar den normaliserade vektorn*/
inline Vector3D Vector3D::normal()
{
	float x, y, z, nytt_x, nytt_y, nytt_z;
	nytt_x = getX() / langd();
	nytt_y = getY() / langd();
	nytt_z = getZ() / langd();
	Vector3D norm_vektor(nytt_x, nytt_y, nytt_z);
	return norm_vektor;
}

inline float Vector3D::QuatLangd()
{
    float tot_langd, x, y, z, w;
    x = getX();
    y = getY();
    z = getZ();
    w = getW();
    tot_langd = sqrt(x*x + y*y + z*z + w*w);
    return tot_langd;
}

inline Vector3D Vector3D::QuaternionNormal()
{
    float nytt_x, nytt_y, nytt_z, nytt_w;
    nytt_x = getX() / QuatLangd();
    nytt_y = getY() / QuatLangd();
    nytt_z = getZ() / QuatLangd();
    nytt_w = getW() / QuatLangd();
    Vector3D norm_vektor(nytt_x, nytt_y, nytt_z, nytt_w);
    return norm_vektor;
}

/*! Funktion som r�knar ut skal�rprodukten mellan tv� vektorer*/
inline float Vector3D::dotproduct(Vector3D &en_vektor)
{
	float dot_prod;
	float xsum = getX() * en_vektor.getX();
	float ysum = getY() * en_vektor.getY();
	float zsum = getZ() * en_vektor.getZ();
	dot_prod = xsum + ysum + zsum;
	return dot_prod;
}

inline float Vector3D::QuatDotproduct(Vector3D &en_vektor)
{
    float dot_prod;
    float xsum = getX() * en_vektor.getX();
    float ysum = getY() * en_vektor.getY();
    float zsum = getZ() * en_vektor.getZ();
    float wsum = getW() * en_vektor.getW();
    dot_prod = xsum + ysum + zsum + wsum;
    return dot_prod;
}

/*! Funktion som r�knar ut kryssprodukten mellan tv� vektorer*/
inline Vector3D Vector3D::cross_product(Vector3D &en_vektor)
{
	float nytt_x, nytt_y, nytt_z;
	nytt_x = (getY() * en_vektor.getZ()) - (getZ() * en_vektor.getY());
	nytt_y = (getX() * en_vektor.getZ()) - (getZ() * en_vektor.getZ());
	nytt_z = (getX() * en_vektor.getY()) - (getY() * en_vektor.getX());
	Vector3D cross_p(nytt_x, nytt_y, nytt_z);
	return cross_p;
}

/*! Operator som hanterar multiplikation mellan vektor och matris*/
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

inline Vector3D Vector3D::lerp(Vector3D start, Vector3D end, float dt)
{
   return (start + (end-start)*dt);
}

inline Vector3D Vector3D::Slerp(Vector3D start, Vector3D end, float percent)
{
    start.QuaternionNormal();
    end.QuaternionNormal();

    // Compute the cosine of the angle between the two vectors.
    double dot = start.QuatDotproduct(end);

    const double DOT_THRESHOLD = 0.9999999999999999999999999999999995;
    if (fabs(dot) > DOT_THRESHOLD)
    {
        // If the inputs are too close for comfort, linearly interpolate
        // and normalize the result.
        Vector3D result = end.minusOperatorQuat(start);
        result = result.scalarProduct(percent);
        result = start.plusOperatorQuat(result);
        result.QuaternionNormal();
        return result;

    }

    // If the dot product is negative, the quaternions
    // have opposite handed-ness and slerp won't take
    // the shorter path. Fix by reversing one quaternion.
    if (dot < 0.0f)
    {
        end = end.scalarProduct(-1.0f);
        dot = -dot;
    }

    if(dot > 1)
    {
        dot = 1;
    }
    else if(dot< -1)
    {
        dot = -1;
    }
    double theta_0 = acos(dot);  // theta_0 = angle between input vectors
    double theta = theta_0*percent;    // theta = angle between v0 and result

    Vector3D interpolated = end.minusOperatorQuat((start));
    interpolated = interpolated.scalarProduct(dot);
    interpolated.QuaternionNormal();

    Vector3D temp = end.scalarProduct(cos(theta));
    Vector3D temp2 = start.scalarProduct(sin(theta));

    temp = temp.plusOperatorQuat(temp2);

    return temp;

}