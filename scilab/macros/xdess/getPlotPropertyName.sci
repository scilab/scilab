//
//Author : F.Leray
//Copyright INRIA
//

function [PName] = getPlotPropertyName(PropertyName,current_figure,cur_draw_mode)

//conversion to lower format
err = execstr('str = convstr(PropertyName);','m');

if err <> 0
  mprintf("plot syntax error (help plot for info)\n");
  mprintf("Error %d : in getPlotPropertyName called by plot\n",err);
  ResetFigureDDM(current_figure, cur_draw_mode);
  return;
end

Table    = ['clipping' 'color'      'foreground' 'linestyle' 'linewidth' 'thickness' 'marker' ..
	'markstyle' 'markeredgecolor' 'markforeground' 'markerfacecolor'  'markbackground' ..
	'markersize' 'marksize' 'visible' 'xdata' 'ydata' 'zdata'];
TableEQ =  ['clipping' 'foreground' 'foreground' 'linestyle' 'thickness' 'thickness' 'markstyle'..
	'markstyle'  'markforeground' 'markforeground' 'markbackground' 'markbackground' .. 
	'marksize'   'marksize' 'visible' 'xdata' 'ydata' 'zdata']; // equivalent Table

opt1=[];

k=find(part(Table,1:length(str))==str);

if (k == [])
  disp("Error in Property specification : bad argument specified");
  PName=[];
  ResetFigureDDM(current_figure, cur_draw_mode);
  return;
elseif ( size(k,'*') > 1)
  if or(find(k==7)) // case PropertyName == 'marker'
    k=7; // set to PropertyName == 'marker' only
  else
    disp("Ambiguous line property");
    PName=[];
    ResetFigureDDM(current_figure, cur_draw_mode);
    return;
  end
end

//disp("str=")
//disp(str)
//disp("k=")
//disp(k);

opt = Table(k);
for i=1:length(str)
  if part(opt,i) <> part(str,i)
    break;
  end
end

opt1 = [opt1 k];

str = part(str,i:length(str));

if (size(opt1,'*') > 1)
  disp("Error in Property specification : too much properties specified");
  ResetFigureDDM(current_figure, cur_draw_mode);
  return;
end

//disp("opt1 vaut")
//disp(opt1);

PName = TableEQ(opt1);

endfunction
