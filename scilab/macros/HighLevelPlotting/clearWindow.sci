function clearWindow(win,typeOfPlot,cmap)

	[lhs,rhs]=argn(0);

    f=gcf();
    xbasc();
    if rhs==1
		return		
	end

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
				xset('colormap',finalColormap);
			end
		elseif typeOfPlot=="fig"
			if length(state('RGBcolormaptable'))==0
				finalColormap=[state('colormap')];
				xset('colormap',finalColormap);
			end
		end		
	end
	fg=state('foreground')*255;	
	bg=state('frameColor')*255;
	f.foreground=color(fg(1),fg(2),fg(3));
	f.background=color(bg(1),bg(2),bg(3));

endfunction
