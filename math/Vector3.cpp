#include "Vector3.h"
#include <cmath>
//#include "MathUtility.h"
//
//using namespace MathUtility;
float Vector3::length() const { return std::sqrt(x * x + y * y + z * z); }

Vector3& Vector3::normalize() {
	float len = length();
	if (len != 0) {
		return *this /= len;
	}
	return *this;
}

Vector3 Vector3::vector3Normalize(Vector3& v) {
	float x, y, z, w;

	w = std::sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
	x = v.x / w;
	y = v.y / w;
	z = v.z / w;

	return Vector3(x, y, z);
}

float Vector3::dot(const Vector3& v) const { return x * v.x + y * v.y; }

Vector3 Vector3::cross(const Vector3& v) const {
	return Vector3(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);
}

Vector3 Vector3::SphereLinear(Vector3& out, Vector3& start, Vector3& end, float t)
{

	Vector3 s, e;
	start.normalize();
	end.normalize();
	s = start;
	e = end;

	// 2�x�N�g���Ԃ̊p�x�i�s�p���j
	float angle = 0.0f;
	angle = acos(s.dot(e));
	float SinTh = sin(angle);

	// ��ԌW��
	float Ps = sin(angle * (1 - t));
	float Pe = sin(angle * t);

	/*out = (s * Ps+ Pe * e) / SinTh;*/
	out = { (s.x * Ps) + (e.x * Ps),(s.y * Ps) + (e.y * Ps),(s.z * Ps) + (e.z * Ps) };
	// �ꉞ���K�����ċ��ʐ��`��Ԃ�
	out.normalize();

	return out;

}


// Vector2 �N���X�ɑ����Ȃ��֐��Q
// 2�����Z�q
const Vector3 operator+(const Vector3& v1, const Vector3& v2) {
	Vector3 temp(v1);
	return temp += v2;
}

const Vector3 operator-(const Vector3& v1, const Vector3& v2) {
	Vector3 temp(v1);
	return temp -= v2;
}

const Vector3 operator*(const Vector3& v, float s) {
	Vector3 temp(v);
	return temp * s;
}

const Vector3 operator*(float s, const Vector3& v) { 
	Vector3 temp(v);
	return v * s;
}

const Vector3 operator/(const Vector3& v, float s) {
	Vector3 temp(v);
	return temp / s;
}
