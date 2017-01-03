#include "math.h"

#ifndef ANGLE_H_
#define ANGLE_H_

const double PI = 3.1415926;

struct Angle
{
	short x;
	short y;

	Angle() {}
	Angle(short _x, short _y)
	{
		Set(_x, _y);
	}

	inline void Set(short _x, short _y)
	{
		x = _x;
		y = _y;
	}
};

#endif /* ANGLE_H_ */
