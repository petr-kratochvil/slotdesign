#include <stdio.h>
#include <math.h>
#include "Cocktail.h"
#include "Random.h"

int main()
{
	Game *game = new GameCocktail();
	game->load();
	game->isInteractive = false;

	long long spinCount = 1e7;

	printf("Probiha simulace 10^%d otacek, verze valce %s:\n"
			, int(ceil(log(double(spinCount))/log(10.0)))
			, game->getRSVersion().c_str());

	for (long long i = 0; i < spinCount; i++)
	{
		game->start();
		if (!(i % (spinCount / 10))) printf("* ");
	}
	printf("\n");
	int sc = game->getSpinCount();
	long long w = game->getCredit() + Settings::bet * sc;
	printf("Spin count: %d, total win: %llu, RTP: %10.3f %%\n", sc, w, 100.0 * double(w) / double(Settings::bet * sc));

	//printf("RTP: %5.2f%%\n", s.statWin.getPct(Settings::bet * spinCount));

	//double pct = s.statWin0.getPct(spinCount);
	//printf("Procento nulovych: %5.2f%%, pocet nulovych: %4.1f\n", pct, 100.0/(100.0-pct));

	//int nz = spinCount - s.statWin0.getTotal();
	//printf("U100: %5.2f%%,   U200: %5.2f%%,	  O200: %5.2f%%\n", s.statWinU100.getPct(nz), s.statWinU200.getPct(nz), s.statWinO200.getPct(nz));
	//printf("Maximalni vyhra: %d\n", s.maxWin);

	game->printFinalFormattedStats("Statistics");

	return 0;
}