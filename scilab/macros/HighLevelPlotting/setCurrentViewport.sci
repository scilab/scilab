function setCurrentViewport(state,win,modeScale)

	if state.nextPlot=='add'
		return
	end

	fg=addcolor(state.foreground);
	bg=addcolor(state.background);
	a=gca();
	a.font_style=6;
	a.font_color=fg;
	a.tics_color=fg;
	a.background=bg;
	a.foreground=fg;

	if fontSize~=-1
		a.font_size=fontSize;
	end

	if length(state.axis)==4 // 2D case
		a.axes_bounds=state.ech;
		a.data_bounds=[state.axis(1:2);state.axis(3:4)]
		a.margins=[0,0,0,0];  
		a.log_flags=modeScale;
	else // 3D case
		a.view="3d";
		a.axes_bounds=state.ech;
		a.margins=[0,0,0,0];  
	end

	if state.axisMode=='equal'
		a.isoview='on';
	end

	a.visible="on";

	state.mainAxis=a;
    saveGraphicState(state,win); // save the state of the current window

endfunction
