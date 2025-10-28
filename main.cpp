#include <Novice.h>
#include <cmath>
#include <imgui.h>
#include <numbers>

const char kWindowTitle[] = "LE2C_20_タニタ_カイセイ";


struct Vector3
{
	float x;
	float y;
	float z;
};

struct Matrix4x4
{
	float m[4][4];
};

// Quaternion構造体
struct Quaternion
{
	float x;
	float y;
	float z;
	float w;
};

//x軸回転行列
Matrix4x4 MakeRotateXMatrix(float radian)
{
	Matrix4x4 result;

	result.m[0][0] = 1.0f;
	result.m[0][1] = 0.0f;
	result.m[0][2] = 0.0f;
	result.m[0][3] = 0.0f;
	result.m[1][0] = 0.0f;
	result.m[1][1] = std::cos(radian);
	result.m[1][2] = std::sin(radian);
	result.m[1][3] = 0.0f;
	result.m[2][0] = 0.0f;
	result.m[2][1] = -std::sin(radian);
	result.m[2][2] = std::cos(radian);
	result.m[2][3] = 0.0f;
	result.m[3][0] = 0.0f;
	result.m[3][1] = 0.0f;
	result.m[3][2] = 0.0f;
	result.m[3][3] = 1.0f;


	return result;
}

// y軸回転行列
Matrix4x4 MakeRotateYMatrix(float radian)
{
	Matrix4x4 result;

	result.m[0][0] = std::cos(radian);
	result.m[0][1] = 0.0f;
	result.m[0][2] = -std::sin(radian);
	result.m[0][3] = 0.0f;
	result.m[1][0] = 0.0f;
	result.m[1][1] = 1.0f;
	result.m[1][2] = 0.0f;
	result.m[1][3] = 0.0f;
	result.m[2][0] = std::sin(radian);
	result.m[2][1] = 0.0f;
	result.m[2][2] = std::cos(radian);
	result.m[2][3] = 0.0f;
	result.m[3][0] = 0.0f;
	result.m[3][1] = 0.0f;
	result.m[3][2] = 0.0f;
	result.m[3][3] = 1.0f;


	return result;
}

// z軸回転行列
Matrix4x4 MakeRotateZMatrix(float radian)
{
	Matrix4x4 result;

	result.m[0][0] = std::cos(radian);
	result.m[0][1] = std::sin(radian);
	result.m[0][2] = 0.0f;
	result.m[0][3] = 0.0f;
	result.m[1][0] = -std::sin(radian);
	result.m[1][1] = std::cos(radian);
	result.m[1][2] = 0.0f;
	result.m[1][3] = 0.0f;
	result.m[2][0] = 0.0f;
	result.m[2][1] = 0.0f;
	result.m[2][2] = 1.0f;
	result.m[2][3] = 0.0f;
	result.m[3][0] = 0.0f;
	result.m[3][1] = 0.0f;
	result.m[3][2] = 0.0f;
	result.m[3][3] = 1.0f;

	return result;
}

// 加算
Vector3 Add(const Vector3& v1, const Vector3& v2)
{
	Vector3 result;

	result.x = v1.x + v2.x;
	result.y = v1.y + v2.y;
	result.z = v1.z + v2.z;

	return result;

}

// 減算
Vector3 Subtract(const Vector3& v1, const Vector3& v2)
{

	Vector3 result;

	result.x = v1.x - v2.x;
	result.y = v1.y - v2.y;
	result.z = v1.z - v2.z;

	return result;

}

// スカラー倍
Vector3 Multiply(float scalar, const Vector3& v)
{
	Vector3 result;

	result.x = scalar * v.x;
	result.y = scalar * v.y;
	result.z = scalar * v.z;

	return result;

}

//行列の加法
Matrix4x4 Add(const Matrix4x4& m1, const Matrix4x4& m2)
{
	Matrix4x4 result;

	result.m[0][0] = m1.m[0][0] + m2.m[0][0];
	result.m[0][1] = m1.m[0][1] + m2.m[0][1];
	result.m[0][2] = m1.m[0][2] + m2.m[0][2];
	result.m[0][3] = m1.m[0][3] + m2.m[0][3];
	result.m[1][0] = m1.m[1][0] + m2.m[1][0];
	result.m[1][1] = m1.m[1][1] + m2.m[1][1];
	result.m[1][2] = m1.m[1][2] + m2.m[1][2];
	result.m[1][3] = m1.m[1][3] + m2.m[1][3];
	result.m[2][0] = m1.m[2][0] + m2.m[2][0];
	result.m[2][1] = m1.m[2][1] + m2.m[2][1];
	result.m[2][2] = m1.m[2][2] + m2.m[2][2];
	result.m[2][3] = m1.m[2][3] + m2.m[2][3];
	result.m[3][0] = m1.m[3][0] + m2.m[3][0];
	result.m[3][1] = m1.m[3][1] + m2.m[3][1];
	result.m[3][2] = m1.m[3][2] + m2.m[3][2];
	result.m[3][3] = m1.m[3][3] + m2.m[3][3];

	return result;
}

//行列の減法
Matrix4x4 Subtract(const Matrix4x4& m1, const Matrix4x4& m2)
{
	Matrix4x4 result;

	result.m[0][0] = m1.m[0][0] - m2.m[0][0];
	result.m[0][1] = m1.m[0][1] - m2.m[0][1];
	result.m[0][2] = m1.m[0][2] - m2.m[0][2];
	result.m[0][3] = m1.m[0][3] - m2.m[0][3];
	result.m[1][0] = m1.m[1][0] - m2.m[1][0];
	result.m[1][1] = m1.m[1][1] - m2.m[1][1];
	result.m[1][2] = m1.m[1][2] - m2.m[1][2];
	result.m[1][3] = m1.m[1][3] - m2.m[1][3];
	result.m[2][0] = m1.m[2][0] - m2.m[2][0];
	result.m[2][1] = m1.m[2][1] - m2.m[2][1];
	result.m[2][2] = m1.m[2][2] - m2.m[2][2];
	result.m[2][3] = m1.m[2][3] - m2.m[2][3];
	result.m[3][0] = m1.m[3][0] - m2.m[3][0];
	result.m[3][1] = m1.m[3][1] - m2.m[3][1];
	result.m[3][2] = m1.m[3][2] - m2.m[3][2];
	result.m[3][3] = m1.m[3][3] - m2.m[3][3];

	return result;
}

//行列の積
Matrix4x4 Multiply(const Matrix4x4& m1, const Matrix4x4& m2)
{
	Matrix4x4 result;

	result.m[0][0] = m1.m[0][0] * m2.m[0][0] + m1.m[0][1] * m2.m[1][0] + m1.m[0][2] * m2.m[2][0] + m1.m[0][3] * m2.m[3][0];
	result.m[0][1] = m1.m[0][0] * m2.m[0][1] + m1.m[0][1] * m2.m[1][1] + m1.m[0][2] * m2.m[2][1] + m1.m[0][3] * m2.m[3][1];
	result.m[0][2] = m1.m[0][0] * m2.m[0][2] + m1.m[0][1] * m2.m[1][2] + m1.m[0][2] * m2.m[2][2] + m1.m[0][3] * m2.m[3][2];
	result.m[0][3] = m1.m[0][0] * m2.m[0][3] + m1.m[0][1] * m2.m[1][3] + m1.m[0][2] * m2.m[2][3] + m1.m[0][3] * m2.m[3][3];
	result.m[1][0] = m1.m[1][0] * m2.m[0][0] + m1.m[1][1] * m2.m[1][0] + m1.m[1][2] * m2.m[2][0] + m1.m[1][3] * m2.m[3][0];
	result.m[1][1] = m1.m[1][0] * m2.m[0][1] + m1.m[1][1] * m2.m[1][1] + m1.m[1][2] * m2.m[2][1] + m1.m[1][3] * m2.m[3][1];
	result.m[1][2] = m1.m[1][0] * m2.m[0][2] + m1.m[1][1] * m2.m[1][2] + m1.m[1][2] * m2.m[2][2] + m1.m[1][3] * m2.m[3][2];
	result.m[1][3] = m1.m[1][0] * m2.m[0][3] + m1.m[1][1] * m2.m[1][3] + m1.m[1][2] * m2.m[2][3] + m1.m[1][3] * m2.m[3][3];
	result.m[2][0] = m1.m[2][0] * m2.m[0][0] + m1.m[2][1] * m2.m[1][0] + m1.m[2][2] * m2.m[2][0] + m1.m[2][3] * m2.m[3][0];
	result.m[2][1] = m1.m[2][0] * m2.m[0][1] + m1.m[2][1] * m2.m[1][1] + m1.m[2][2] * m2.m[2][1] + m1.m[2][3] * m2.m[3][1];
	result.m[2][2] = m1.m[2][0] * m2.m[0][2] + m1.m[2][1] * m2.m[1][2] + m1.m[2][2] * m2.m[2][2] + m1.m[2][3] * m2.m[3][2];
	result.m[2][3] = m1.m[2][0] * m2.m[0][3] + m1.m[2][1] * m2.m[1][3] + m1.m[2][2] * m2.m[2][3] + m1.m[2][3] * m2.m[3][3];
	result.m[3][0] = m1.m[3][0] * m2.m[0][0] + m1.m[3][1] * m2.m[1][0] + m1.m[3][2] * m2.m[2][0] + m1.m[3][3] * m2.m[3][0];
	result.m[3][1] = m1.m[3][0] * m2.m[0][1] + m1.m[3][1] * m2.m[1][1] + m1.m[3][2] * m2.m[2][1] + m1.m[3][3] * m2.m[3][1];
	result.m[3][2] = m1.m[3][0] * m2.m[0][2] + m1.m[3][1] * m2.m[1][2] + m1.m[3][2] * m2.m[2][2] + m1.m[3][3] * m2.m[3][2];
	result.m[3][3] = m1.m[3][0] * m2.m[0][3] + m1.m[3][1] * m2.m[1][3] + m1.m[3][2] * m2.m[2][3] + m1.m[3][3] * m2.m[3][3];

	return result;


}

// 正規化
Vector3 Normalize(const Vector3& v)
{
	float length = std::sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
	Vector3 result = { v.x / length, v.y / length, v.z / length };
	return result;
}

// 任意軸回転行列
Matrix4x4 MakeRotateAxisAngleMatrix(const Vector3& axis, float radian)
{
	Vector3 n = Normalize(axis);

	float cosTheta = std::cos(radian);
	float sinTheta = std::sin(radian);
	float oneMinusCos = 1.0f - cosTheta;

	Matrix4x4 result{};

	result.m[0][0] = n.x * n.x * oneMinusCos + cosTheta;
	result.m[0][1] = n.x * n.y * oneMinusCos + n.z * sinTheta;
	result.m[0][2] = n.x * n.z * oneMinusCos - n.y * sinTheta;
	result.m[0][3] = 0.0f;

	result.m[1][0] = n.y * n.x * oneMinusCos - n.z * sinTheta;
	result.m[1][1] = n.y * n.y * oneMinusCos + cosTheta;
	result.m[1][2] = n.y * n.z * oneMinusCos + n.x * sinTheta;
	result.m[1][3] = 0.0f;

	result.m[2][0] = n.z * n.x * oneMinusCos + n.y * sinTheta;
	result.m[2][1] = n.z * n.y * oneMinusCos - n.x * sinTheta;
	result.m[2][2] = n.z * n.z * oneMinusCos + cosTheta;
	result.m[2][3] = 0.0f;

	result.m[3][0] = 0.0f;
	result.m[3][1] = 0.0f;
	result.m[3][2] = 0.0f;
	result.m[3][3] = 1.0f;

	return result;
}


// 内積を求める
float Dot(const Vector3& v1, const Vector3& v2) {
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

// 外積を求める
Vector3 Cross(const Vector3& v1, const Vector3& v2) {
	return {
		v1.y * v2.z - v1.z * v2.y,
		v1.z * v2.x - v1.x * v2.z,
		v1.x * v2.y - v1.y * v2.x
	};
}

Matrix4x4 DirectionToDirection(const Vector3& from, const Vector3& to) {
	Vector3 f = Normalize(from);
	Vector3 t = Normalize(to);

	float dot = Dot(f, t);

	// from と to がほぼ同じ場合（回転不要）
	if (dot > 0.9999f) {
		Matrix4x4 identity{};
		for (int i = 0; i < 4; i++) identity.m[i][i] = 1.0f;
		return identity;
	}

	// 逆向きの場合（180度回転）
	if (dot < -0.9999f) {
		// どの軸でもいいが、from に垂直な軸を作る
		Vector3 ortho = { 1.0f, 0.0f, 0.0f };
		if (fabs(f.x) > 0.9f) ortho = { 0.0f, 1.0f, 0.0f };

		Vector3 axis = Normalize(Cross(f, ortho));
		return MakeRotateAxisAngleMatrix(axis, std::numbers::pi_v<float>);
	}

	// 通常ケース
	Vector3 axis = Normalize(Cross(f, t));
	float angle = std::acos(dot);

	return MakeRotateAxisAngleMatrix(axis, angle);
}

// Quaternionの積
Quaternion Multiply(const Quaternion& q1, const Quaternion& q2)
{
	Quaternion result;

	result.x = q1.w * q2.x + q1.x * q2.w + q1.y * q2.z - q1.z * q2.y;
	result.y = q1.w * q2.y - q1.x * q2.z + q1.y * q2.w + q1.z * q2.x;
	result.z = q1.w * q2.z + q1.x * q2.y - q1.y * q2.x + q1.z * q2.w;
	result.w = q1.w * q2.w - q1.x * q2.x - q1.y * q2.y - q1.z * q2.z;

	return result;
}


Quaternion IdentityQuaternion()
{
	return { 0.0f, 0.0f, 0.0f, 1.0f };
}


// 共役Quaternion
Quaternion Conjugate(const Quaternion& q)
{
	Quaternion result;
	result.x = -q.x;
	result.y = -q.y;
	result.z = -q.z;
	result.w = q.w;
	return result;
}

// Quaternionの長さ
float Norm(const Quaternion& q)
{
	return std::sqrt(q.x * q.x + q.y * q.y + q.z * q.z + q.w * q.w);
}


// Quaternionを正規化
Quaternion Normalize(const Quaternion& q)
{
	float n = Norm(q);
	Quaternion result;
	result.x = q.x / n;
	result.y = q.y / n;
	result.z = q.z / n;
	result.w = q.w / n;
	return result;
}


// 逆Quaternion
Quaternion Inverse(const Quaternion& q)
{
	float n = Norm(q);
	Quaternion conj = Conjugate(q);

	Quaternion result;
	result.x = conj.x / (n * n);
	result.y = conj.y / (n * n);
	result.z = conj.z / (n * n);
	result.w = conj.w / (n * n);

	return result;
}


static const int kRowHeight = 20;
static const int kColumnWidth = 60;

void MatrixScreenPrintf(int x, int y, Matrix4x4 matrix) {
	for (int row = 0; row < 4; ++row) {
		for (int column = 0; column < 4; ++column) {
			Novice::ScreenPrintf(x + column * kColumnWidth, y + row * kRowHeight, "%6.03f", matrix.m[row][column]);
		}
	}
}

Vector3 operator+(const Vector3& v1, const Vector3& v2) { return Add(v1, v2); }
Vector3 operator-(const Vector3& v1, const Vector3& v2) { return Subtract(v1, v2); }
Vector3 operator* (float s, const Vector3& v) { return Multiply(s, v); }
Vector3 operator* (const Vector3& v, float s) { return s * v; }
Vector3 operator/(const Vector3& v, float s) { return Multiply(1.0f / s, v); }
Matrix4x4 operator+(const Matrix4x4& m1, const Matrix4x4& m2) { return Add(m1, m2); }
Matrix4x4 operator-(const Matrix4x4& m1, const Matrix4x4& m2) { return Subtract(m1, m2); }
Matrix4x4 operator*(const Matrix4x4& m1, const Matrix4x4& m2) { return Multiply(m1, m2); }


// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1280, 720);


	Quaternion q1{ 2.0f, 3.0f, 4.0f, 1.0f }; // Y軸90度回転
	Quaternion q2{ 1.0f, 3.0f, 5.0f, 2.0f }; // X軸90度回転

	Quaternion identity = IdentityQuaternion();

	Quaternion conj = Conjugate(q1);

	Quaternion inverse = Inverse(q1);

	Quaternion normal = Normalize(q1);

	Quaternion mul1 = Multiply(q1, q2);

	Quaternion mul2 = Multiply(q2, q1);

	float norm = Norm(q1);

	// キー入力結果を受け取る箱
	char keys[256] = { 0 };
	char preKeys[256] = { 0 };

	

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





		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		Novice::ScreenPrintf(0, 0, "%.2f,%.2f,%.2f,%.2f	:Identity", identity.x, identity.y, identity.z, identity.w);
		Novice::ScreenPrintf(0, 20, " %.2f, %.2f, %.2f, %.2f	:Conjugate", conj.x, conj.y, conj.z, conj.w);
		Novice::ScreenPrintf(0, 40, "%.2f, %.2f, %.2f, %.2f	:Inverse", inverse.x, inverse.y, inverse.z, inverse.w);
		Novice::ScreenPrintf(0, 60, "%.2f, %.2f, %.2f, %.2f	:Normalize", normal.x, normal.y, normal.z, normal.w);
		Novice::ScreenPrintf(0, 80, "%.2f, %.2f, %.2f, %.2f	:Multiply(q1, q2)", mul1.x, mul1.y, mul1.z, mul1.w);
		Novice::ScreenPrintf(0, 100, "%.2f, %.2f, %.2f, %.2f	:Multiply(q2, q1)", mul1.x, mul1.y, mul1.z, mul1.w);
		Novice::ScreenPrintf(0, 120, "%.2f	:Norm", norm);

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