function [s] = parseFontSize(funcName, parseList)

//
// Parsing function for the 'fontSize' property 
//

if length(parseList) == 1  
    error(sprintf('%s : missing font size',funcName));
end
select type(parseList(2))
case 1 // a matrix (must be a scalar)
   if (length(parseList(2))==1) & (parseList(2)>=8) & (parseList(2)<=24)
	  sz=[8 10 12 14 18 24];
	  [m,s]=min(abs(parseList(2)-sz));
	  s=s-1;
   else
      error(sprintf('%s : font size must be between 8 and 24',funcName));
   end
else
    error(sprintf('%s : missing font size',funcName));
end
