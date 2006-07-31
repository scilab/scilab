//
//Author : F.Leray
//Copyright INRIA
//

function [fail]=setSurfProperty(PropertyName,PropertyValue,Surface,X,Y,Z,C,current_figure,cur_draw_mode)

fail=0;

//conversion to lower format
str = convstr(PropertyName);



//Property = ['foreground' 'clipping'];

[PName] = getSurfPropertyName(str,current_figure,cur_draw_mode)

if (PName==[])
  ResetFigureDDM(current_figure, cur_draw_mode);
  return;
end

select PName

  /////////////////////////
case 'colordata'
  /////////////////////////
  
  if (type(PropertyValue)<>1 | or(size(PropertyValue)==1))
    warning("Color data  must be a matrix.");
    ResetFigureDDM(current_figure, cur_draw_mode);
    return;
  end
  
  if (size(Z) == size(PropertyValue)) | (size(Z)-1 == size(PropertyValue))
    [tmp1,tmp2,tmp3,CC] = CreateFacetsFromXYZColor(X,Y,Z,PropertyValue);
    Surface.data.color = CC;
  else
    str='surf : incompatible dimensions in input arguments';
    warning(str);
    ResetFigureDDM(current_figure, cur_draw_mode);
    return;
  end
  
  /////////////////////////
case 'colordatamapping'
  /////////////////////////

  if (type(PropertyValue)<>10)
    warning("Color data mapping must be a string with value ''scaled'' or ''direct''.");
    ResetFigureDDM(current_figure, cur_draw_mode);
    return;
  end
  
  if (PropertyValue=='scaled')
    Surface.cdata_mapping = 'scaled';
  elseif (PropertyValue=='direct')
    Surface.cdata_mapping = 'direct';
  else
    warning("Error : the cdatamapping property should be set exactly to ''scaled'' or ''direct''.")
    ResetFigureDDM(current_figure, cur_draw_mode);
    return;
  end
  
  /////////////////////////
case 'cdatamode'
  /////////////////////////
  
  //  /////////////////////////
  //case 'clipping'           // Clipping // NO CLIPPING for now with 3d objects
  //  /////////////////////////
  //  if (type(PropertyValue)==10 & (PropertyValue=='on' | PropertyValue=='off'))
  //    Surface.clip_state=PropertyValue;
  //  else
  //    warning("Bad value for property : Clipping")
  //    return;
  //  end

  
  /////////////////////////
case 'foreground'         // <=> EdgeColor
  /////////////////////////

  if (type(PropertyValue) == 10)
    
    index = getColorIndex(PropertyValue);
    
    ColorVal   = ['red' 'green' 'blue' 'cyan' 'magenta' 'yellow' 'black' 'black' 'white']

    if index < 10
      Surface.surface_mode='on';
      Surface.foreground = color(ColorVal(index));
      Surface.mark_foreground = color(ColorVal(index));
    elseif index == 10  // 'none' selected
      Surface.surface_mode='on';
      Surface.color_mode = 0; // <=> - colormap(1) and not black at all!!
      Surface.mark_foreground = -1; // <=> black
    else
      warning("Color value must be a 3 element vector or an index in the colormap.");
      ResetFigureDDM(current_figure, cur_draw_mode);
      return;
    end
  elseif (type(PropertyValue) == 1) // we entered plot(x,y,'Color',[R,G,B])
    
    if (size(PropertyValue,'*')==3)
      Surface.surface_mode='on';
      Surface.foreground = addcolor(PropertyValue);
      Surface.mark_foreground = addcolor(PropertyValue);
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
case 'facecolor'          // FaceColor
  /////////////////////////

  if (type(PropertyValue) == 10)
    
    index = getColorIndex(PropertyValue);
    
    ColorVal   = ['red' 'green' 'blue' 'cyan' 'magenta' 'yellow' 'black' 'black' 'white']

    if index < 10
      Surface.surface_mode='on';
      Surface.color_mode = color(ColorVal(index));
      //Surface.mark_foreground = color(ColorVal(index));
      Surface.color_flag = 0;
    elseif index == 10  // 'none' selected
      Surface.surface_mode='on';
      Surface.color_mode = 0;
      //Surface.mark_foreground = color(ColorVal(index));
      Surface.color_flag = 0;
    elseif index == 12  // 'flat' selected
      Surface.surface_mode='on';
      Surface.color_flag = 4;
    elseif index == 13  // 'interp' selected
      Surface.surface_mode='on';
      Surface.color_flag = 3;
    else
      warning("Incorrect input : Color vector should be a 3x1 or 1x3 vector");
      ResetFigureDDM(current_figure, cur_draw_mode);
      return;
    end
  elseif (type(PropertyValue) == 1) // we entered plot(x,y,'Color',[R,G,B])
    
    if (size(PropertyValue,'*')==3)
      Surface.surface_mode='on';
      Surface.foreground = addcolor(PropertyValue);
      Surface.mark_foreground = addcolor(PropertyValue);
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
case 'linestyle'          // LineStyle
  /////////////////////////
  if (type(PropertyValue)==10)
    if (PropertyValue=='--')
      Surface.line_style=2;
      Surface.surface_mode = 'on';
    elseif (PropertyValue=='-.')
      Surface.line_style=4;
      Surface.surface_mode = 'on';
    elseif (PropertyValue==':')
      Surface.line_style=5;
      Surface.surface_mode = 'on';
    elseif (PropertyValue=='-')
      Surface.line_style=1;
      Surface.surface_mode = 'on';
    elseif (PropertyValue=='none')
      Surface.surface_mode = 'off';
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
    Surface.thickness=PropertyValue;
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
      Surface.mark_style = MarksStyleVal(opt1);
      Surface.mark_size  = 6;
      //MarksSizeVal(opt1);
    else
      // 'none' is selected
      Surface.mark_mode='off'
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
    
    markmodeON = find(Surface.mark_mode=='on');
    
    if index == 10
      // 'none' specified
      a=gca(); // pick up the background color of the parent axes
      if markmodeON <> []
	Surface(markmodeON).mark_foreground = a.background;
      end
    elseif index == 11
      // 'auto' specified
      if markmodeON <> []
	Surface(markmodeON).mark_foreground =  Surface.foreground;
      end
    else
      if (index==-1)
	warning("Bad value for markforeground property");
	ResetFigureDDM(current_figure, cur_draw_mode);
	return;
      else
	if markmodeON <> []
	  Surface(markmodeON).mark_foreground = color(ColorVal(index));
	end
      end
    end
  elseif (type(PropertyValue)==1)
    if (size(PropertyValue,'*')==3)
      
      markmodeON = find(Surface.mark_mode=='on');
      if markmodeON <> []
	Surface(markmodeON).mark_foreground = addcolor(PropertyValue);
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
    
    markmodeON = find(Surface.mark_mode=='on');
    
    if index == 10
      // 'none' specified
      a=gca(); // pick up the background color of the parent axes
      if markmodeON <> []
	Surface(markmodeON).mark_background = a.background;
      end
    elseif index == 11
      // 'auto' specified
      a=gca();
      if markmodeON <> []
	Surface(markmodeON).mark_background = a.background;
      end
    else
      if (index==-1)
	warning("Bad value for markbackground property");
	ResetFigureDDM(current_figure, cur_draw_mode);
	return;
      else
	if markmodeON <> []
	  Surface(markmodeON).mark_background = color(ColorVal(index));
	end
      end
    end
  elseif (type(PropertyValue)==1)
    
    if (size(PropertyValue,'*')==3)
      
      markmodeON = find(Surface.mark_mode=='on');
      if markmodeON <> []
	Surface(markmodeON).mark_background = addcolor(PropertyValue);
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
case 'marksize'        // <=> MarkerSize
  /////////////////////////
  if (type(PropertyValue)==1 & size(PropertyValue,'*')==1)
    
    markmodeON = find(Surface.mark_mode=='on');
    
    if markmodeON <> []
      Surface(markmodeON).mark_size = PropertyValue;
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
    Surface.visible = PropertyValue;
  else
    warning("Error : the visibility property should be set to on or off.");
    ResetFigureDDM(current_figure, cur_draw_mode);
    return;
  end
  
  
  /////////////////////////
case 'xdata'
  /////////////////////////
    
  // Already done at the beginning of surf execution.
 
    /////////////////////////
case 'ydata'
  /////////////////////////
  
  // Already done at the beginning of surf execution.
 
  /////////////////////////
case 'zdata'
  /////////////////////////
  
  // Already done at the beginning of surf execution.
end

endfunction



function k=getIndexInStringTable(pattern,table)

str =  convstr(pattern);
k=find(part(table,1:length(str))==str);

endfunction


