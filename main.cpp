#include <Novice.h>
#include <math.h>



struct Quaternion
{
	float x;
	float y;
	float z;
	float w;
};

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



Vector3 Normalize(const Vector3& v)
{
	Vector3 result;

	result.x = v.x / sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
	result.y = v.y / sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
	result.z = v.z / sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);

	return result;

}

//座標変換
Vector3 Transform(const Vector3& vector, const Matrix4x4& matrix)
{
	Vector3 result;

	result.x = vector.x * matrix.m[0][0] + vector.y * matrix.m[1][0] + vector.z * matrix.m[2][0] + 1.0f * matrix.m[3][0];
	result.y = vector.x * matrix.m[0][1] + vector.y * matrix.m[1][1] + vector.z * matrix.m[2][1] + 1.0f * matrix.m[3][1];
	result.z = vector.x * matrix.m[0][2] + vector.y * matrix.m[1][2] + vector.z * matrix.m[2][2] + 1.0f * matrix.m[3][2];
	float w = vector.x * matrix.m[0][3] + vector.y * matrix.m[1][3] + vector.z * matrix.m[2][3] + 1.0f * matrix.m[3][3];

	result.x /= w;
	result.y /= w;
	result.z /= w;

	return result;
}

//内積
float Dot(const Vector3& v1, const Vector3& v2)
{
	float result;

	result = v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;

	return result;

}

//外積
Vector3 Cross(const Vector3& v1, const Vector3& v2)
{
	Vector3 result;

	result.x = (v1.y * v2.z - v1.z * v2.y);
	result.y = (v1.z * v2.x - v1.x * v2.z);
	result.z = (v1.x * v2.y - v1.y * v2.x);

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

// 加算
Vector3 Add(const Vector3& v1, const Vector3& v2)
{
	Vector3 result;

	result.x = v1.x + v2.x;
	result.y = v1.y + v2.y;
	result.z = v1.z + v2.z;

	return result;

}

Vector3 operator+(const Vector3& v1, const Vector3& v2) { return Add(v1, v2); }

//Quternionの積
Quaternion Multiply(const Quaternion& lhs, const Quaternion& rhs)
{
	Quaternion result;

	Vector3 lhsv = { lhs.x,lhs.y,lhs.z };
	float lhsw = lhs.w;

	Vector3 rhsv = { rhs.x,rhs.y,rhs.z };
	float rhsw = rhs.w;

	result.w = (lhsw * rhsw) - Dot(lhsv, rhsv);

	Vector3 v = Cross(lhsv, rhsv) + Multiply(rhsw, lhsv) + Multiply(lhsw, rhsv);

	result.x = v.x;
	result.y = v.y;
	result.z = v.z;

	return result;
}


//任意回転軸を表すQuaternionの生成
Quaternion MakeRotateAxisAngleQuaternion(const Vector3& axis, float angle)
{
	Quaternion q;


	q.x = axis.x * sinf(angle / 2);
	q.y = axis.y * sinf(angle / 2);
	q.z = axis.z * sinf(angle / 2);
	q.w = cosf(angle / 2);


	return q;

}

//ベクトルをQuaternionで回転させた結果のベクトルを求める
Vector3 RotateVector(const Vector3& v, const Quaternion& q) {

	Vector3 result;

	Quaternion r = { v.x, v.y, v.z, 0.0f };
	Quaternion qc = { -q.x, -q.y, -q.z, q.w };


	Quaternion qr;
	qr.w = q.w * r.w - q.x * r.x - q.y * r.y - q.z * r.z;
	qr.x = q.w * r.x + q.x * r.w + q.y * r.z - q.z * r.y;
	qr.y = q.w * r.y - q.x * r.z + q.y * r.w + q.z * r.x;
	qr.z = q.w * r.z + q.x * r.y - q.y * r.x + q.z * r.w;




	result.x = qr.w * qc.x + qr.x * qc.w + qr.y * qc.z - qr.z * qc.y;
	result.y = qr.w * qc.y - qr.x * qc.z + qr.y * qc.w + qr.z * qc.x;
	result.z = qr.w * qc.z + qr.x * qc.y - qr.y * qc.x + qr.z * qc.w;


	return { result.x, result.y, result.z };
}

//Quaternionから回転行列を求める
Matrix4x4 MakeRotateMatrix(const Quaternion& quaternion)
{
	Matrix4x4 result;

	Quaternion q = quaternion;

	result.m[0][0] = q.w * q.w + q.x * q.x - q.y * q.y - q.z * q.z;
	result.m[0][1] = 2 * (q.x * q.y + q.w * q.z);
	result.m[0][2] = 2 * (q.x * q.z - q.w * q.y);
	result.m[0][3] = 0.0f;

	result.m[1][0] = 2 * (q.x * q.y - q.w * q.z);
	result.m[1][1] = q.w * q.w - q.x * q.x + q.y * q.y - q.z * q.z;
	result.m[1][2] = 2 * (q.y * q.z + q.w * q.x);
	result.m[1][3] = 0.0f;

	result.m[2][0] = 2 * (q.x * q.z + q.w * q.y);
	result.m[2][1] = 2 * (q.y * q.z - q.w * q.x);
	result.m[2][2] = q.w * q.w - q.x * q.x - q.y * q.y + q.z * q.z;;
	result.m[2][3] = 0.0f;

	result.m[3][0] = 0.0f;
	result.m[3][1] = 0.0f;
	result.m[3][2] = 0.0f;
	result.m[3][3] = 1.0f;

	return result;
}


static const int kColumnWidth = 60;
static const int kRowHeight = 20;
void QuaternionScreenPrintf(int x, int y, const Quaternion& quaternion, const char* label)
{
	Novice::ScreenPrintf(x, y, "%.02f", quaternion.x);
	Novice::ScreenPrintf(x + kColumnWidth, y, "%.02f", quaternion.y);
	Novice::ScreenPrintf(x + kColumnWidth * 2, y, "%.02f", quaternion.z);
	Novice::ScreenPrintf(x + kColumnWidth * 3, y, "%.02f", quaternion.w);
	Novice::ScreenPrintf(x + kColumnWidth * 4, y, "%s", label);
}

void MatrixScreenPrintf(int x, int y, const Matrix4x4& matrix, const char* label)
{
	Novice::ScreenPrintf(x, y, "%s\n", label);
	for (int row = 0; row < 4; ++row)
	{
		for (int column = 0; column < 4; ++column)
		{
			Novice::ScreenPrintf(x + column * kColumnWidth, y + row * kRowHeight + kRowHeight, "%6.03f", matrix.m[row][column]);
		}
	}
}

void VectorScreenPrintf(int x, int y, const Vector3& vector, const char* label)
{
	Novice::ScreenPrintf(x, y, "%.02f", vector.x);
	Novice::ScreenPrintf(x + kColumnWidth, y, "%.02f", vector.y);
	Novice::ScreenPrintf(x + kColumnWidth * 2, y, "%.02f", vector.z);
	Novice::ScreenPrintf(x + kColumnWidth * 3, y, "%s", label);
}


const char kWindowTitle[] = "LE2C_20_タニタ_カイセイ";

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1280, 720);

	Quaternion rotation = MakeRotateAxisAngleQuaternion(
		Normalize(Vector3{ 1.0f,0.4f,-0.2f }), 0.45f);
	Vector3 pointY = { 2.1f,-0.9f,1.3f, };
	Matrix4x4 rotateMatrix = MakeRotateMatrix(rotation);
	Vector3 rotateByQuaternion = RotateVector(pointY, rotation);
	Vector3 rotateByMatrix = Transform(pointY, rotateMatrix);



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

		QuaternionScreenPrintf(0, 0, rotation, " : rotation");
		MatrixScreenPrintf(0, kRowHeight * 1, rotateMatrix, "rotateMatrix");
		VectorScreenPrintf(0, kRowHeight * 6, rotateByQuaternion, " : rotateByQuaternion");
		VectorScreenPrintf(0, kRowHeight * 7, rotateByMatrix, " : rotateByMatrix");


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