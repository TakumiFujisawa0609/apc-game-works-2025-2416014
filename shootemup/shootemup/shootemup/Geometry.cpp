#include "Geometry.h"
#include <DxLib.h>
#include <cmath>

// 四角形の描画
void Rect::Draw() {
	DxLib::DrawBox(Left() * 2, Top() * 2, Right() * 2, Bottom() * 2, 0xffffffff, false);
}

void Rect::Draw(Vector2& offset) {
	DxLib::DrawBox(
		(Left() + offset.x) * 2,
		(Top() + offset.y) * 2,
		(Right() + offset.x) * 2,
		(Bottom() + offset.y) * 2,
		0xffffffff, false
	);
}

// *= スカラー倍（代入）
void Vector2::operator*=(float scale) {
	x *= scale;
	y *= scale;
}

// * スカラー倍（新しい Vector2 を返す）
Vector2 Vector2::operator*(float scale) {
	return Vector2(x * scale, y * scale);
}

// + ベクトル加算
Vector2 operator+(const Vector2& va, const Vector2 vb) {
	return Vector2(va.x + vb.x, va.y + vb.y);
}

// - ベクトル減算
Vector2 operator-(const Vector2& va, const Vector2 vb) {
	return Vector2(va.x - vb.x, va.y - vb.y);
}

// ベクトルの長さ（ノルム）
float Vector2::Length() const {
	return hypot(x, y);
}

// 正規化（自身を正規化）
void Vector2::Normalize() {
	float mag = Length();
	if (mag != 0.0f) {
		x /= mag;
		y /= mag;
	}
}

// 正規化（新しいベクトルを返す）
Vector2 Vector2::Normalized() {
	float mag = Length();
	if (mag != 0.0f) {
		return Vector2(x / mag, y / mag);
	}
	return Vector2(0.0f, 0.0f);
}

// 内積
float Dot(const Vector2& va, const Vector2& vb) {
	return va.x * vb.x + va.y * vb.y;
}

// 外積（2D の場合はスカラー値）
float Cross(const Vector2& va, const Vector2& vb) {
	return va.x * vb.y - va.y * vb.x;
}

// 内積演算子
float operator*(const Vector2& va, const Vector2& vb) {
	return Dot(va, vb);
}

// 外積演算子
float operator%(const Vector2& va, const Vector2& vb) {
	return Cross(va, vb);
}

// += ベクトル加算代入
void Vector2::operator+=(const Vector2& v) {
	x += v.x;
	y += v.y;
}

// -= ベクトル減算代入
void Vector2::operator-=(const Vector2& v) {
	x -= v.x;
	y -= v.y;
}