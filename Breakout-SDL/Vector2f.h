#pragma once
#include "MathHelper.h"

struct Vector2f {
	Vector2f() {}
	Vector2f(float _x, float _y) : x(_x), y(_y) {}
	Vector2f(const Vector2f& other) : x(other.x), y(other.y) {}
	Vector2f(Vector2f&& other) noexcept : x(other.x), y(other.y) {
		other.x = -1;
		other.y = -1;
	}

	float x = 0;
	float y = 0;

	Vector2f& operator=(const Vector2f& other) {
		if (this == &other) return *this;
		x = other.x;
		y = other.y;
		return *this;
	}

	Vector2f& operator=(Vector2f&& other) noexcept {
		if (this == &other) return *this;
		x = other.x;
		y = other.y;
		other.x = -1;
		other.y = -1;
		return *this;
	}

	Vector2f operator+(const Vector2f& other) const {
		return Vector2f(x + other.x, y + other.y);
	}

	Vector2f operator-(const Vector2f& other) const {
		return Vector2f(x - other.x, y - other.y);
	}

	bool operator==(const Vector2f& other) {
		return FAbs(x - other.x) < FLOAT_PAD && FAbs(y - other.y) < FLOAT_PAD;
	}

	double SqDistance() {
		return ((double)x * x) + ((double)y * y);
	}

	double SqDistance(const Vector2f& other) {
		return (*this - other).SqDistance();
	}

	double Dot(const Vector2f& other) {
		return ((double)x * other.x) + ((double)y * other.y);
	}


	// support trig operators
};