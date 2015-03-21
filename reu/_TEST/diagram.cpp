#include "../diagram.h"

int main(void)
{
	REU__PRINTLN("=== Test diagram ===");

	REU__PRINTLN("radian::");
	REU__PRINTLN("0, 0, 1, 1 = " << reu::diagram::radian(0, 0, 1, 1));

	REU__PRINTLN("degree::");
	REU__PRINTLN("0, 0, 1, 1 = " << reu::diagram::degree(0, 0, 1, 1));
	REU__PRINTLN("0, 0, 0, 2 = " << reu::diagram::degree(0, 0, 0, 2));
	REU__PRINTLN("0, 0, 2, 0 = " << reu::diagram::degree(0, 0, 2, 0));

	REU__PRINTLN("distance::");
	REU__PRINTLN("0, 0, 1, 1 = " << reu::diagram::distance(0, 0, 1, 1));

	return 0;
}

