function [marg] = parseMargin(funcName, parseList)

//
// Parsing function for the 'margin' property 
//

if length(parseList) == 1  
    error(sprintf('%s : missing margin value',funcName));
end
select type(parseList(2))
case 1 // a matrix (must be a scalar)
   if (length(parseList(2))==1) & (parseList(2)>=0) & (parseList(2)<.5)
      marg=parseList(2);
   else
      error(sprintf('%s : margin value must be a positive scalar smaller than .5',funcName));
   end
else
    error(sprintf('%s : missing margin value',funcName));
end
