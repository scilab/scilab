function clearWindow(win,typeOfPlot,cmap)

	[lhs,rhs]=argn(0);

	winH=gcf();
	imm_draw = winH.immediate_drawing;
	winH.immediate_drawing='off';
	
    xbasc();
	
	if rhs~=1

		state=loadGraphicState(win);

		if rhs==2
			if 	typeOfPlot=="pcolor" | ...
				typeOfPlot=="surf" | ...
				typeOfPlot=="surfl" | ...
				typeOfPlot=="tripcolor" | ...
				typeOfPlot=="trisurf" | ...
				typeOfPlot=="trisurfl"

				if length(state('RGBcolormaptable'))==0
					RGBmap=state('RGBcolormap');
					n1=size(state('colormap'),1);
					n2=size(RGBmap,1);
					finalColormap=[state('colormap');RGBmap];
					state('RGBcolormaptable')=n1+1:n1+n2;
					saveGraphicState(state,win);
					winH.color_map=finalColormap;
				end
			elseif typeOfPlot=="fig"
				if length(state('RGBcolormaptable'))==0
					finalColormap=[state('colormap')];
					winH.color_map=finalColormap;
				end
			end		
		end
		fg=state('foreground')*255;	
		bg=state('frameColor')*255;
    	f=gcf();
		f.foreground=color(fg(1),fg(2),fg(3));
		f.background=color(bg(1),bg(2),bg(3));
	end

	a=gca();
	a.visible='off';
	
	winH.immediate_drawing=imm_draw;
	
endfunction
