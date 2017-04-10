#include "Vector3.h"
#include "RotationMatrix.h"
#include "MathUtil.h"
#include "Quaternion.h"
#include "EulerAngles.h"

void RotationMatrix::identity(){
	m11 = 1.0f; m12 = 0.0f; m13 = 0.0f;
	m21 = 0.0f; m22 = 1.0f; m23 = 0.0f;
	m31 = 0.0f; m32 = 0.0f; m33 = 1.0f;
}

void RotationMatrix::setup(){

}