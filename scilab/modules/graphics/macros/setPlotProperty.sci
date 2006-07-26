//
//Author : F.Leray
//Copyright INRIA
//

function [fail]=setPlotProperty(PropertyName,PropertyValue,Curves,current_figure,cur_draw_mode)

fail=0;

//conversion to lower format
str = convstr(PropertyName);



//Property = ['foreground' 'clipping'];

[PName] = getPlotPropertyName(str,current_figure,cur_draw_mode)


if (PName==[])
  ResetFigureDDM(current_figure, cur_draw_mode);
  return;
end


select PName

  /////////////////////////
case 'foreground'         // <=> Color
  /////////////////////////

  if (type(PropertyValue) == 10)
    
    index = getColorIndex(PropertyValue);
    
    ColorVal   = ['red' 'green' 'blue' 'cyan' 'magenta' 'yellow' 'black' 'black' 'white'];

    if (index < 10)
      Curves.line_mode='on';
      Curves.foreground = color(ColorVal(index));
      Curves.mark_foreground = color(ColorVal(index));
    else  // 'none' selected
      warning("Bad value for line color property : none can not be selected");
      ResetFigureDDM(current_figure, cur_draw_mode);
      return;
    end
  elseif (type(PropertyValue) == 1) // we entered plot(x,y,'Color',[R,G,B])
    
    if (size(PropertyValue,'*')==3)
      Curves.line_mode='on';
      Curves.foreground = addcolor(PropertyValue);
      Curves.mark_foreground = addcolor(PropertyValue);
    else
      warning("Incorrect input : Color vector should be a 3x1 or 1x3 vector");
      ResetFigureDDM(current_figure, cur_draw_mode);
      return;
    end

  else
    warning("Color value must be a 3 element vector or an index in the colormap.");
    ResetFigureDDM(current_figure, cur_draw_mode);
    return;
  end


  /////////////////////////
case 'clipping'           // Clipping
  /////////////////////////
  if (type(PropertyValue)==10 & (PropertyValue=='on' | PropertyValue=='off'))
    Curves.clip_state=PropertyValue;
  else
    warning("Bad value for property : Clipping");
    ResetFigureDDM(current_figure, cur_draw_mode);
    return;
  end


  /////////////////////////
case 'linestyle'          // LineStyle
  /////////////////////////
  if (type(PropertyValue)==10)
    if (PropertyValue=='--')
      Curves.line_style=2;
      Curves.line_mode = 'on';
    elseif (PropertyValue=='-.')
      Curves.line_style=4;
      Curves.line_mode = 'on';
    elseif (PropertyValue==':')
      Curves.line_style=5;
      Curves.line_mode = 'on';
    elseif (PropertyValue=='-')
      Curves.line_style=1;
      Curves.line_mode = 'on';
    elseif (PropertyValue=='none')
      Curves.line_mode = 'off';
    end
  else
    warning("Bad value for property : LineStyle");
    ResetFigureDDM(current_figure, cur_draw_mode);
    return;
  end

  /////////////////////////
case 'thickness'        // <=> LineWidth
  /////////////////////////
  if (type(PropertyValue)==1)
    Curves.thickness=PropertyValue;
  else
    warning("Bad value for property : LineStyle");
    ResetFigureDDM(current_figure, cur_draw_mode);
    return;
  end

  /////////////////////////
case 'markstyle'        // <=> Marker
  /////////////////////////
  if (type(PropertyValue)==10)

    str = convstr(PropertyValue);
    //Marks
    Table = [ '+' 'o' '*' '.' 'x' 'square' 'diamond' '^' 'v' '>' '<' 'pentagram' 'none'];
    MarksStyleVal=[1 9 10 0 2 11 5 6 7 12 13 14 -99];
//    MarksSizeVal =[4 3 7  1 3  3 4 3 3  3  3  3 -99];
    
    opt1=[];
    
    k=find(part(Table,1:length(str))==str);
    
    if (k == [])
      warning("Error in MarkStyle specification : bad argument specified");
      PName=[];
      ResetFigureDDM(current_figure, cur_draw_mode);
      return;
    elseif ( size(k,'*') > 1)
      warning("Ambiguous MarkStyle value"); //unreachable case normally
      PName=[];
      ResetFigureDDM(current_figure, cur_draw_mode);
      return;
    end
        
    opt = Table(k);
    for i=1:length(str)
      if part(opt,i) <> part(str,i)
	break;
      end
    end
    
    opt1 = [opt1 k];
    
    str = part(str,i:length(str));
    
    if (size(opt1,'*') > 1)
      warning("Error in MarkStyle specification : too much MarkStyle properties specified");
      ResetFigureDDM(current_figure, cur_draw_mode);
      return;
    end
    
    if (opt1 > 0)
      Curves.mark_style = MarksStyleVal(opt1);
      Curves.mark_size  = 6;
      //MarksSizeVal(opt1);
    else
      // 'none' is selected
      Curves.mark_mode='off'
    end
    
  else
    warning("Bad value for property : LineStyle");
    ResetFigureDDM(current_figure, cur_draw_mode);
    return;
  end

  
  /////////////////////////
case 'markforeground'        // <=> MarkerEdgeColor
  /////////////////////////
  if (type(PropertyValue)==10)
    
    index = getColorIndex(PropertyValue);
    
    ColorVal   = ['red' 'green' 'blue' 'cyan' 'magenta' 'yellow' 'black' 'black' 'white' 'none']
    
    markmodeON = find(Curves.mark_mode=='on');
    
    if index == 10
      // 'none' specified
      a=gca(); // pick up the background color of the parent axes
      if markmodeON <> []
	Curves(markmodeON).mark_foreground = a.background;
      end
    elseif index == 11
      // 'auto' specified
      if markmodeON <> []
	Curves(markmodeON).mark_foreground =  Curves.foreground;
      end
    else
      if (index==-1)
	warning("Bad value for markforeground property");
	ResetFigureDDM(current_figure, cur_draw_mode);
	return;
      else
	if markmodeON <> []
	  Curves(markmodeON).mark_foreground = color(ColorVal(index));
	end
      end
    end
  elseif (type(PropertyValue)==1)
    if (size(PropertyValue,'*')==3)
      
      markmodeON = find(Curves.mark_mode=='on');
      if markmodeON <> []
	Curves(markmodeON).mark_foreground = addcolor(PropertyValue);
      end
    else
      warning("Incorrect input : Color vector should be a 3x1 or 1x3 vector");
      ResetFigureDDM(current_figure, cur_draw_mode);
      return;
    end
    
  else
    warning("Color value must be a 3 element vector or an index in the colormap.");
    ResetFigureDDM(current_figure, cur_draw_mode);
    return;
  end
  
  
  /////////////////////////
case 'markbackground'        // <=> MarkerFaceColor
  /////////////////////////
  if (type(PropertyValue)==10)
    
    index = getColorIndex(PropertyValue);
    
    ColorVal   = ['red' 'green' 'blue' 'cyan' 'magenta' 'yellow' 'black' 'black' 'white' 'none']
        
    markmodeON = find(Curves.mark_mode=='on');
    
    if index == 10
      // 'none' specified
      a=gca(); // pick up the background color of the parent axes
      if markmodeON <> []
	Curves(markmodeON).mark_background = a.background;
      end
    elseif index == 11
      // 'auto' specified
      a=gca();
      if markmodeON <> []
	Curves(markmodeON).mark_background = a.background;
      end
    else
      if (index==-1)
	warning("Bad value for markbackground property");
	ResetFigureDDM(current_figure, cur_draw_mode);
	return;
      else
	if markmodeON <> []
	  Curves(markmodeON).mark_background = color(ColorVal(index));
	end
      end
    end
  elseif (type(PropertyValue)==1)
    
    if (size(PropertyValue,'*')==3)
      
      markmodeON = find(Curves.mark_mode=='on');
      if markmodeON <> []
	Curves(markmodeON).mark_background = addcolor(PropertyValue);
      end
    else
      warning("Incorrect input : Color vector should be a 3x1 or 1x3 vector");
      ResetFigureDDM(current_figure, cur_draw_mode);
      return;
    end
    
  else
    warning("Color value must be a 3 element vector or an index in the colormap.")
    ResetFigureDDM(current_figure, cur_draw_mode);
    return;
  end
  
  
  /////////////////////////
case 'marksize'        // <=> MarkerSize
  /////////////////////////
  if (type(PropertyValue)==1 & size(PropertyValue,'*')==1)
    
    markmodeON = find(Curves.mark_mode=='on');
    
    if markmodeON <> []
      Curves(markmodeON).mark_size = PropertyValue;
    end
  else
    warning("Color value must be an integer.");
    ResetFigureDDM(current_figure, cur_draw_mode);
    return;
  end
  
  /////////////////////////
case 'visible'        // <=> Visible
  /////////////////////////
  if (type(PropertyValue)==10 & (PropertyValue=='on' | PropertyValue=='off'))
    Curves.visible = PropertyValue;
  else
    warning("Error : the visibility property should be set to on or off.");
    ResetFigureDDM(current_figure, cur_draw_mode);
    return;
  end
  
  
  /////////////////////////
case 'xdata'
  /////////////////////////
  
  // Already done at the beginning of plot execution.
  
  /////////////////////////
case 'ydata'
  /////////////////////////

  // Already done at the beginning of plot execution.
  
  /////////////////////////
case 'zdata'        // <=> Zdata is treated after the curve was created
  /////////////////////////
  
  if (type(PropertyValue)<>1 | and(size(PropertyValue)<>1))
    warning("Zdata value must be a column or row vector.")
    ResetFigureDDM(current_figure, cur_draw_mode);
    return;
  else
    PropertyValue = PropertyValue(:); // force
    
    for j=1:size(Curves,'*')
      if size(Curves(i).data,1) <> size(PropertyValue,'*')
	str='plot : incompatible dimensions in input arguments';
	warning(str);
	ResetFigureDDM(current_figure, cur_draw_mode);
	return;
      else
	for jj=1:size(PropertyValue,'*')
	  Curves(j).data(jj,3) = PropertyValue(jj);
	end
	a=gca();
	a.view='3d';
	a.data_bounds=[a.data_bounds(1,1) a.data_bounds(1,2) min(PropertyValue) ;  a.data_bounds(2,1) a.data_bounds(2,2) max(PropertyValue)];
	a.view='2d';
      end
    end
  end
  
  
  
end

endfunction
