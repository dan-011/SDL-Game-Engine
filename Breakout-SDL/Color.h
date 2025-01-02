#pragma once
#include <SDL.h>

struct Color {
	Color() {}
	Color(Uint8 r, Uint8 g, Uint8 b, Uint8 a = 0xFF) : R(r), G(g), B(b), A(a) {}
	Color(const Color& other) : R(other.R), G(other.G), B(other.B), A(other.A) {}
	Color(Color&& other) noexcept : R(other.R), G(other.G), B(other.B), A(other.A) {
		other.R = 0x00;
		other.G = 0x00;
		other.B = 0x00;
		other.A = 0xFF;
	}

	Color& operator=(const Color& other) {
		if (this == &other) return *this;
		R = other.R;
		G = other.G;
		B = other.B;
		A = other.A;
		return *this;
	}

	Color& operator=(Color&& other) noexcept {
		if (this == &other) return *this;
		R = other.R;
		G = other.G;
		B = other.B;
		A = other.A;

		other.R = 0x00;
		other.G = 0x00;
		other.B = 0x00;
		other.A = 0xFF;
		return *this;
	}

	Uint8 R = 0x00;
	Uint8 G = 0x00;
	Uint8 B = 0x00;
	Uint8 A = 0xFF;

};