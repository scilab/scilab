function [bfc]=parseBackFaceLighting(typeOfPlot,argList)

if ~(typeOfPlot=='surfl' | typeOfPlot=='trisurfl')
   error(sprintf('%s : BackFaceLighting property is not allowed (see surfl or trisurfl)',typeOfPlot))
end

if length(argList)>=2
if type(argList(2))==10
   select argList(2)
   case 'lit'
      bfc='none';
   case 'unlit'
      bfc='auto';
   else
     error(sprintf('%s : unknown BackFaceLighting specification ''%s''',typeOfPlot,argList(2)))
   end
else
   error(sprintf('%s : BackFaceLighting specification must be a string',typeOfPlot))
end
else
    error(sprintf('%s : missing BackFaceLighting specification',typeOfPlot))
end

endfunction // end of parseBackFaceLighting
