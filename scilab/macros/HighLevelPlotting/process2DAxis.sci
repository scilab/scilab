function process2DAxis(state,nTicksX,nTicksY,foreground,background,modeStart,modeScale,gridFlag,gridColor)
  
 	if state('nextPlot')=='erase' 
	
		// Prepare the axis and all the stuff

		cmap=xget('colormap')
		if and(cmap(17,:)==background)
			back=17;
		else
			cmap(16,:)=background;
			back=16;
			xset('colormap',cmap);
		end
		fram=addcolor(state('frameColor'));
		xset('foreground',addcolor(foreground));

		ax=state('axis');
		minX=ax(1); minY=ax(2); maxX=ax(3); maxY=ax(4);	
		
		plot2d1(modeScale,minX,minY,back,part(modeStart,1)+part(modeStart,2)+'0',' ',ax);

		if 0  // Paint the the plot box only if the background is different from the frameColor

			if part(modeScale,2)=='l'
				minXX=minX/1000;
				maxXX=maxX*1000;
			else
				minXX=minX-(maxX-minX)*10;
				maxXX=minX+(maxX-minX)*10;
			end
			if part(modeScale,3)=='l'
				minYY=minY/10000;
				maxYY=maxY*10000;
			else
				minYY=minY-(maxY-minY)*10;
				maxYY=maxY+(maxY-minY)*10;
			end
			a=gca();
			a.clip_state='clipgrf'
			xfpolys([minXX maxXX maxXX minXX]',[minYY minYY maxYY maxYY]',back);
			a.clip_state='off'
		end
	end

endfunction
