function process2DAxis(state,nTicksX,nTicksY,foreground,background,modeStart,modeScale,gridFlag,gridColor)
  
    f=gcf();
  
 	if state('nextPlot')=='erase' 
	
		// Prepare the axis and all the stuff

		cmap=f.color_map;
		
		if and(cmap(17,:)==background)
			back=17;
		else
			cmap(16,:)=background;
			back=16;
			f.color_map=cmap;
		end
		
		ax=state('axis');
		minX=ax(1); minY=ax(2); maxX=ax(3); maxY=ax(4);	
		
		plot2d1(modeScale,minX,minY,back,part(modeStart,1)+part(modeStart,2)+'0',' ',ax);

	end

endfunction
