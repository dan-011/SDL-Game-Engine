#pragma once
struct Vector2i {
	Vector2i() {}
	Vector2i(int _x, int _y) : x(_x), y(_y) {}
	Vector2i(const Vector2i& other) : x(other.x), y(other.y) {}
	Vector2i(Vector2i&& other) noexcept : x(other.x), y(other.y) {
		other.x = -1;
		other.y = -1;
	}

	int x = 0;
	int y = 0;

	Vector2i& operator=(const Vector2i& other) {
		if (this == &other) return *this;
		x = other.x;
		y = other.y;
		return *this;
	}

	Vector2i& operator=(Vector2i&& other) noexcept {
		if (this == &other) return *this;
		x = other.x;
		y = other.y;
		other.x = -1;
		other.y = -1;
		return *this;
	}

	Vector2i operator+(const Vector2i& other) {
		return Vector2i(x + other.x, y + other.y);
	}

	Vector2i operator-(const Vector2i& other) {
		return Vector2i(x - other.x, y - other.y);
	}

	bool operator==(const Vector2i& other) {
		return x == other.x && y == other.y;
	}

	long SqDistance() {
		return ((long)x * x) + ((long)y * y);
	}

	long SqDistance(const Vector2i& other) {
		return (*this - other).SqDistance();
	}



	// support trig operators
};