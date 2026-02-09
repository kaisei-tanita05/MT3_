#include <Novice.h>
#include <math.h>

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

Vector3 operator-(const Vector3& v) { return { -v.x, -v.y, -v.z }; }
//Vector3 operator+(const Vector3& v) {return v; }

Vector3 Normalize(const Vector3& v)
{
	Vector3 result;

	result.x = v.x / sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
	result.y = v.y / sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
	result.z = v.z / sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);

	return result;

}

float Dot(const Vector3& v1, const Vector3& v2)
{
	float result;

	result = v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;

	return result;

}

Vector3 Cross(const Vector3& v1, const Vector3& v2)
{
	Vector3 result;

	result.x = (v1.y * v2.z - v1.z * v2.y);
	result.y = (v1.z * v2.x - v1.x * v2.z);
	result.z = (v1.x * v2.y - v1.y * v2.x);

	return result;
}

Matrix4x4 DirectionToDirection(const Vector3& from, const Vector3& to)
{

	Matrix4x4 result{};

	Vector3 u = Normalize(from);
	Vector3 v = Normalize(to);


	Vector3 cross = Cross(u, v);
	float cos = Dot(u, v);
	float sin = sqrtf(Dot(cross, cross));


	if (cos < -0.9999f) {
		Vector3 ortho;
		if (fabs(u.x) < fabs(u.y) && fabs(u.x) < fabs(u.z))
			ortho = { 1, 0, 0 };
		else if (fabs(u.y) < fabs(u.z))
			ortho = { 0, 1, 0 };
		else
			ortho = { 0, 0, 1 };
		cross = Normalize(Cross(u, ortho));
		sin = 0.0f;
		cos = -1.0f;
	}
	else {
		cross = Normalize(cross);
	}

	Vector3 n = Normalize(cross);


	result.m[0][0] = (n.x * n.x) * (1 - cos) + cos;
	result.m[0][1] = (n.x * n.y) * (1 - cos) + (n.z * sin);
	result.m[0][2] = (n.x * n.z) * (1 - cos) - (n.y * sin);
	result.m[0][3] = 0.0f;

	result.m[1][0] = (n.x * n.y) * (1 - cos) - (n.z * sin);
	result.m[1][1] = (n.y * n.y) * (1 - cos) + cos;
	result.m[1][2] = (n.y * n.z) * (1 - cos) + (n.x * sin);
	result.m[1][3] = 0.0f;

	result.m[2][0] = (n.x * n.z) * (1 - cos) + (n.y * sin);
	result.m[2][1] = (n.y * n.z) * (1 - cos) - (n.x * sin);
	result.m[2][2] = (n.z * n.z) * (1 - cos) + cos;
	result.m[2][3] = 0.0f;


	result.m[3][0] = 0.0f;
	result.m[3][1] = 0.0f;
	result.m[3][2] = 0.0f;
	result.m[3][3] = 1.0f;

	return result;


}





static const int kRowHeight = 20;
static const int kColumnWidth = 60;
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

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1280, 720);

	Vector3 from0 = { 1.0f,0.7f,0.5f };
	Vector3 to0 = -from0;
	Vector3 from1 = { -0.6f,0.9f,0.2f };
	Vector3 to1 = { 0.4f,0.7f,-0.5f };



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

		Matrix4x4 rotateMatrix0 = DirectionToDirection(Vector3{ 1.0f,0.0f,0.0f }, Vector3{ -1.0f,0.0f,0.0f });
		Matrix4x4 rotateMatrix1 = DirectionToDirection(from0, to0);
		Matrix4x4 rotateMatrix2 = DirectionToDirection(from1, to1);


		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		MatrixScreenPrintf(0, 0, rotateMatrix0, "rotateMatrix0");
		MatrixScreenPrintf(0, kRowHeight * 5, rotateMatrix1, "rotateMatrix1");
		MatrixScreenPrintf(0, kRowHeight * 10, rotateMatrix2, "rotateMatrix2");


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