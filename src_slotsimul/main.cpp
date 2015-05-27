#include <stdio.h>
#include "SizzlingHot.hpp"
#include "Random.hpp"

int main()
{
	Game* game = new GameSizzlingHot();
	game->load();
	Random::init();
	long long spinCount = 1e8;
	printf("This is slotsimul: %d Otacek>\n", spinCount);
	for (long long i = 0; i < spinCount; i++)
	{
		game->start();
		if (!(i % (spinCount / 10))) printf("* ");
	}
	printf("\n");
	const Statistics& s = game->getStats();
	printf("RTP: %5.2f%%\n", s.statWin.getPct(Settings::bet * spinCount));
	double pct = s.statWin0.getPct(spinCount);
	printf("Procento nulovych: %5.2f%%, pocet nulovych: %4.1f\n", pct, 100.0/(100.0-pct));
	int nz = spinCount - s.statWin0.getTotal();
	printf("U100: %5.2f%%,   U200: %5.2f%%,	  O200: %5.2f%%\n", s.statWinU100.getPct(nz), s.statWinU200.getPct(nz), s.statWinO200.getPct(nz));
	printf("Maximalni vyhra: %d\n", s.maxWin);
	s.printFinalFormatted();
	return 0;
}