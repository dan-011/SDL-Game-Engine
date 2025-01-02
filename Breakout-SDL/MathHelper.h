#pragma once
#include <SDL.h>

#define FLOAT_PAD 0.0001f

float FAbs(float val);

bool FEq(float a, float b);

int Abs(int val);

template <typename T>
T Min(T a, T b) {
	if (a < b) return a;
	else return b;
}

template <typename T>
T Max(T a, T b) {
	if (a > b) return a;
	else return b;
}

int FlipSign(int val);

Uint32 FlipSign(Uint32 val);

float FlipSign(float val);

float Interpolate(float goal, float cur, Uint32 approach);

float Determinant(float aX, float aY, float bX, float bY);
