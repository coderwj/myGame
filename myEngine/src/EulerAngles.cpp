
#include <math.h>

#include "EulerAngles.h"
#include "Quaternion.h"
#include "MathUtil.h"
#include "Matrix4x3.h"
#include "RotationMatrix.h"

const EulerAngles kEulerAnglesIdentity(0.0f, 0.0f, 0.0f);

void EulerAngles::canonize() {
	pitch = wrapPi(pitch);

	if(pitch < -kPiOver2){
		pitch = - kPi - pitch;
		heading += kPi;
		bank += kPi;
	}
	else if(pitch > kPiOver2)
	{
		pitch = kPi - pitch;
		heading += kPi;
		bank += kPi;
	}

	if(fabs(pitch) > kPiOver2 - 1e-4){
		heading += bank;
		bank = 0.0f;
	}
	else{
		bank = wrapPi(bank);
	}

	heading = wrapPi(heading);
}

void EulerAngles::fromObjectToInertialQuaternion(const Quaternioin &q){
	float sp = -2.0f * (q.y * p.z - q.w * q.x);
	//todo
}