function [scale] = parseScale(funcName, scaleName, parseList)

//
// Parsing function for the 'Xscale' or 'Yscale' property 
//

if length(parseList) == 1  
    error(sprintf('%s : missing %s specification',funcName,scaleName));
end

select type(parseList(2))
case 10 //  a string
   select parseList(2)
   case 'log'
      scale='log'
   case 'linear'
      scale='linear'
   else
      error(sprintf('%s : unknown scale type ''%s''',funcName,parseList(2)));
   end
else
   error(sprintf('%s : missing scale type mode ',funcName));
end






