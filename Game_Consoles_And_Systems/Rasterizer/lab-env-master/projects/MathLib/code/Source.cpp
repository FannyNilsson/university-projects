#include "Matrix3D.h"
#include <iostream>

//test the mathlib
int main()
{
	Vector3D vector(1, 2, 3);
	float x = vector.getX();
	float z = vector.getZ();
	std::cout << x << z << std::endl;		//output 1 3

	Vector3D vector1;
	vector1.setX(2);
	vector1.setY(2);
	vector1.setZ(2);
	float x1 = vector1.getX();
	std::cout << x1 << std::endl;		//output 2

	Vector3D vectSum = vector + vector1;
	float xsum = vectSum.getX();
	float ysum = vectSum.getY();
	float zsum = vectSum.getZ();
	std::cout << xsum << ysum << zsum << std::endl;		//output 3 4 5
	
	Vector3D vektdif = vectSum - vector;
	float xdif = vektdif.getX();
	float ydif = vektdif.getY();
	float zdif = vektdif.getZ();
	std::cout << xdif << ydif << zdif << std::endl;		//output 2 2 2

	Vector3D vektmult = vector * 2;
	float xmult = vektmult.getX();
	float ymult = vektmult.getY();
	float zmult = vektmult.getZ();
	std::cout << xmult << ymult << zmult << std::endl;		//output 2 4 6

	float vekt_length = vector.length();
	std::cout << vekt_length << std::endl;					//output sqrt(14) = 3.7166

	Vector3D  vekt_norm = vector.normal();
	float norm_x = vekt_norm.getX();
	float norm_y = vekt_norm.getY();
	float norm_z = vekt_norm.getZ();
	std::cout << norm_x << " " << norm_y << " " << norm_z << std::endl;		//output 0.27 0.54 0.81
	
	float dot = vector.dotproduct(vectSum);		//vector*vectSum
	std::cout << dot << std::endl;				//output 26

	Vector3D cross_vekt = vector.cross_product(vectSum);
	float cross_x = cross_vekt.getX();
	float cross_y = cross_vekt.getY();
	float cross_z = cross_vekt.getZ();
	std::cout << cross_x << cross_y << cross_z << std::endl;		//output -2 -4 -2

	Vector3D cross_vekt2 = vector1.cross_product(vector);
	float cross_x1 = cross_vekt2.getX();
	float cross_y1 = cross_vekt2.getY();
	float cross_z1 = cross_vekt2.getZ();
	std::cout << cross_x1 << cross_y1 << cross_z1 << std::endl;		//output 2 4 2

	Matrix3D matrix(1, 2, 3, 4, 5, 6, 7, 8, 9, 1, 2, 3, 4, 5, 6, 7);
	Matrix3D matrix2(1, 2, 3, 4, 5, 6, 7, 8, 9, 1, 1, 1, 1, 1, 1, 1);
	Matrix3D matrix_mult = matrix * matrix2;
	float m = matrix_mult.get(0, 0);
	float m1 = matrix_mult.get(0, 1);
	float m2 = matrix_mult.get(0, 2);
	float m3 = matrix_mult.get(0, 3);
	float m4 = matrix_mult.get(1, 0);
	float m5 = matrix_mult.get(1, 1);
	float m6 = matrix_mult.get(1, 2);
	float m7 = matrix_mult.get(1, 3);
	float m8 = matrix_mult.get(2, 0);
	float m9 = matrix_mult.get(2, 1);
	float m10 = matrix_mult.get(2, 2);
	float m11 = matrix_mult.get(2, 3);
	float m12 = matrix_mult.get(3, 0);
	float m13 = matrix_mult.get(3, 1);
	float m14 = matrix_mult.get(3, 2);
	float m15 = matrix_mult.get(3, 3);
	std::cout << "Row 1: " << m << " " << m1 << " " << m2 << " " << m3 << std::endl;
	std::cout << "Row 2: " << m4 << " " << m5 << " " << m6 << " " << m7 << std::endl;
	std::cout << "Row 3: " << m8 << " " << m9 << " " << m10 << " " << m11 << std::endl;
	std::cout << "Row 4: " << m12 << " " << m13 << " " << m14 << " " << m15 << std::endl;

	Matrix3D mat_in(1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 2, 1, 1, 1, 1, 2);
	Matrix3D inverse_matrix = mat_in.inverse();
	float a = inverse_matrix.get(0, 0);
	float a1 = inverse_matrix.get(0, 1);
	float a2 = inverse_matrix.get(0, 2);
	float a3 = inverse_matrix.get(0, 3);
	float a4 = inverse_matrix.get(1, 0);
	float a5 = inverse_matrix.get(1, 1);
	float a6 = inverse_matrix.get(1, 2);
	float a7 = inverse_matrix.get(1, 3);
	float a8 = inverse_matrix.get(2, 0);
	float a9 = inverse_matrix.get(2, 1);
	float a10 = inverse_matrix.get(2, 2);
	float a11 = inverse_matrix.get(2, 3);
	float a12 = inverse_matrix.get(3, 0);
	float a13 = inverse_matrix.get(3, 1);
	float a14 = inverse_matrix.get(3, 2);
	float a15 = inverse_matrix.get(3, 3);
	std::cout << "Row 1: " << a << " " << a1 << " " << a2 << " " << a3 << std::endl;
	std::cout << "Row 2: " << a4 << " " << a5 << " " << a6 << " " << a7 << std::endl;
	std::cout << "Row 3: " << a8 << " " << a9 << " " << a10 << " " << a11 << std::endl;
	std::cout << "Row 4: " << a12 << " " << a13 << " " << a14 << " " << a15 << std::endl;

	Vector3D mat_vec(mat_in * vector);
	std::cout << "X:" << mat_vec.getX() << std::endl;
	std::cout << "Y:" << mat_vec.getY() << std::endl;
	std::cout << "Z:" << mat_vec.getZ() << std::endl;
	std::cout << "W:" << mat_vec.getW() << std::endl;

	Matrix3D rot_matrix_x(1,2,1,2,1,2,1,2,1);
	Matrix3D temp;																			//create a temp matrix which will be rotated
	temp = temp.rotate_x(90.0f);															//rotate 90 degrees
	rot_matrix_x = rot_matrix_x * temp;														//rotating matrix by multiplying it with the temp rotation matrix
	float e = rot_matrix_x.get(0, 0);
	float e1 = rot_matrix_x.get(0, 1);
	float e2 = rot_matrix_x.get(0, 2);
	float e3 = rot_matrix_x.get(1, 0);
	float e4 = rot_matrix_x.get(1, 1);
	float e5 = rot_matrix_x.get(1, 2);
	float e6 = rot_matrix_x.get(2, 0);
	float e7 = rot_matrix_x.get(2, 1);
	float e8 = rot_matrix_x.get(2, 2);
	std::cout << "Row 1: " << e << " " << e1 << " " << e2 << std::endl;
	std::cout << "Row 2: " << e3 << " " << e4 << " " << e5 << std::endl;
	std::cout << "Row 3: " << e6 << " " << e7 << " " << e8 << std::endl;

	Matrix3D rot_matrix_y(1, 2, 1, 2, 1, 2, 1, 2, 1);									  //rotate 90 degrees
	Matrix3D temp1;																		  //create a temp matrix which will be rotated
	temp1 = temp1.rotate_y(90.0f);
	rot_matrix_y = rot_matrix_y * temp1;											      //rotating matrix by multiplying it with the temp rotation matrix
	float q = rot_matrix_y.get(0, 0);
	float q1 = rot_matrix_y.get(0, 1);
	float q2 = rot_matrix_y.get(0, 2);
	float q3 = rot_matrix_y.get(1, 0);
	float q4 = rot_matrix_y.get(1, 1);
	float q5 = rot_matrix_y.get(1, 2);
	float q6 = rot_matrix_y.get(2, 0);
	float q7 = rot_matrix_y.get(2, 1);
	float q8 = rot_matrix_y.get(2, 2);
	std::cout << "Row 1: " << q << " " << q1 << " " << q2 << std::endl;
	std::cout << "Row 2: " << q3 << " " << q4 << " " << q5 << std::endl;
	std::cout << "Row 3: " << q6 << " " << q7 << " " << q8 << std::endl;

	Matrix3D rot_matrix_z(1, 2, 1, 2, 1, 2, 1, 2, 1);									//rotate 90 degrees
	Matrix3D temp2;																		//create a temp matrix which will be rotated
	temp2 = temp2.rotate_z(90.0f);
	rot_matrix_z = rot_matrix_z * temp2;												//rotating matrix by multiplying it with the temp rotation matrix
	float r = rot_matrix_z.get(0, 0);
	float r1 = rot_matrix_z.get(0, 1);
	float r2 = rot_matrix_z.get(0, 2);
	float r3 = rot_matrix_z.get(1, 0);
	float r4 = rot_matrix_z.get(1, 1);
	float r5 = rot_matrix_z.get(1, 2);
	float r6 = rot_matrix_z.get(2, 0);
	float r7 = rot_matrix_z.get(2, 1);
	float r8 = rot_matrix_z.get(2, 2);
	std::cout << "Row 1: " << r << " " << r1 << " " << r2 << std::endl;
	std::cout << "Row 2: " << r3 << " " << r4 << " " << r5 << std::endl;
	std::cout << "Row 3: " << r6 << " " << r7 << " " << r8 << std::endl;

	Matrix3D rot_matrix_v(1, 2, 1, 2, 1, 2, 1, 2, 1);									//rotate 90 degrees
	Matrix3D temp3;																	    //create a temp matrix which will be rotated
	temp3 = temp3.rotate_vec(vector1, 90.0f);
	rot_matrix_v = rot_matrix_v * temp3;											    //rotating matrix by multiplying it with the temp rotation matrix
	float o = rot_matrix_v.get(0, 0);
	float o1 = rot_matrix_v.get(0, 1);
	float o2 = rot_matrix_v.get(0, 2);
	float o3 = rot_matrix_v.get(1, 0);
	float o4 = rot_matrix_v.get(1, 1);
	float o5 = rot_matrix_v.get(1, 2);
	float o6 = rot_matrix_v.get(2, 0);
	float o7 = rot_matrix_v.get(2, 1);
	float o8 = rot_matrix_v.get(2, 2);
	std::cout << "Row 1: " << o << " " << o1 << " " << o2 << std::endl;
	std::cout << "Row 2: " << o3 << " " << o4 << " " << o5 << std::endl;
	std::cout << "Row 3: " << o6 << " " << o7 << " " << o8 << std::endl;

	Matrix3D transposed(matrix.transpose());
	float n = transposed.get(0, 0);
	float n1 = transposed.get(0, 1);
	float n2 = transposed.get(0, 2);
	float n3 = transposed.get(0, 3);
	float n4 = transposed.get(1, 0);
	float n5 = transposed.get(1, 1);
	float n6 = transposed.get(1, 2);
	float n7 = transposed.get(1, 3);
	float n8 = transposed.get(2, 0);
	float n9 = transposed.get(2, 1);
	float n10 = transposed.get(2, 2);
	float n11 = transposed.get(2, 3);
	float n12 = transposed.get(3, 0);
	float n13 = transposed.get(3, 1);
	float n14 = transposed.get(3, 2);
	float n15 = transposed.get(3, 3);
	std::cout << "Row 1: " << n << " " << n1 << " " << n2 << " " << n3 << std::endl;
	std::cout << "Row 2: " << n4 << " " << n5 << " " << n6 << " " << n7 << std::endl;
	std::cout << "Row 3: " << n8 << " " << n9 << " " << n10 << " " << n11 << std::endl;
	std::cout << "Row 4: " << n12 << " " << n13 << " " << n14 << " " << n15 << std::endl;

	float pause;
	std::cin >> pause;
}