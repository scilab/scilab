function draw2DAxis(state,nTicksX,nTicksY,foreground,background,modeStart,gridFlag,gridColor)

      // plot the axis and all the stuff

      back=addcolor(background);

      ax=state('axis');
      minX=ax(1); minY=ax(2); maxX=ax(3); maxY=ax(4);

      xset('line style',1);
      
      plot2d1(modeScale,minX,minY,,modeStart,' ',ax,[2,nTicksX,2,nTicksY]);

      if gridFlag
      	 gc=addcolor(gridColor);
		 set(gca(),'grid',[gc gc]);
		 pause
      end
      	  
