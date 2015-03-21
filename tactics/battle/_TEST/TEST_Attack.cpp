#include "../Attack.h"
#include <cassert>

namespace tb = tactics::battle;

int main(void)
{
	printf("TEST::Attack\n");

	tb::Attack attack1;
	tb::Attack attack2(1, 50);
	tb::Attack attack3(1, 50, true);
	tb::Attack attack4(1, 50, false);

	attack1.print(); REU__NEWLINE;
	attack2.print(); REU__NEWLINE;
	attack3.print(); REU__NEWLINE;
	attack4.print(); REU__NEWLINE;

	return 0;
}
