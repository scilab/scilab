function [col] = parseColor(funcName, colName, parseList)

//
// Parsing function for the 'background' or 'foreground' property 
//

if length(parseList) == 1  
    error(sprintf('%s : missing %s color spec',funcName,colName));
end
select type(parseList(2))
case 1 // a matrix (must be a 3 element vector)
   if (length(parseList(2))==3) & or(size(parseList(2))==1)
      col=parseList(2);
   else
      error(sprintf('%s : %s color spec must be a 3 element vector',funcName,colName));
   end
case 10 // a string
    if colName=="facecolor"
		if parseList(2)~="none" ...
		& parseList(2)~="flat" ...
		& parseList(2)~="interp"
			error(sprintf('%s : %s is an unknown facecolor specification',funcName,parseList(2)));
		end
	elseif colName=="edgecolor"
		if parseList(2)~="none"
			error(sprintf('%s : %s is an unknown edgecolor specification',funcName,parseList(2)));
		end
    elseif colName=="backfacecolor"	
	 if parseList(2)~="none"
        error(sprintf('%s : %s is an unknown backfacecolor specification',funcName,parseList(2)));
	 end
	end	
	col=parseList(2);
else
    error(sprintf('%s : missing %s color spec',funcName,colName));
end
