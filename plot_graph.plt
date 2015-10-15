otacek = 1000
rtp = 0.9284
soubor = 'C:\aCodes\Slot design\slotdesign\outputs\statWin.txt'
osartp(r, x) = -5*x*(1-r)

set term unknown
osa2(y) = 100.0*(y/(5*otacek) + 1)
plot 0 ls 101 notitle, -5*x*(1-0.9284) ls 2 title 'Teoretická předpověď', soubor u 1:($2-5*$1) every 1 ls 1 with lines title 'Skutečná hra'	
y_min = GPVAL_Y_MIN
y_max = GPVAL_Y_MAX
set y2range [osa2(y_min): osa2(y_max)]
set y2tics 1
set yrange [y_min: y_max]

set term pngcairo enhanced size 880,660 font "arial,11"
set output 'plot1_out.png'

set title 'Sizzling Hot, kredit při 1 000 otáčkách'
set xlabel 'Otáčky'
set ylabel 'Kredit'
set y2label 'Výhernost'
set y2tics format "{/*0.75 %.0f%%}"

set style line 100 lc rgb '#404040'
set border 3 back ls 100
set tics nomirror
set grid

set style line 101 lc rgb '#E0E0E0' lw 2
set style line 102 lc rgb '#E0E0E0'
set style line 1 lc rgb '#009966'
set style line 2 lc rgb '#dea900'
set y2tics border

plot 0 ls 101 notitle, osartp(0.9, x) ls 102 notitle, osartp(0.95, x) ls 102 notitle, osartp(rtp, x) ls 2 title 'Teoretická předpověď', soubor u 1:($2-5*$1) every 1 ls 1 with lines title 'Skutečná hra'