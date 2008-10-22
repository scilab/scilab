//
//Author : F.Leray
//Copyright INRIA
//

function [PName] = getSurfPropertyName(PropertyName,current_figure,cur_draw_mode)

//conversion to lower format
str = convstr(PropertyName);


Table    = ['cdata' 'colordata' 'cdata_mapping' 'cdatamapping' 'colordatamapping' ..
    'cdatamode'     'colordatamode' 'clipping' 'edgecolor'   'foreground' 'facecolor' ..
    'linestyle' 'linewidth' 'thickness' 'marker' .. 
    'markstyle' 'markeredgecolor' 'markforeground' 'markerfacecolor'  'markbackground' ..
    'markersize' 'marksize' 'visible' 'xdata' 'ydata' 'zdata'];

TableEQ =  ['colordata' 'colordata' 'colordatamapping' 'colordatamapping' 'colordatamapping' ..
    'colordatamode' 'colordatamode' 'clipping' 'foreground'  'foreground' 'facecolor' ..
    'linestyle' 'thickness' 'thickness' 'markstyle' ..
    'markstyle'  'markforeground' 'markforeground' 'markbackground' 'markbackground' .. 
    'marksize'   'marksize' 'visible' 'xdata' 'ydata' 'zdata']; // equivalent Table

opt1=[];

k=find(part(Table,1:length(str))==str);

if (k == [])
  warning("Error in Property specification : bad argument specified");
  PName=[];
  ResetFigureDDM(current_figure, cur_draw_mode);
  return;
elseif ( size(k,'*') > 1)
  if or(find(k==14)) // case PropertyName == 'marker'
    k=14; // set to PropertyName == 'marker' only
  elseif or(find(k==2)) // case PropertyName == 'colordata'
    k=2;
  else
    warning("Ambiguous surface property");
    PName=[];
    ResetFigureDDM(current_figure, cur_draw_mode);
    return;
  end
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
