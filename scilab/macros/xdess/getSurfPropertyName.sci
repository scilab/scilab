//
//Author : F.Leray
//Copyright INRIA
//

function [PName] = getSurfPropertyName(PropertyName)

//conversion to lower format
str = convstr(PropertyName);


Table    = ['cdata'     'colordata' 'cdatamapping'     'colordatamapping' ..
    'cdatamode'     'colordatamode' 'clipping' 'edgecolor'   'foreground' 'facecolor' ..
    'linestyle' 'linewidth' 'thickness' 'marker' .. 
    'markstyle' 'markeredgecolor' 'markforeground' 'markerfacecolor'  'markbackground' ..
    'markersize' 'marksize' 'visible' 'xdata' 'ydata' 'zdata'];

TableEQ =  ['colordata' 'colordata' 'colordatamapping' 'colordatamapping' ..
    'colordatamode' 'colordatamode' 'clipping' 'foreground'  'foreground' 'facecolor' ..
    'linestyle' 'thickness' 'thickness' 'markstyle' ..
    'markstyle'  'markforeground' 'markforeground' 'markbackground' 'markbackground' .. 
    'marksize'   'marksize' 'visible' 'xdata' 'ydata' 'zdata']; // equivalent Table

opt1=[];

k=find(part(Table,1:length(str))==str);

if (k == [])
  disp("Error in Property specification : bad argument specified");
  PName=[];
  return;
elseif ( size(k,'*') > 1)
  if or(find(k==14)) // case PropertyName == 'marker'
    k=14; // set to PropertyName == 'marker' only
  else
    disp("Ambiguous line property");
    PName=[];
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
  disp("Error in Property specification : too much properties specified")
  return;
end

//disp("opt1 vaut")
//disp(opt1);

PName = TableEQ(opt1);

endfunction
