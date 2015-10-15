 md grafy
 for /L %%i in (1, 1, 20) do	(
	Release\slotsimul.exe
	plot_graph.plt
	mv plot1_out.png grafy\graf%%i.png
	)