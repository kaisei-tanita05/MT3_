#include <Novice.h>
#include <cassert>
#include <cmath>
#include <imgui.h>
#include <stdio.h>
#include <stdbool.h>
#include <iostream>
#include <algorithm>

const char kWindowTitle[] = "LE2C_20_タニタ_カイセイ";

struct Matrix4x4 {
	float m[4][4];
};

struct Vector3 {
	float x;
	float y;
	float z;


};

struct Vector2 {
	float x;
	float y;
};

struct Segment {
	Vector3 start;
	Vector3 end;
};

struct AABB {
	Vector3 min;
	Vector3 max;
};

struct Sphere {
	Vector3 center;
	float radius;
};

float Dot(const Vector3& a, const Vector3& b) { return a.x * b.x + a.y * b.y + a.z * b.z; }

Vector3 Add(const Vector3& v1, const Vector3& v2)
{
	Vector3 v{};
	v.x = v1.x + v2.x;
	v.y = v1.y + v2.y;
	v.z = v1.z + v2.z;

	return v;

};

Vector3 Normalize(const Vector3& v) {
	float length = std::sqrt(Dot(v, v));
	return { v.x / length, v.y / length, v.z / length };
}

Vector3 Transform(const Vector3& v, const Matrix4x4& m) {
	float x = v.x * m.m[0][0] + v.y * m.m[1][0] + v.z * m.m[2][0] + m.m[3][0];
	float y = v.x * m.m[0][1] + v.y * m.m[1][1] + v.z * m.m[2][1] + m.m[3][1];
	float z = v.x * m.m[0][2] + v.y * m.m[1][2] + v.z * m.m[2][2] + m.m[3][2];
	float w = v.x * m.m[0][3] + v.y * m.m[1][3] + v.z * m.m[2][3] + m.m[3][3];

	if (std::abs(w) < 1e-5f)
		w = 1.0f; // 0除算防止
	return { x / w, y / w, z / w };
}

Matrix4x4 MakeIdentity() {
	Matrix4x4 result{};
	for (int i = 0; i < 4; ++i)
		result.m[i][i] = 1.0f;
	return result;
}

Matrix4x4 MakeTranslateMatrix(const Vector3& t) {
	Matrix4x4 result = MakeIdentity();
	result.m[3][0] = t.x;
	result.m[3][1] = t.y;
	result.m[3][2] = t.z;
	return result;
}

Matrix4x4 MakeRotateX(float angle) {
	Matrix4x4 result = MakeIdentity();
	result.m[1][1] = cosf(angle);
	result.m[1][2] = sinf(angle);
	result.m[2][1] = -sinf(angle);
	result.m[2][2] = cosf(angle);
	return result;
}

Matrix4x4 MakeRotateY(float angle) {
	Matrix4x4 result = MakeIdentity();
	result.m[0][0] = cosf(angle);
	result.m[0][2] = -sinf(angle);
	result.m[2][0] = sinf(angle);
	result.m[2][2] = cosf(angle);
	return result;
}

Matrix4x4 MakeRotateZ(float angle) {
	Matrix4x4 result = MakeIdentity();
	result.m[0][0] = cosf(angle);
	result.m[0][1] = sinf(angle);
	result.m[1][0] = -sinf(angle);
	result.m[1][1] = cosf(angle);
	return result;
}

Matrix4x4 Multiply(const Matrix4x4& m1, const Matrix4x4& m2) {
	Matrix4x4 result{};
	for (int i = 0; i < 4; ++i)
		for (int j = 0; j < 4; ++j)
			for (int k = 0; k < 4; ++k)
				result.m[i][j] += m1.m[i][k] * m2.m[k][j];
	return result;
}

Matrix4x4 MakePerspectiveFovMatrix(float fovY, float aspect, float nearZ, float farZ) {
	Matrix4x4 result{};
	float f = 1.0f / tanf(fovY / 2.0f);
	result.m[0][0] = f / aspect;
	result.m[1][1] = f;
	result.m[2][2] = farZ / (farZ - nearZ);
	result.m[2][3] = 1.0f;
	result.m[3][2] = (-nearZ * farZ) / (farZ - nearZ);
	return result;
}

Matrix4x4 MakeViewportMatrix(float left, float top, float width, float height, float minDepth, float maxDepth) {
	Matrix4x4 result = MakeIdentity();
	result.m[0][0] = width / 2.0f;
	result.m[1][1] = -height / 2.0f;
	result.m[2][2] = maxDepth - minDepth;
	result.m[3][0] = left + width / 2.0f;
	result.m[3][1] = top + height / 2.0f;
	result.m[3][2] = minDepth;
	return result;
}

void DrawGrid(const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix) {
	const float kGridHalfWidth = 2.0f;
	const uint32_t kSubdivision = 10;
	const float kGridEvery = (kGridHalfWidth * 2.0f) / float(kSubdivision);

	for (uint32_t xIndex = 0; xIndex <= kSubdivision; ++xIndex) {
		float x = -kGridHalfWidth + xIndex * kGridEvery;
		Vector3 start = Transform({ x, 0.0f, -kGridHalfWidth }, viewProjectionMatrix);
		Vector3 end = Transform({ x, 0.0f, kGridHalfWidth }, viewProjectionMatrix);
		start = Transform(start, viewportMatrix);
		end = Transform(end, viewportMatrix);
		uint32_t color = (std::abs(x) < 0.001f) ? 0x000000FF : 0xAAAAAAFF;
		Novice::DrawLine(int(start.x), int(start.y), int(end.x), int(end.y), color);
	}

	for (uint32_t zIndex = 0; zIndex <= kSubdivision; ++zIndex) {
		float z = -kGridHalfWidth + zIndex * kGridEvery;
		Vector3 start = Transform({ -kGridHalfWidth, 0.0f, z }, viewProjectionMatrix);
		Vector3 end = Transform({ kGridHalfWidth, 0.0f, z }, viewProjectionMatrix);
		start = Transform(start, viewportMatrix);
		end = Transform(end, viewportMatrix);
		uint32_t color = (std::abs(z) < 0.001f) ? 0x000000FF : 0xAAAAAAFF;
		Novice::DrawLine(int(start.x), int(start.y), int(end.x), int(end.y), color);
	}
}

// AABBの描画（線で囲む）
void DrawAABB(AABB& box, const Matrix4x4& vpMatrix, const Matrix4x4& viewport, uint32_t color) {
	Vector3 corners[8] = {
		{box.min.x, box.min.y, box.min.z},
		{box.max.x, box.min.y, box.min.z},
		{box.min.x, box.max.y, box.min.z},
		{box.max.x, box.max.y, box.min.z},
		{box.min.x, box.min.y, box.max.z},
		{box.max.x, box.min.y, box.max.z},
		{box.min.x, box.max.y, box.max.z},
		{box.max.x, box.max.y, box.max.z},
	};

	for (int i = 0; i < 8; ++i) {
		corners[i] = Transform(corners[i], vpMatrix);
		corners[i] = Transform(corners[i], viewport);
	}

	// 辺を線で描画
	int edges[12][2] = {
		{0,1},{1,3},{3,2},{2,0},
		{4,5},{5,7},{7,6},{6,4},
		{0,4},{1,5},{2,6},{3,7}
	};

	for (int i = 0; i < 12; ++i) {
		Novice::DrawLine((int)corners[edges[i][0]].x, (int)corners[edges[i][0]].y,
			(int)corners[edges[i][1]].x, (int)corners[edges[i][1]].y, color);
	}
}

void DrawSphereXY(const Sphere& sphere, const Matrix4x4& vp, const Matrix4x4& viewport, uint32_t color) {
	const int kSegments = 36;
	const float kPI = 3.141592f;
	for (int i = 0; i < kSegments; ++i) {
		float theta1 = (float)i / kSegments * 2.0f * kPI;
		float theta2 = (float)(i + 1) / kSegments * 2.0f * kPI;
		Vector3 p1 = { sphere.center.x + cosf(theta1) * sphere.radius, sphere.center.y + sinf(theta1) * sphere.radius, sphere.center.z };
		Vector3 p2 = { sphere.center.x + cosf(theta2) * sphere.radius, sphere.center.y + sinf(theta2) * sphere.radius, sphere.center.z };

		p1 = Transform(p1, vp);
		p1 = Transform(p1, viewport);
		p2 = Transform(p2, vp);
		p2 = Transform(p2, viewport);
		Novice::DrawLine((int)p1.x, (int)p1.y, (int)p2.x, (int)p2.y, color);
	}
}


float Clamp(float value, float min, float max) {
	if (value < min) return min;
	if (value > max) return max;
	return value;
}


Vector3 Cross(const Vector3& v1, const Vector3& v2) {
	Vector3 result{};

	result.x = (v1.y * v2.z) - (v1.z * v2.y);

	result.y = (v1.z * v2.x) - (v1.x * v2.z);

	result.z = (v1.x * v2.y) - (v1.y * v2.x);

	return result;
}



Vector3 Perpendicular(const Vector3& vector) {
	if (vector.x != 0.0f || vector.y != 0.0f) {
		return{ -vector.y,vector.x,0.0f };
	}
	return{ 0.0f,-vector.z,vector.y };
}

// ベクトルの減算
Vector3 Subtract(const Vector3& a, const Vector3& b) {
	return { a.x - b.x, a.y - b.y, a.z - b.z };
}

bool IsAABBvsSphere(const AABB* box, const Sphere* sphere) {
	Vector3 closest{};
	closest.x = Clamp(sphere->center.x, box->min.x, box->max.x);
	closest.y = Clamp(sphere->center.y, box->min.y, box->max.y);
	closest.z = Clamp(sphere->center.z, box->min.z, box->max.z);

	Vector3 diff = Subtract(sphere->center, closest);
	float distanceSq = Dot(diff, diff);

	return distanceSq <= (sphere->radius * sphere->radius);
}

bool LineIntersectsAABB(const Segment& segment, const AABB& box) {
	float tmin = 0.0f;
	float tmax = 1.0f;
	Vector3 d = Subtract(segment.end, segment.start);

	for (int i = 0; i < 3; i++) {
		float p = (i == 0) ? d.x : (i == 1) ? d.y : d.z;
		float start = (i == 0) ? segment.start.x : (i == 1) ? segment.start.y : segment.start.z;
		float minB = (i == 0) ? box.min.x : (i == 1) ? box.min.y : box.min.z;
		float maxB = (i == 0) ? box.max.x : (i == 1) ? box.max.y : box.max.z;

		if (p == 0.0f) {
			// 線分がこの軸に対して平行で、かつボックス外  
			if (start < minB || start > maxB)
				return false;
		}
		else {
			float ood = 1.0f / p;
			float t1 = (minB - start) * ood;
			float t2 = (maxB - start) * ood;
			if (t1 > t2) std::swap(t1, t2);

			tmin = std::fmax(tmin, t1);
			tmax = std::fmin(tmax, t2);

			if (tmin > tmax)
				return false; // 交差しない  
		}
	}

	return true;
}



// Windowsアプリでのエントリーポイント(main関数
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1280, 720);

	// キー入力結果を受け取る箱
	char keys[256] = { 0 };
	char preKeys[256] = { 0 };

	Vector3 cameraTranslate{ 0.0f, 1.9f, -6.49f };
	Vector3 cameraRotate{ 0.26f, 0.0f, 0.0f };

	Segment segment = {
	{0.0f, 0.0f, 0.0f},  // 始点
	{0.0f, 1.0f, 0.0f}   // 終点
	};

	AABB box1 = { {-0.5f, 0.0f, -0.5f}, {0.5f, 1.0f, 0.5f} };
	AABB box2 = { {0.0f, 0.5f, 0.0f}, {1.0f, 1.5f, 1.0f} };
	Sphere sphere = { {0.5f, 0.5f, 0.5f}, 0.3f };

	AABB box = { .min{-0.5f,0.0f, -0.5f,}, .max{0.5f, 0.5f,0.5f} };
    

    // 修正された呼び出し  
    bool hit = LineIntersectsAABB(segment, box);
	Vector2 lineStart = { -1.0f, 2.0f };
	Vector2 lineEnd = { 6.0f, 2.0f };

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

		Matrix4x4 cameraT = MakeTranslateMatrix({ -cameraTranslate.x, -cameraTranslate.y, -cameraTranslate.z });
		Matrix4x4 cameraRY = MakeRotateY(-cameraRotate.y);
		Matrix4x4 cameraRX = MakeRotateX(-cameraRotate.x);
		Matrix4x4 viewMatrix = Multiply(cameraT, Multiply(cameraRY, cameraRX));

		Matrix4x4 projectionMatrix = MakePerspectiveFovMatrix(0.45f, 1280.0f / 720.0f, 0.1f, 100.0f);
		Matrix4x4 viewProjectionMatrix = Multiply(viewMatrix, projectionMatrix);
		Matrix4x4 viewportMatrix = MakeViewportMatrix(0, 0, 1280, 720, 0.0f, 1.0f);

		if (LineIntersectsAABB(segment, box)) {
			hit = true;
		}
		else {
			hit = false;
		}

		ImGui::Begin("Window");
		ImGui::DragFloat3("CameraTranslate", &cameraTranslate.x, 0.01f);
		ImGui::DragFloat3("CameraRotate", &cameraRotate.x, 0.01f);

		ImGui::DragFloat3("Segment Start", &segment.start.x, 0.01f);
		ImGui::DragFloat3("Segment End", &segment.end.x, 0.01f);

		// ImGui上で表示

		//AABB
		ImGui::DragFloat3("Box Min", &box.min.x, 0.01f);
		ImGui::DragFloat3("Box Max", &box.max.x, 0.01f);
		ImGui::DragFloat3("Box Min", &box2.min.x, 0.01f);
		ImGui::DragFloat3("Box Max", &box2.max.x, 0.01f);

		//Sphere
		ImGui::DragFloat3("Sphere Center", &sphere.center.x, 0.01f);
		ImGui::DragFloat("Sphere Radius", &sphere.radius, 0.01f, 0.01f, 10.0f);

		ImGui::End();



		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		// 衝突判定
		LineIntersectsAABB(segment, box);


		DrawGrid(viewProjectionMatrix, viewportMatrix);

		// 描画
		Vector3 start = Transform(Transform(segment.start, viewProjectionMatrix), viewportMatrix);
		Vector3 end = Transform(Transform(Add(segment.start, segment.end), viewProjectionMatrix), viewportMatrix);

		// 衝突していたら色を赤に、していなければ白に
		//uint32_t segmentColor = hit ? 0xFF0000FF : 0xFFFFFFFF;

		Novice::DrawLine((int)start.x, (int)start.y, (int)end.x, (int)end.y,0xFFFFFFFF);


		DrawAABB(box, viewProjectionMatrix, viewportMatrix, hit ? 0xFF0000FF : 0x00FF00FF);
		//DrawSphereXY(sphere, viewProjectionMatrix, viewportMatrix, hit ? 0xFF0000FF : 0x0000FFFF);







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