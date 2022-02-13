#pragma once

namespace GE {

	struct Vertex {

		float x, y, z;
		float u, v;

		Vertex(float _x, float _y, float _z, float _u, float _v) :
			x(_x), y(_y), z(_z), u(_u), v(_v){}

		Vertex() {
			x = y = z = 0.0f;
			u = v = 0.0f;
		}
	};

	struct Vector3 {
		float x, y, z;

		Vector3(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}

		Vector3 Multiply(const Vector3& other) const
		{
			return *this * other;
		}

		Vector3 operator *(const Vector3& other) const
		{
			return Vector3(x * other.x, y * other.y, z * other.z);
		}

	};

	struct VertexRGBA {

		float x, y, z;
		float r, g, b, a;

		VertexRGBA(float _x, float _y, float _z, float _r, float _g, float _b, float _a) :
			x(_x), y(_y), z(_z), r(_r), g(_g), b(_b), a(_a) {}

		VertexRGBA() {
			x = y = z = 0.0f;
			r = g = b = a = 0.0f;
		}
	};
}