function [color] = parseColor(funcName, colName, parseList)

//
// Parsing function for the 'background' or 'foreground' property 
//

if length(parseList) == 1  
    error(sprintf('%s : missing %s color spec',funcName,colName));
end
select type(parseList(2))
case 1 // a matrix (must be a 3 element vector)
   if (length(parseList(2))==3) & or(size(parseList(2))==1)
      color=parseList(2);
   else
      error(sprintf('%s : %s color spec must be a 3 element vector',funcName,colName));
   end
case 10 // a string
	color=parseList(2);
else
    error(sprintf('%s : missing %s color spec',funcName,colName));
end
