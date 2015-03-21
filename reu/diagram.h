#ifndef reu__diagram_h_
#define reu__diagram_h_

#include "_header.h"
#include "macro.h"

#include <math.h>

namespace reu {
	namespace diagram {

inline double radian(double x, double y, double x2, double y2)
{
    return atan2(x2 - x, y2 - y);
}

inline double degree(double x, double y, double x2, double y2)
{
	return radian(x, y, x2, y2) * 57.29577951; // PI * 180
}

inline double distance(double x, double y, double x2, double y2)
{
	double xDistace = x2 - x;
	double yDistace = y2 - y;

	return sqrt(xDistace * xDistace + yDistace * yDistace);
}

}}

#endif // reu__diagram_h_
