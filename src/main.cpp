#include <stdio.h>
#include <math.h>
#include "SizzlingHot.h"
#include "Random.h"

int main()
{
	Game *game = new GameSizzlingHot();
	game->load();
	game->isInteractive = false;

	long long spinCount = 1e5;

	printf("Probiha simulace 10^%d otacek, verze valce %s:\n"
			, int(ceil(log(double(spinCount))/log(10.0)))
			, game->getRSVersion().c_str());

	for (long long i = 0; i < spinCount; i++)
	{
		game->start();
		if (!(i % (spinCount / 10))) printf("* ");
	}
	printf("\n");

	//printf("RTP: %5.2f%%\n", s.statWin.getPct(Settings::bet * spinCount));

	//double pct = s.statWin0.getPct(spinCount);
	//printf("Procento nulovych: %5.2f%%, pocet nulovych: %4.1f\n", pct, 100.0/(100.0-pct));

	//int nz = spinCount - s.statWin0.getTotal();
	//printf("U100: %5.2f%%,   U200: %5.2f%%,	  O200: %5.2f%%\n", s.statWinU100.getPct(nz), s.statWinU200.getPct(nz), s.statWinO200.getPct(nz));
	//printf("Maximalni vyhra: %d\n", s.maxWin);

	game->printFinalFormattedStats("Statistics");

	return 0;
}