#pragma once

#include "pch.h"

template<typename T>
concept is_vec2 = requires(T vec) { vec.x, vec.y; }
					&& sizeof(float) * 2 == sizeof(T)
					&& std::constructible_from<float, float>;

template<typename T>
concept is_vec3 = requires(T vec) { vec.x, vec.y, vec.z; }
					&& sizeof(float) * 3 == sizeof(T) && !is_vec2<T>
					&& std::constructible_from<float, float, float>;

template<typename T>
concept is_vec4 = requires(T vec) { vec.x, vec.y, vec.z, vec.w; }
					&& sizeof(float) * 4 == sizeof(T) && !is_vec3<T>
					&& std::constructible_from<float, float, float, float>;

float clamp(float value, float min, float max) {
	return std::max(min, std::min(max, value));
}

float toDegree(float x) {
	return x / 180 * (float)std::numbers::pi;
}

float toAngle(float x) {
	return x * 180 / (float)std::numbers::pi;
}

struct vec2_t {

	float x, y;

	vec2_t() = default;
	constexpr vec2_t(float x, float y) : x(x), y(y) {};

	template<typename T> requires is_vec2<T>
	constexpr vec2_t(T vec) : vec2_t(vec.x, vec.y) {};

	template<typename T> requires is_vec2<T>
	constexpr operator T() const noexcept {
		return { x, y };
	}

	std::string to_string() {
		return std::format("{:.2f}, {:.2f}", x, y);
	}
};

struct vec3_t {

	float x, y, z;

	vec3_t() = default;
	constexpr vec3_t(float x, float y, float z) : x(x), y(y), z(z) {}

	template<typename T> requires is_vec3<T>
	constexpr vec3_t(T vec) : vec3_t(vec.x, vec.y, vec.z) {};

	template<typename T> requires is_vec3<T>
	constexpr operator T() const noexcept {
		return { x, y, z };
	}

	constexpr vec3_t operator+(vec3_t& other) const {
		vec3_t Temp = *this;
		Temp += other;
		return Temp;
	}

	constexpr vec3_t& operator+=(vec3_t& other) {
		x += other.x;
		y += other.y;
		z += other.z;
		return *this;
	}

	constexpr float& operator[](const std::ptrdiff_t index) {

		switch (index) {
		case 0: return x;
		case 1: return y;
		case 2: return z;
		default: throw std::out_of_range("Out of Bound vec3_t index");
		}
	}

	constexpr float operator[](const std::ptrdiff_t index) const {
		return this->operator[](index);
	}
	
	std::string to_string() {
		return std::format("{:.2f}, {:.2f}, {:.2f}", x, y, z);
	}
};

struct vec4_t {

	float x, y, z, w;

	vec4_t() = default;
	constexpr vec4_t(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}

	template<typename T> requires is_vec4<T>
	constexpr vec4_t(T vec) : vec4_t(vec.x, vec.y, vec.z, vec.w) {};

	template<typename T> requires is_vec4<T>
	constexpr operator T() const noexcept {
		return { x, y, z, w };
	}

	constexpr float& operator[](const std::ptrdiff_t index) {

		switch (index) {
			case 0: return x;
			case 1: return y;
			case 2: return z;
			case 3: return w;
			default: throw std::out_of_range("Out of Bound vec4_t index");
		}
	}

	constexpr float operator[](const std::ptrdiff_t index) const {
		return this->operator[](index);
	}
};

struct mat3x3_t {

	vec3_t r1, r2, r3;

	constexpr vec3_t& operator[](const std::ptrdiff_t index) {

		switch (index) {
			case 0: return r1;
			case 1: return r2;
			case 2: return r3;
			default: throw std::out_of_range("Out of Bound mat3x3_t index");
		}
	}

	constexpr vec3_t operator[](const std::ptrdiff_t index) const {
		return this->operator[](index);
	}

	std::string to_string() {
		return std::format("[{}]\n[{}]\n[{}]", r1.to_string(), r2.to_string(), r3.to_string());
	}
};

struct mat4x4_t {

	vec4_t r1, r2, r3, r4;

	constexpr vec4_t& operator[](const std::ptrdiff_t index) {

		switch (index) {
			case 0: return r1;
			case 1: return r2;
			case 2: return r3;
			case 3: return r4;
			default: throw std::out_of_range("Out of Bound mat4x4_t index");
		}
	}

	constexpr vec4_t operator[](const std::ptrdiff_t index) const {
		return this->operator[](index);
	}

	mat4x4_t operator*(mat4x4_t& other) {

		mat4x4_t lhs = *this;
		mat4x4_t rhs = other;

		mat4x4_t Temp = {};

		Temp[0][0] = lhs[0][0] * rhs[0][0] + lhs[0][1] * rhs[1][0] + lhs[0][2] * rhs[2][0] + lhs[0][3] * rhs[3][0];
		Temp[0][1] = lhs[0][0] * rhs[0][1] + lhs[0][1] * rhs[1][1] + lhs[0][2] * rhs[2][1] + lhs[0][3] * rhs[3][1];
		Temp[0][2] = lhs[0][0] * rhs[0][2] + lhs[0][1] * rhs[1][2] + lhs[0][2] * rhs[2][2] + lhs[0][3] * rhs[3][2];
		Temp[0][3] = lhs[0][0] * rhs[0][3] + lhs[0][1] * rhs[1][3] + lhs[0][2] * rhs[2][3] + lhs[0][3] * rhs[3][3];

		Temp[1][0] = lhs[1][0] * rhs[0][0] + lhs[1][1] * rhs[1][0] + lhs[1][2] * rhs[2][0] + lhs[1][3] * rhs[3][0];
		Temp[1][1] = lhs[1][0] * rhs[0][1] + lhs[1][1] * rhs[1][1] + lhs[1][2] * rhs[2][1] + lhs[1][3] * rhs[3][1];
		Temp[1][2] = lhs[1][0] * rhs[0][2] + lhs[1][1] * rhs[1][2] + lhs[1][2] * rhs[2][2] + lhs[1][3] * rhs[3][2];
		Temp[1][3] = lhs[1][0] * rhs[0][3] + lhs[1][1] * rhs[1][3] + lhs[1][2] * rhs[2][3] + lhs[1][3] * rhs[3][3];

		Temp[2][0] = lhs[2][0] * rhs[0][0] + lhs[2][1] * rhs[1][0] + lhs[2][2] * rhs[2][0] + lhs[2][3] * rhs[3][0];
		Temp[2][1] = lhs[2][0] * rhs[0][1] + lhs[2][1] * rhs[1][1] + lhs[2][2] * rhs[2][1] + lhs[2][3] * rhs[3][1];
		Temp[2][2] = lhs[2][0] * rhs[0][2] + lhs[2][1] * rhs[1][2] + lhs[2][2] * rhs[2][2] + lhs[2][3] * rhs[3][2];
		Temp[2][3] = lhs[2][0] * rhs[0][3] + lhs[2][1] * rhs[1][3] + lhs[2][2] * rhs[2][3] + lhs[2][3] * rhs[3][3];

		Temp[3][0] = lhs[3][0] * rhs[0][0] + lhs[3][1] * rhs[1][0] + lhs[3][2] * rhs[2][0] + lhs[3][3] * rhs[3][0];
		Temp[3][1] = lhs[3][0] * rhs[0][1] + lhs[3][1] * rhs[1][1] + lhs[3][2] * rhs[2][1] + lhs[3][3] * rhs[3][1];
		Temp[3][2] = lhs[3][0] * rhs[0][2] + lhs[3][1] * rhs[1][2] + lhs[3][2] * rhs[2][2] + lhs[3][3] * rhs[3][2];
		Temp[3][3] = lhs[3][0] * rhs[0][3] + lhs[3][1] * rhs[1][3] + lhs[3][2] * rhs[2][3] + lhs[3][3] * rhs[3][3];

		return Temp;
	}
};
// 99% it's not working coz of copy-pasting
struct Quaternion : vec4_t {

	Quaternion() : vec4_t(0.f, 0.f, 0.f, 1.f) {}

	float length() {
		return std::sqrt(x * x + y * y + z * z + w * w);
	}

	void normalize() {

		float length = this->length();

		if (length == 0) {
			x, y, z = {};
			w = 1.f;
		}
		else {

			float divLength = 1.f / length;

			x *= divLength;
			y *= divLength;
			z *= divLength;
			w *= divLength;
		}
	}

	void from_matrix(mat3x3_t& matrix) {

		float trace = matrix[0][0] + matrix[1][1] + matrix[2][2];

		if (trace > 0.f) {

			const float s = 0.5f / std::sqrt(trace + 1.0f);

			w = 0.25f / s;
			x = (matrix[1][2] - matrix[2][1]) * s;
			y = (matrix[2][0] - matrix[0][2]) * s;
			z = (matrix[0][1] - matrix[1][0]) * s;

		}
		else if (matrix[0][0] > matrix[1][1] && matrix[0][0] > matrix[2][2]) {

			const float s = 2.0f * std::sqrt(1.0f + matrix[0][0] - matrix[1][1] - matrix[2][2]);

			w = (matrix[1][2] - matrix[2][1]) / s;
			x = 0.25f * s;
			y = (matrix[1][0] + matrix[0][1]) / s;
			z = (matrix[2][0] + matrix[0][2]) / s;

		}
		else if (matrix[1][1] > matrix[2][2]) {

			const float s = 2.0f * std::sqrt(1.0f + matrix[1][1] - matrix[0][0] - matrix[2][2]);

			w = (matrix[2][0] - matrix[0][2]) / s;
			x = (matrix[1][0] + matrix[0][1]) / s;
			y = 0.25f * s;
			z = (matrix[2][1] + matrix[1][2]) / s;

		}
		else {

			const float s = 2.0f * std::sqrt(1.0f + matrix[2][2] - matrix[0][0] - matrix[1][1]);

			w = (matrix[0][1] - matrix[1][0]) / s;
			x = (matrix[2][0] + matrix[0][2]) / s;
			y = (matrix[2][1] + matrix[1][2]) / s;
			z = 0.25f * s;

		}
	}
};
// 99% it's not working coz of copy-pasting
struct rotmat : mat3x3_t {

	void from_quaternion(Quaternion q) {

		auto& matrix = *this;

		vec3_t scale = { 1.f, 1.f, 1.f };

		const float x = q.x, y = q.y, z = q.z, w = q.w;
		const float x2 = x + x, y2 = y + y, z2 = z + z;
		const float xx = x * x2, xy = x * y2, xz = x * z2;
		const float yy = y * y2, yz = y * z2, zz = z * z2;
		const float wx = w * x2, wy = w * y2, wz = w * z2;

		const float sx = scale.x, sy = scale.y, sz = scale.z;

		matrix[0][0] = (1.f - (yy + zz)) * sx;
		matrix[0][1] = (xy + wz) * sx;
		matrix[0][2] = (xz - wy) * sx;

		matrix[1][0] = (xy - wz) * sy;
		matrix[1][1] = (1.f - (xx + zz)) * sy;
		matrix[1][2] = (yz + wx) * sy;

		matrix[2][0] = (xz + wy) * sz;
		matrix[2][1] = (yz - wx) * sz;
		matrix[2][2] = (1.f - (xx + yy)) * sz;
	}

	vec3_t getAngles() {

		auto& matrix = *this;

		Quaternion quanterion;
		quanterion.from_matrix(matrix);
		quanterion.normalize();

		matrix.from_quaternion(quanterion);

		vec3_t angles;

		angles.y = std::asin(clamp(matrix[2][0], -1, 1));

		if (std::abs(matrix[2][0]) < 0.9999999) {
			angles.x = std::atan2(-matrix[2][1], matrix[2][2]);
			angles.z = std::atan2(-matrix[1][0], matrix[0][0]);
		}
		else {
			angles.x = std::atan2(matrix[1][2], matrix[1][1]);
			angles.z = 0;
		}

		angles.x = toAngle(angles.x);
		angles.y = toAngle(angles.y);
		angles.z = toAngle(angles.z);

		return angles;
	}
};

std::optional<vec2_t> WorldToScreen(vec3_t position, mat4x4_t matrix, vec2_t screenResolution, bool rowMajor = true)
{
	//Matrix-vector Product, multiplying world(eye) coordinates by projection matrix = clipCoords
	vec4_t clipCoords;

	if (rowMajor) {

		clipCoords.x = position.x * matrix[0][0] + position.y * matrix[0][1] + position.z * matrix[0][2] + matrix[0][3];
		clipCoords.y = position.x * matrix[1][0] + position.y * matrix[1][1] + position.z * matrix[1][2] + matrix[1][3];
		clipCoords.z = position.x * matrix[2][0] + position.y * matrix[2][1] + position.z * matrix[2][2] + matrix[2][3];
		clipCoords.w = position.x * matrix[3][0] + position.y * matrix[3][1] + position.z * matrix[3][2] + matrix[3][3];
	}
	else {

		clipCoords.x = position.x * matrix[0][0] + position.y * matrix[1][0] + position.z * matrix[2][0] + matrix[3][0];
		clipCoords.y = position.x * matrix[0][1] + position.y * matrix[1][1] + position.z * matrix[2][1] + matrix[3][1];
		clipCoords.z = position.x * matrix[0][2] + position.y * matrix[1][2] + position.z * matrix[2][2] + matrix[3][2];
		clipCoords.w = position.x * matrix[0][3] + position.y * matrix[1][3] + position.z * matrix[2][3] + matrix[3][3];
	}


	if (clipCoords.w < 0.1f)
		return std::nullopt;

	//perspective division, dividing by clip.W = Normalized Device Coordinates
	vec3_t NDC;
	NDC.x = clipCoords.x / clipCoords.w;
	NDC.y = clipCoords.y / clipCoords.w;
	NDC.z = clipCoords.z / clipCoords.w;

	vec2_t screenPos;
	screenPos.x = (screenResolution.x / 2 * NDC.x) + (NDC.x + screenResolution.x / 2);
	screenPos.y = -(screenResolution.y / 2 * NDC.y) + (NDC.y + screenResolution.y / 2);

	return screenPos;
}
