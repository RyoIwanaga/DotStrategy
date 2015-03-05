#ifndef reu__RandMt_h_
#define reu__RandMt_h_

#include "_header.h"
#include "macro.h"

namespace reu { // namespace reu /////////////////////////////////////

/** Random class using mt19937
 *
 * XXX need to memoize
 * TODO range real number
 */
class RandMt
{
	std::mt19937 _mt;

private:
	RandMt() :
		_mt((unsigned int)time(NULL))
	{
//		std::random_device rd;
	}

public:
	int getRandom()
	{
		return _mt();
	}

	int getInt(int max, int min = 0)
	{
		std::uniform_int_distribution<int> dice(min, max);

		return dice(_mt);
	}

	static RandMt& getInstance()
	{
		static RandMt instance;

		return instance;
	}

	static int random()
	{
		return RandMt::getInstance().getRandom();
	}

	static int range(int max, int min = 0)
	{
		return RandMt::getInstance().getInt(max, min);
	}

	/** base 100 20 %
	 *  80 ~ 120
	 */
	static int rangePercent(int base, int percent)
	{
		int n = base * percent / 100;
		return base + RandMt::range(n, -n);
	}
};

}

#endif // reu__RandMt_h_
