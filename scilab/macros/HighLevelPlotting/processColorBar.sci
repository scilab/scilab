function processColorBar(table,typ,state)

ech=state('cbech');
x=ech(1);y=ech(2);w=ech(3);h=ech(4);

n=length(table);
ax=caxis();



if typ=='left' | typ=='right'

	xsetech(wrect=[x y w h],frect=[0 ax(1) 1 ax(2)],arect=[0 0 0 0]);
	plot2d(0,0,nax=[1 0 2 4],rect=[0 ax(1) 1 ax(2)]);
	a=gca();
	a.background=state.mainAxis.background;
	a.tics_color=state.mainAxis.tics_color;
	a.font_color=state.mainAxis.font_color;
	a.font_size=state.mainAxis.font_size;
	Matplot1(table($:-1:1)',[0 ax(1) 1 ax(2)])
elseif typ=='top' | typ=='bot'

	xsetech(wrect=[x y w h],frect=[ax(1) 0 ax(2) 1],arect=[0 0 0 0]);
	plot2d(0,0,nax=[2 4 1 0],rect=[ax(1) 0 ax(2) 1]);
	a=gca();
	a.background=state.mainAxis.background;
	a.tics_color=state.mainAxis.tics_color;
	a.font_color=state.mainAxis.font_color;
	a.font_size=state.mainAxis.font_size;
	Matplot1(table,[ax(1) 0 ax(2) 1])

end
