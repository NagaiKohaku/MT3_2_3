#pragma once
#include "Matrix4x4.h"

class MatrixMath {

public:

	//行列の加法
	static Matrix4x4 Add(const Matrix4x4& m1, const Matrix4x4& m2);

	//行列の減法
	static Matrix4x4 Subtract(const Matrix4x4& m1, const Matrix4x4& m2);

	//行列の積
	static Matrix4x4 Multiply(const Matrix4x4& m1, const Matrix4x4& m2);

	//逆行列
	static Matrix4x4 Inverse(const Matrix4x4& m);

	//転置行列
	static Matrix4x4 Transpose(const Matrix4x4& m);

	//単位行列の作成
	static Matrix4x4 MakeIdentity4x4();

};