
#include "Motors_Dashboard.h"

int main()
{
	Motors_Dashboard_Init();

	while (1)
	{
		Motors_Dashboard_Run();
	}
}