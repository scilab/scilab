function setCurrentViewport(state,win)

	a=gca();
	a.font_style=6;
	if fontSize~=-1
	a.font_size=fontSize;
	end
	if length(state.axis)==4 // 2D case
		a.axes_bounds=state.ech;
		a.data_bounds=state.axis;
		a.margins=[0,0,0,0];  
	else // 3D case
		a.axes_bounds=state.ech;
		a.margins=[0,0,0,0];  
	end
	state.mainAxis=a;
    saveGraphicState(state,win); // save the state of the current window

endfunction
