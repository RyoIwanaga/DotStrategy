#include "../action/actions.h"
#include <cassert>

namespace tb = tactics::battle;

int main(void)
{
	printf("TEST::Actions\n");

	printf("actions::ActionAttackMelee\n");

	// make path
	tactics::battle::Path path;
	path.push_back(boardgame::Point(1, 1));
	path.push_back(boardgame::Point(2, 1));
	path.push_back(boardgame::Point(3, 1));
	path.push_back(boardgame::Point(4, 2));

	printf(":::: normal attack -> enemy retaliation\n");
	{
		std::vector<tb::Attack> attacksNormal;
		attacksNormal.push_back(tb::Attack(2, 50));
		attacksNormal.push_back(tb::Attack(3, 100));
		tb::ActionAttackMelee meleeNormal(1, 2, 3, path,
				boardgame::Point(55, 55), attacksNormal);
		meleeNormal.print();
	}

	printf(":::: normal attack [dead] -> no retaliation\n");
	{
		std::vector<tb::Attack> attacksNormal;
		attacksNormal.push_back(tb::Attack(2, 50, true));
		attacksNormal.push_back(tb::Attack());
		tb::ActionAttackMelee meleeNormal(1, 2, 3, path,
				boardgame::Point(55, 55), attacksNormal);
		meleeNormal.print();
	}

	printf(":::: normal attack -> enemy retaliation [dead]\n");
	{
		std::vector<tb::Attack> attacksNormal;
		attacksNormal.push_back(tb::Attack(2, 50));
		attacksNormal.push_back(tb::Attack(3, 100, true));
		tb::ActionAttackMelee meleeNormal(1, 2, 3, path,
				boardgame::Point(55, 55), attacksNormal);
		meleeNormal.print();
	}


	printf(":::: [double attack] normal attack -> enemy retaliation -> normal attack\n");
	{
		std::vector<tb::Attack> attacksDouble;
		attacksDouble.push_back(tb::Attack(2, 50));
		attacksDouble.push_back(tb::Attack(3, 100));
		attacksDouble.push_back(tb::Attack(2, 50));
		tb::ActionAttackMelee meleeDouble(1, 2, 3, path,
				boardgame::Point(55, 55), attacksDouble);
		meleeDouble.print();
	}

	printf(":::: [double attack] normal attack -> no retaliation -> normal attack\n");
	{
		std::vector<tb::Attack> attacksDouble;
		attacksDouble.push_back(tb::Attack(2, 50));
		attacksDouble.push_back(tb::Attack());
		attacksDouble.push_back(tb::Attack(2, 50));
		tb::ActionAttackMelee meleeDouble(1, 2, 3, path,
				boardgame::Point(55, 55), attacksDouble);
		meleeDouble.print();
	}

	printf(":::: no damage -> enemy retaliation >>> error\n");
	if (false) {
		std::vector<tb::Attack> attacksWrong;
		attacksWrong.push_back(tb::Attack()); // <= wrong
		attacksWrong.push_back(tb::Attack(3, 100));
		tb::ActionAttackMelee meleeWrong(1, 2, 3, path,
				boardgame::Point(55, 55), attacksWrong);
	}

	printf(":::: only nomal attack >>> error\n");
	if (false) {
		std::vector<tb::Attack> attacksWrong;
		attacksWrong.push_back(tb::Attack(3, 100));
		tb::ActionAttackMelee meleeWrong(1, 2, 3, path,
				boardgame::Point(55, 55), attacksWrong);
	}

//	// normal attack
//	std::vector<tb::Attack> attacksWrong;
//	attacksNormal.push_back(tb::Attack(2, 50));
//	tb::ActionAttackMelee meleeWrong(1, 2, 3, path,
//			boardgame::Point(55, 55), attacksNormal);




	return 0;
}
