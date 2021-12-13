#pragma once

#define CLAMP(x) (x) = ((x) > 1)? 1 : (x)

struct Color {
	float r;
	float g;
	float b;

	Color() : r(1), g(1), b(1) {}
	Color(float r, float g, float b) : r(r), g(g), b(b) {}

	Color operator + (const Color& c) const {
		return Color{ r + c.r, g + c.g, b + c.b };
	}

	Color operator * (const float s) const {
		return Color{ s * r, s * g, s * b };
	}

	Color operator / (const float s) const {
		return Color{ r / s, g / s, b / s };
	}

	Color operator * (const Color& c) const {
		return Color{ r * c.r, g * c.g, b * c.b };
	}

	friend Color operator * (const float s, const Color& c) {
		return Color{ s * c.r, s * c.g, s * c.b };
	}

	friend bool operator== (const Color& c1, const Color& c2) {
		return c1.r == c2.r && c1.g == c2.g && c1.b == c2.b;
	}

	friend bool operator!= (const Color& c1, const Color& c2) {
		return !(c1 == c2);
	}


	friend std::ostream& operator << (std::ostream& os, const Color& c) {
		return os << "( " << c.r << ", " << c.g << ", " << c.b << " )";
	}
};

