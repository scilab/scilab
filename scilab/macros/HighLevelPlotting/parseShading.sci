function [facec,edgec]=parseShading(typeOfPlot,argList)
if length(argList)>=2
if type(argList(2))==10
   select argList(2)
   case 'flat'
      facec="flat';
	  edgec="none";
   case 'faceted'
      facec="flat';
      edgec=[];
   case 'interp'
      facec="interp';
      edgec="none";
   else
     error(sprintf('%s : unknown shading specification ''%s''',typeOfPlot,argList(2)))
   end
else
   error(sprintf('%s : shading specification must be a string',typeOfPlot))
end
else
    error(sprintf('%s : missing shading specification',typeOfPlot))
end

endfunction // end of parseShading
