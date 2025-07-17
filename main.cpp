#include <Novice.h>

#include <cmath>
#include <imgui.h>

const char kWindowTitle[] = "LE2C_20_タニタ_カイセイ";

struct Matrix4x4 {
	float m[4][4];
};

struct Vector3 {
	float x;
	float y;
	float z;


};

struct Segment {
	Vector3 start;
	Vector3 end;
};

struct Triangle
{
	Vector3 vertices[3];
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

// 三角形と線分の交差判定（簡易版）
bool IsSegmentTriangleIntersect(const Segment& segment, const Triangle& triangle, Vector3* outIntersection = nullptr) {
	Vector3 v0 = triangle.vertices[0];
	Vector3 v1 = triangle.vertices[1];
	Vector3 v2 = triangle.vertices[2];

	// 三角形の法線を計算
	Vector3 edge1 = Subtract(v1, v0);
	Vector3 edge2 = Subtract(v2, v0);
	Vector3 normal = Normalize(Cross(edge1, edge2));

	// 線分のベクトルと方向
	Vector3 dir = Subtract(segment.end, segment.start);
	float dotND = Dot(normal, dir);
	if (std::abs(dotND) < 1e-5f) return false; // 平面と平行

	// 線分と平面の交点tを求める
	float t = (Dot(normal, v0) - Dot(normal, segment.start)) / dotND;
	if (t < 0.0f || t > 1.0f) return false; // 線分の範囲外

	// 交点を求める
	Vector3 intersection = {
		segment.start.x + dir.x * t,
		segment.start.y + dir.y * t,
		segment.start.z + dir.z * t,
	};

	// バリューを barycentric 座標で判定（面積法）
	Vector3 v0v1 = Subtract(v1, v0);
	Vector3 v0v2 = Subtract(v2, v0);
	Vector3 v0p = Subtract(intersection, v0);

	float d00 = Dot(v0v1, v0v1);
	float d01 = Dot(v0v1, v0v2);
	float d11 = Dot(v0v2, v0v2);
	float d20 = Dot(v0p, v0v1);
	float d21 = Dot(v0p, v0v2);
	float denom = d00 * d11 - d01 * d01;

	float v = (d11 * d20 - d01 * d21) / denom;
	float w = (d00 * d21 - d01 * d20) / denom;
	float u = 1.0f - v - w;

	if (u >= 0 && v >= 0 && w >= 0) {
		if (outIntersection) *outIntersection = intersection;
		return true;
	}

	return false;
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

	Triangle triangle = {
	Vector3{ -1.0f, 0.0f, 1.0f },
	Vector3{ 1.0f, 0.0f, 1.0f },
	Vector3{ 0.0f, 0.0f, -1.0f }
	};

	Vector3 intersection{};



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

		ImGui::Begin("Window");
		ImGui::DragFloat3("CameraTranslate", &cameraTranslate.x, 0.01f);
		ImGui::DragFloat3("CameraRotate", &cameraRotate.x, 0.01f);

		ImGui::DragFloat3("Segment Start", &segment.start.x, 0.01f);
		ImGui::DragFloat3("Segment End", &segment.end.x, 0.01f);

		ImGui::DragFloat3("Triangle V0", &triangle.vertices[0].x, 0.01f);
		ImGui::DragFloat3("Triangle V1", &triangle.vertices[1].x, 0.01f);
		ImGui::DragFloat3("Triangle V2", &triangle.vertices[2].x, 0.01f);

		ImGui::End();

		bool hit = IsSegmentTriangleIntersect(segment, triangle, &intersection);

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///



		Vector3 start = Transform(Transform(segment.start, viewProjectionMatrix), viewportMatrix);
		Vector3 end = Transform(Transform(Add(segment.start, segment.end), viewProjectionMatrix), viewportMatrix);

		// 衝突していたら色を赤に、していなければ白に
		uint32_t segmentColor = hit ? 0xFF0000FF : 0xFFFFFFFF;

		Novice::DrawLine((int)start.x, (int)start.y, (int)end.x, (int)end.y, segmentColor);


		// 三角形を描画
		for (int i = 0; i < 3; ++i) {
			Vector3 p0 = Transform(Transform(triangle.vertices[i], viewProjectionMatrix), viewportMatrix);
			Vector3 p1 = Transform(Transform(triangle.vertices[(i + 1) % 3], viewProjectionMatrix), viewportMatrix);
			Novice::DrawLine((int)p0.x, (int)p0.y, (int)p1.x, (int)p1.y, WHITE);
		}

		//// 交差点を表示
		//if (hit) {
		//	Vector3 screenPos = Transform(Transform(intersection, viewProjectionMatrix), viewportMatrix);
		//	Novice::DrawBox((int)screenPos.x - 4, (int)screenPos.y - 4, 8, 8, 0.0f, 0x00FF00FF, kFillModeSolid);
		//}

		DrawGrid(viewProjectionMatrix, viewportMatrix);







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