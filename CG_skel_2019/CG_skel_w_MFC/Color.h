#pragma once

#define CLAMP(x) (x) = ((x) > 1)? 1 : (x)

struct Color {
	float r;
	float g;
	float b;

	Color() : r(1), g(1), b(1) {}
	Color(float r, float g, float b) : r(CLAMP(r)), g(CLAMP(g)), b(CLAMP(b)) {}

	Color operator + (const Color& c) const {
		return Color{ r + c.r, g + c.g, b + c.b };
	}

	Color operator * (const float s) const {
		return Color{ s * r, s * g, s * b };
	}

	Color operator * (const Color& c) const {
		return Color{ r * c.r, g * c.g, b * c.b };
	}

	friend Color operator * (const float s, const Color& c) {
		return Color{ s * c.r, s * c.g, s * c.b };
	}


	friend std::ostream& operator << (std::ostream& os, const Color& c) {
		return os << "( " << c.r << ", " << c.g << ", " << c.b << " )";
	}
};

