function [cmap]=parseColormap(funcName,parseList)

if length(parseList) == 1
   error(sprintf('%s : missing colormap spec',funcName));
end

state=loadGraphicState(gcw());
nc=size(state('RGBcolormap'),1);

select type(parseList(2))

case 10 //  a string
  select parseList(2)
    case 'hot'
       cmap=hotcolormap(nc);
    case 'gray'
       cmap=graycolormap(nc);
    case 'copper'
       cmap=coppercolormap(nc);
    case 'cool'
       cmap=coolcolormap(nc);
    case 'bone'
       cmap=bonecolormap(nc);
    case 'red'
       cmap=redcolormap(nc);
    case 'red'
       cmap=redcolormap(nc);
    case 'green'
       cmap=greencolormap(nc);
    case 'blue'
       cmap=bluecolormap(nc);
    case 'jet'
       cmap=jetcolormap(nc);
    else
       error(sprintf('%s : unknown colormap name',funcName));
    end
case 1 // a matrix (must be a nx3 element vector)
   if size(parseList(2),2)==3
      cmap=parseList(2);
   else
       error(sprintf('%s : colormap spec must be a n x 3 matrix',funcName));
   end
else
   error(sprintf('%s : missing colormap spec',funcName));
end
