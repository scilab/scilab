function [cbar]=parseColorBar(funcName,parseList)

if length(parseList) == 1
   error(sprintf('%s : missing colorbar spec',funcName));
end

select type(parseList(2))

case 10 //  a string
  select parseList(2)
    case 'off'
       cbar='off';
    case 'on'
       cbar='left';
    case 'right'
       cbar='right';
    case 'left'
       cbar='left';
    case 'bottom'
       cbar='bot';
    case 'bot'
       cbar='bot';
    case 'top'
       cbar='top';
    else
       error(sprintf('%s : %s is an unknown colorbar spec',funcName,parseList(2)));
    end
else
   error(sprintf('%s : missing colorbar spec',funcName));
end
