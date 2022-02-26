#pragma once
#include "Texture.h"

namespace GE {

	class Billboard
	{
	public:
		Billboard(Texture* _t) {
			m_texture = _t;
			pos_x = pos_y = pos_z = 0.0f;
			scale_x = scale_y = scale_z = 10.0f;
		}

		~Billboard() {
			delete m_texture;
		}

		float getX() {
			return pos_x;
		}

		float getY() {
			return pos_y;
		}

		float getZ() {
			return pos_z;
		}

		float setX(float _x) {
			pos_x = _x;
		}

		float setY(float _y) {
			pos_y= _y;
		}

		float setZ(float _z) {
			pos_z = _z;
		}

		float getScaleX() {
			return scale_x;
		}

		float getScaleY() {
			return scale_y;
		}

		float getScaleZ() {
			return scale_z;
		}

		float setScaleX(float _x) {
			scale_x = _x;
		}

		float setScaleY(float _y) {
			scale_y = _y;
		}

		float setScaleZ(float _z) {
			scale_z = _z;
		}

		Texture* getTexture() {
			return m_texture;
		}

	private:
		Texture* m_texture;
		float pos_x, pos_y, pos_z;
		float scale_x, scale_y, scale_z;


	};

}
