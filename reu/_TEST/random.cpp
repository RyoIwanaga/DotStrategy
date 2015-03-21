#include "../RandMt.h"

int main(void)
{
	std::random_device rd;
	for (int i=0; i<10; i++) {
		std::cout << reu::RandMt::random()
			<< std::endl;
	}
}

