#define PI 3.14f
#include "Draw.h"
#include "Novice.h"
#include "Vector3Math.h"
#include "MatrixMath.h"
#include "MakeMatrix.h"

void Draw::Pipeline(const Vector3& cameraTranslate, const Vector3& cameraRotate, float kWindowWidth, float kWindowHeight) {

	cameraMatrix_ = MakeMatrix::MakeAffineMatrix({ 1.0f,1.0f,1.0f }, cameraRotate, cameraTranslate);

	viewMatrix_ = MatrixMath::Inverse(cameraMatrix_);

	projectionMatrix_ = MakeMatrix::MakePerspectiveFovMatrix(0.45f, float(kWindowWidth) / float(kWindowHeight), 0.1f, 100.0f);

	viewProjectionMatrix_ = MatrixMath::Multiply(viewMatrix_, projectionMatrix_);

	viewPortMatrix_ = MakeMatrix::MakeViewportMatrix(0, 0, float(kWindowWidth), float(kWindowHeight), 0.0f, 1.0f);

}

void Draw::DrawGrid() {

	//Gridの半分の幅
	const float kGridHalfWidth = 2.0f;

	//分割数
	const uint32_t kSubdivision = 10;

	//1つ分の長さ
	const float kGridEvery = (kGridHalfWidth * 2.0f) / float(kSubdivision);

	Matrix4x4 worldMatrix = MakeMatrix::MakeAffineMatrix(
		Vector3(1, 1, 1),
		Vector3(0, 0, 0),
		Vector3(0, 0, 0)
	);

	Matrix4x4 worldViewProjectionMatrix = MatrixMath::Multiply(worldMatrix, viewProjectionMatrix_);

	//奥から手前への線を順順で引いていく
	for (uint32_t xIndex = 0; xIndex <= kSubdivision; ++xIndex) {

		float start = -(kSubdivision / 2.0f) * kGridEvery;
		float end = (kSubdivision / 2.0f) * kGridEvery;

		Vector3 startNdcVertex = MakeMatrix::Transform(Vector3(((kSubdivision / 2.0f) - xIndex) * kGridEvery, 0, start), worldViewProjectionMatrix);

		Vector3 startScreenVertex = MakeMatrix::Transform(startNdcVertex, viewPortMatrix_);

		Vector3 endNdcVertex = MakeMatrix::Transform(Vector3(((kSubdivision / 2.0f) - xIndex) * kGridEvery, 0, end), worldViewProjectionMatrix);

		Vector3 endScreenVertex = MakeMatrix::Transform(endNdcVertex, viewPortMatrix_);

		if (xIndex == kSubdivision / 2.0f) {
			Novice::DrawLine(
				int(startScreenVertex.x),
				int(startScreenVertex.y),
				int(endScreenVertex.x),
				int(endScreenVertex.y),
				0x000000FF);
		} else {
			Novice::DrawLine(
				int(startScreenVertex.x),
				int(startScreenVertex.y),
				int(endScreenVertex.x),
				int(endScreenVertex.y),
				0xAAAAAAFF);
		}

	}

	//左から右も同じように順々に引いていく
	for (uint32_t zIndex = 0; zIndex <= kSubdivision; ++zIndex) {

		float start = -(kSubdivision / 2.0f) * kGridEvery;
		float end = (kSubdivision / 2.0f) * kGridEvery;

		Vector3 startNdcVertex = MakeMatrix::Transform(Vector3(start, 0, ((kSubdivision / 2.0f) - zIndex) * kGridEvery), worldViewProjectionMatrix);

		Vector3 startScreenVertex = MakeMatrix::Transform(startNdcVertex, viewPortMatrix_);

		Vector3 endNdcVertex = MakeMatrix::Transform(Vector3(end, 0, ((kSubdivision / 2.0f) - zIndex) * kGridEvery), worldViewProjectionMatrix);

		Vector3 endScreenVertex = MakeMatrix::Transform(endNdcVertex, viewPortMatrix_);

		if (zIndex == kSubdivision / 2.0f) {
			Novice::DrawLine(
				int(startScreenVertex.x),
				int(startScreenVertex.y),
				int(endScreenVertex.x),
				int(endScreenVertex.y),
				0x000000FF);
		} else {
			Novice::DrawLine(
				int(startScreenVertex.x),
				int(startScreenVertex.y),
				int(endScreenVertex.x),
				int(endScreenVertex.y),
				0xAAAAAAFF);
		}
	}
}

void Draw::DrawSphere(Sphere sphere, uint32_t color) {

	const uint32_t kSubDivision = 12;

	//経度1つ分の角度(φ)
	const float kLonEvery = (2.0f * PI) / kSubDivision;

	//緯度1つ分の角度(θ)
	const float kLatEvery = PI / kSubDivision;

	Matrix4x4 worldMatrix = MakeMatrix::MakeAffineMatrix(
		Vector3(1, 1, 1),
		Vector3(0, 0, 0),
		Vector3(0, 0, 0)
	);

	Matrix4x4 worldViewProjectionMatrix = MatrixMath::Multiply(worldMatrix, viewProjectionMatrix_);

	//緯度(θ)の方向に分割　-π/2 ~ π/2
	for (uint32_t latIndex = 0; latIndex < kSubDivision; ++latIndex) {

		float lat = -PI / 2.0f + kLatEvery * latIndex;

		//経度(φ)の方向に分割 0 ~ 2π
		for (uint32_t lonIndex = 0; lonIndex < kSubDivision; ++lonIndex) {

			float lon = lonIndex * kLonEvery;

			//world座標系でのa,b,cを決める
			Vector3 a, b, c;

			a = { sphere.radius * cosf(lat) * cosf(lon)             , sphere.radius * sinf(lat)             , sphere.radius * cosf(lat) * sinf(lon) };
			b = { sphere.radius * cosf(lat + kLatEvery) * cosf(lon) , sphere.radius * sinf(lat + kLatEvery) , sphere.radius * cosf(lat + kLatEvery) * sinf(lon) };
			c = { sphere.radius * cosf(lat) * cosf(lon + kLonEvery) , sphere.radius * sinf(lat)             , sphere.radius * cosf(lat) * sinf(lon + kLonEvery) };

			a.x += sphere.center.x;
			a.y += sphere.center.y;
			a.z += sphere.center.z;

			b.x += sphere.center.x;
			b.y += sphere.center.y;
			b.z += sphere.center.z;

			c.x += sphere.center.x;
			c.y += sphere.center.y;
			c.z += sphere.center.z;

			Vector3 ANdcVertex = MakeMatrix::Transform(a, worldViewProjectionMatrix);
			Vector3 BNdcVertex = MakeMatrix::Transform(b, worldViewProjectionMatrix);
			Vector3 CNdcVertex = MakeMatrix::Transform(c, worldViewProjectionMatrix);

			Vector3 AScreenVertex = MakeMatrix::Transform(ANdcVertex, viewPortMatrix_);
			Vector3 BScreenVertex = MakeMatrix::Transform(BNdcVertex, viewPortMatrix_);
			Vector3 CScreenVertex = MakeMatrix::Transform(CNdcVertex, viewPortMatrix_);

			Novice::DrawLine(
				int(AScreenVertex.x),
				int(AScreenVertex.y),
				int(BScreenVertex.x),
				int(BScreenVertex.y),
				color
			);

			Novice::DrawLine(
				int(AScreenVertex.x),
				int(AScreenVertex.y),
				int(CScreenVertex.x),
				int(CScreenVertex.y),
				color
			);

		}
	}
}

void Draw::DrawLine(Vector3 start, Vector3 end, uint32_t color) {

	Matrix4x4 worldMatrix = MakeMatrix::MakeAffineMatrix(
		Vector3(1, 1, 1),
		Vector3(0, 0, 0),
		Vector3(0, 0, 0)
	);

	Matrix4x4 worldViewProjectionMatrix = MatrixMath::Multiply(worldMatrix, viewProjectionMatrix_);

	Vector3 startNdcVertex = MakeMatrix::Transform(start, worldViewProjectionMatrix);
	Vector3 startScreenVertex = MakeMatrix::Transform(startNdcVertex, viewPortMatrix_);

	Vector3 endNdcVertex = MakeMatrix::Transform(Vector3Math::Add(start,end), worldViewProjectionMatrix);
	Vector3 endScreenVertex = MakeMatrix::Transform(endNdcVertex, viewPortMatrix_);

	Novice::DrawLine(
		int(startScreenVertex.x),
		int(startScreenVertex.y),
		int(endScreenVertex.x),
		int(endScreenVertex.y),
		color
	);
}

void Draw::DrawPlane(Plane plane, uint32_t color) {

	Vector3 center = Vector3Math::Multiply(plane.distance,plane.normal);

	Vector3 perpendicular[4];
	perpendicular[0] = Vector3Math::Normalize(Vector3Math::Perpendicular(plane.normal));
	perpendicular[1] = { -perpendicular[0].x,-perpendicular[0].y,-perpendicular[0].z };
	perpendicular[2] = Vector3Math::Cross(plane.normal, perpendicular[0]);
	perpendicular[3] = { -perpendicular[2].x,-perpendicular[2].y,-perpendicular[2].z };

	Vector3 points[4];
	for (int32_t index = 0; index < 4; ++index) {
		Vector3 extend = Vector3Math::Multiply(2.0f, perpendicular[index]);
		Vector3 point = Vector3Math::Add(center, extend);
		points[index] = MakeMatrix::Transform(MakeMatrix::Transform(point, viewProjectionMatrix_), viewPortMatrix_);
	}

	Novice::DrawLine(
		int(points[0].x),
		int(points[0].y),
		int(points[3].x),
		int(points[3].y),
		color
	);

	Novice::DrawLine(
		int(points[3].x),
		int(points[3].y),
		int(points[1].x),
		int(points[1].y),
		color
	);

	Novice::DrawLine(
		int(points[1].x),
		int(points[1].y),
		int(points[2].x),
		int(points[2].y),
		color
	);

	Novice::DrawLine(
		int(points[2].x),
		int(points[2].y),
		int(points[0].x),
		int(points[0].y),
		color
	);
}