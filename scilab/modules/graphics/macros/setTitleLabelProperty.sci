function [fail]=setTitleLabelProperty(PropertyName,PropertyValue,titlelabel,current_figure,cur_draw_mode)
// Copyright INRIA
// SETTITLELABELPROPERTY function
// is used by the functions title, xlabel, ylabel, zlabel
// This function sets the title (or x_, y_, z_label) properties 
// F.Belahcene

fail=0;

//conversion to lower format
str = convstr(PropertyName);

//Property = ['foreground' 'clipping'];

[PName] = getTitleLabelPropertyNam(str,current_figure,cur_draw_mode)

if (PName==[])
  ResetFigureDDM(current_figure, cur_draw_mode);
  return;
end

select PName

  /////////////////////////
case 'font_size'       // <=> fontsize 
  /////////////////////////

if (type(PropertyValue)<>1 | size(PropertyValue,"*")<>1)
  warning("Color data  must be a scalar.");
  ResetFigureDDM(current_figure, cur_draw_mode);
  return;
end

titlelabel.font_size = PropertyValue
return
  

  /////////////////////////
case 'font_angle'  // <=> rotation
  /////////////////////////
if (type(PropertyValue)<>1 | size(PropertyValue,"*")<>1)
  warning("Color data  must be a scalar.");
  ResetFigureDDM(current_figure, cur_draw_mode);
  return;
end

titlelabel.auto_rotation = 'off'
newPropertyValue=modulo(PropertyValue,360)
titlelabel.font_angle = 360-newPropertyValue
return


  /////////////////////////
case 'font_foreground'         // <=> color or edgecolor
  /////////////////////////

  if (type(PropertyValue) == 10)
    index = getColorIndex(PropertyValue);
    
    ColorVal   = ['red' 'green' 'blue' 'cyan' 'magenta' 'yellow' 'black' 'black' 'white']

    if index < 10
      titlelabel.font_foreground = color(ColorVal(index));
    elseif index == 10  // 'none' selected
      titlelabel.color_mode = 0; // <=> - colormap(1) and not black at all!!
    else
      warning("Color value must be a 3 element vector or an index in the colormap.");
      ResetFigureDDM(current_figure, cur_draw_mode);
      return;
    end
  elseif (type(PropertyValue) == 1) // we entered plot(x,y,'Color',[R,G,B])
    
    if (size(PropertyValue,'*')==3)
      titlelabel.font_foreground = addcolor(PropertyValue);
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
case 'foreground'        // <=> foregroundcolor
  /////////////////////////
  
if (type(PropertyValue) == 10)
    index = getColorIndex(PropertyValue);
    
    ColorVal   = ['red' 'green' 'blue' 'cyan' 'magenta' 'yellow' 'black' 'black' 'white']

   if index < 10
      titlelabel.fill_mode = 'on'
      titlelabel.foreground = color(ColorVal(index));
    elseif index == 10  // 'none' selected
      titlelabel.color_mode = 0; // <=> - colormap(1) and not black at all!!
    else
      warning("Color value must be a 3 elements vector or an index in the colormap.");
      ResetFigureDDM(current_figure, cur_draw_mode);
      return;
    end
elseif (type(PropertyValue) == 1) // we entered plot(x,y,'Color',[R,G,B])
    
    if (size(PropertyValue,'*')==3) 
      titlelabel.fill_mode = 'on'
      titlelabel.foreground = addcolor(PropertyValue);
    else
      warning("Incorrect input : Color vector should be a 3x1 or 1x3 vector");
      ResetFigureDDM(current_figure, cur_draw_mode);
      return;
    end
else
    warning("Color value must be a 3 elements vector or an index in the colormap.");
    ResetFigureDDM(current_figure, cur_draw_mode);
    return;
end
  
  
  /////////////////////////
case 'background'        // <=> backgroundcolor
  /////////////////////////

if (type(PropertyValue) == 10)
   
    index = getColorIndex(PropertyValue);
    
    ColorVal   = ['red' 'green' 'blue' 'cyan' 'magenta' 'yellow' 'black' 'black' 'white']

    if index < 10
      titlelabel.fill_mode = 'on'
      titlelabel.background = color(ColorVal(index));
    elseif index == 10  // 'none' selected
      titlelabel.color_mode = 0; // <=> - colormap(1) and not black at all!!
    else
      warning("Color value must be a 3 elements vector or an index in the colormap.");
      ResetFigureDDM(current_figure, cur_draw_mode);
      return;
    end
elseif (type(PropertyValue) == 1) // we entered plot(x,y,'Color',[R,G,B])
    
    if (size(PropertyValue,'*')==3) 
      titlelabel.fill_mode = 'on'
      titlelabel.background = addcolor(PropertyValue);
    else
      warning("Incorrect input : Color vector should be a 3x1 or 1x3 vector");
      ResetFigureDDM(current_figure, cur_draw_mode);
      return;
    end
else
    warning("Color value must be a 3 elements vector or an index in the colormap.");
    ResetFigureDDM(current_figure, cur_draw_mode);
    return;
end

  
  /////////////////////////
case 'text'        // <=> string
  /////////////////////////
  if (type(PropertyValue) == 10)   
     titlelabel.text = PropertyValue;
     return
  else
    warning("Incorrect input : ...");
    ResetFigureDDM(current_figure, cur_draw_mode);
    return;
  end
  

  /////////////////////////
case 'position'        // <=> position, à completer: la position par rapport à z n'est pas pris en compte, de plus le vecteur position doit contenir des coordonnées utilisateur (coordonnées selon l'axe)
  /////////////////////////

  if type(PropertyValue) == 1 & size(PropertyValue,"*")<=3    
    titlelabel.auto_position = 'off';
    titlelabel.position = PropertyValue ;
    return
  else
    warning("Incorrect input : ...");
    ResetFigureDDM(current_figure, cur_draw_mode);
    return;
  end

  
  /////////////////////////  
case 'visible'           // <=> visible
  /////////////////////////

  if type(PropertyValue) == 10
    newPropertyName = convstr(PropertyName);
    if or(PropertyValue == ['off' 'of'])
      titlelabel.visible = 'off';
      return
    elseif PropertyValue == 'on'
      titlelabel.visible = 'on'; 
        return
    else
      warning("Incorrect input : ...");
      ResetFigureDDM(current_figure, cur_draw_mode);
      return;
    end
  else
    warning("Incorrect input : ...");
    ResetFigureDDM(current_figure, cur_draw_mode);
    return;    
  end
  
 
  /////////////////////////  
case 'font_style'     // <=> fontangle
  /////////////////////////
 
    warning("unknown property value : ...");
    ResetFigureDDM(current_figure, cur_draw_mode);
    return;    
  
end

endfunction


function k=getIndexInStringTable(pattern,table)

str =  convstr(pattern);
k=find(part(table,1:length(str))==str);

endfunction


