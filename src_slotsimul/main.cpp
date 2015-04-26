#include <stdio.h>
#include "SizzlingHot.hpp"
#include "Random.hpp"

int main()
{
	Game* game = new GameSizzlingHot();
	game->load();
	Random::init();
	int spinCount = 1e7;
	printf("This is slotsimul: %d Otacek>\n", spinCount);
	for (int i = 0; i < spinCount; i++)
	{
		game->start();
	}
	const Statistics& s = game->getStats();
	printf("RTP: %5.2f%%\n", s.statWin.getPct(Settings::priceOfSpin * spinCount));
	double pct = s.statWin0.getPct(spinCount);
	printf("Procento nulovych: %5.2f%%, pocet nulovych: %4.1f\n", pct, 100.0/(100.0-pct));
	int nz = spinCount - s.statWin0.getTotal();
	printf("U100: %5.2f%%,   U200: %5.2f%%,	  O200: %5.2f%%\n", s.statWinU100.getPct(nz), s.statWinU200.getPct(nz), s.statWinO200.getPct(nz));
	printf("Maximalni vyhra: %d\n", s.maxWin);
	return 0;
}