#include <Novice.h>
#include <imgui.h>
#include "Vector3Math.h"
#include "MatrixMath.h"
#include "MakeMatrix.h"
#include "Draw.h"

const char kWindowTitle[] = "LE2B_17_ナガイ_コハク_3次元の衝突判定(平面と線)";

bool IsCollision(const Segment& segment, const Plane& plane);

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1280, 720);

	// キー入力結果を受け取る箱
	char keys[256] = { 0 };
	char preKeys[256] = { 0 };

	//ウィンドウサイズ
	float kWindowWidth = 1280.0f;
	float kWindowHeight = 720.0f;

	Segment segment{ {-0.45f,0.33f,0.0f},{0.5f,0.58f,0.0f} };

	Plane plane{ Vector3(0.0f,1.0f,0.0f),1.0f };

	Vector3 cameraTranslate{ 0.0f,1.9f,-6.49f };

	Vector3 cameraRotate{ 0.26f,0.0f,0.0f };

	Draw draw;

	// ウィンドウの×ボタンが押されるまでループ
	while (Novice::ProcessMessage() == 0) {
		// フレームの開始
		Novice::BeginFrame();

		// キー入力を受け取る
		memcpy(preKeys, keys, 256);
		Novice::GetHitKeyStateAll(keys);

		///
		/// ↓更新処理ここから
		///

		ImGui::DragFloat3("segmentOrigin", &segment.origin.x, 0.01f);
		ImGui::DragFloat3("segmentDiff", &segment.diff.x, 0.01f);
		ImGui::DragFloat3("plateNormal", &plane.normal.x, 0.01f);
		ImGui::DragFloat3("cameraTranslate", &cameraTranslate.x, 0.01f);
		ImGui::DragFloat3("cameraRotate", &cameraRotate.x, 0.01f);


		plane.normal = Vector3Math::Normalize(plane.normal);

		draw.Pipeline(cameraTranslate, cameraRotate, kWindowWidth, kWindowHeight);

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		draw.DrawGrid();

		draw.DrawSphere(Sphere{ segment.origin,0.01f }, RED);
		draw.DrawSphere(Sphere{ Vector3Math::Add(segment.origin,segment.diff),0.01f }, RED);

		if (IsCollision(segment, plane)) {
			draw.DrawLine(segment.origin, segment.diff, RED);
		} else {
			draw.DrawLine(segment.origin, segment.diff, WHITE);
		}

		draw.DrawPlane(plane, WHITE);

		///
		/// ↑描画処理ここまで
		///

		// フレームの終了
		Novice::EndFrame();

		// ESCキーが押されたらループを抜ける
		if (preKeys[DIK_ESCAPE] == 0 && keys[DIK_ESCAPE] != 0) {
			break;
		}
	}

	// ライブラリの終了
	Novice::Finalize();
	return 0;
}

bool IsCollision(const Segment& segment, const Plane& plane) {

	//法線と線の内積を求める
	float dot = Vector3Math::Dot(plane.normal, segment.diff);

	//垂直 = 並行であるので、衝突しているはずがない
	if (dot == 0.0f) {
		return false;
	}

	//tを求める
	float t = (plane.distance - Vector3Math::Dot(segment.origin, plane.normal)) / dot;

	//tの値と線の種類によって衝突しているかを判断する
	if (t >= 0.0f && t <= 1.0f) {
		return true;
	}

	return false;

}