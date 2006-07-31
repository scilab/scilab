function [PName] = getTitleLabelPropertyNam(PropertyName,current_figure,cur_draw_mode)
// Copyright INRIA
// GETTITLELABELPROPERTYNAME function
// is used by the functions title, xlabel, ylabel, zlabel
// this function gets the matlab properties names and repalce them by the equivalent scilab properties names, using Table and TableEQ  
// F.Belahcene

//conversion to lower format
str = convstr(PropertyName);


Table    = [ 'visible' 'fontname' 'fontangle' 'fontsize' 'rotation' ..
             'position' 'color' 'edgecolor' 'backgroundcolor' ];

TableEQ =  [ 'visible' 'font_style' 'font_style' 'font_size' 'font_angle' ..
             'position' 'font_foreground' 'foreground' 'background'] 

//TableEQ =  [ 'visible' 'font_style' 'font_style' 'font_size' 'font_angle' ..
  //           'position' 'foreground' 'background' .. 
    //         'visible' 'fill_mode' 'auto_position' 'auto_rotation']; // equivalent Table

opt1=[];

k=find(part(Table,1:length(str))==str);

if (k == [])
  warning("Error in Property specification : bad argument specified");
  PName=[];
  ResetFigureDDM(current_figure, cur_draw_mode);
  return;
elseif ( size(k,'*') > 1)
    warning("Ambiguous text property");
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
  warning("Error in Property specification : too much properties specified");
  ResetFigureDDM(current_figure, cur_draw_mode);
  return;
end

PName = TableEQ(opt1);
endfunction
