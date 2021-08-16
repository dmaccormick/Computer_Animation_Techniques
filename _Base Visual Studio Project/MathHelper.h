#ifndef MATHHELPER_H
#define MATHHELPER_H

template <typename T>
T LERP(T& P1, T& P2, float t)
{
	return ((1 - t) * P1) + (t * P2);
}

#endif