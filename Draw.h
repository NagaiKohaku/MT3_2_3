#pragma once
#include "Vector3.h"
#include "Matrix4x4.h"
#include "cmath"

struct Sphere {
	Vector3 center; //中心点
	float radius; //半径
};

struct Plane {
	Vector3 normal; //法線
	float distance; //距離
};

class Draw {

public:

	void Pipeline(const Vector3& cameraTranslate, const Vector3& cameraRotate, float kWindowWidth, float kWindowHeight);
	void DrawGrid();
	void DrawSphere(Sphere sphere, uint32_t color);
	void DrawLine(Vector3 start, Vector3 end, uint32_t color);
	void DrawPlane(Plane plane, uint32_t color);

private:

	Matrix4x4 cameraMatrix_;

	Matrix4x4 viewMatrix_;

	Matrix4x4 projectionMatrix_;

	Matrix4x4 viewProjectionMatrix_;

	Matrix4x4 viewPortMatrix_;

};