function [gridFlag,gridColor]=parseGrid(funcName,parseList)

if length(parseList) == 1
   error(sprintf('%s : missing grid spec',funcName));
end

state=loadGraphicState(gcw());

select type(parseList(2))

case 10 //  a string
  select parseList(2)
    case 'on'
       gridColor=state('foreground');
       gridFlag=%t;
    case 'off'
       gridColor=[];
       gridFlag=%f;
    else
       error(sprintf('%s : unknown grid spec',funcName));
    end
case 1 // a matrix (must be a nx3 element vector)
   if length(parseList(2))==3
      gridColor=parseList(2);
      gridFlag=%t;
   else
       error(sprintf('%s : grid color must be a 3-vector',funcName));
   end
else
   error(sprintf('%s : missing grid spec',funcName));
end
