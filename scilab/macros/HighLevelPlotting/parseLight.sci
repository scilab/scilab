function [vect]=parseLight(typeOfPlot,argList,azin,elin);


if ~(typeOfPlot=='surfl' | typeOfPlot=='trisurfl')
   error(sprintf('%s : light property is not allowed (see surfl or trisurfl)',typeOfPlot))
end

if length(argList)>=2
if type(argList(2))==1
   if length(argList(2))==2
      az=argList(2)(1)*%pi/180;
      el=argList(2)(2)*%pi/180;
      vect=[cos(az)*sin(el);sin(az)*sin(el);cos(el)];
   elseif length(argList(2))==3
      vect=argList(2);
      vect=vect(:);
   else
      error(sprintf('%s : light specification must be a 2 or 3-vector',typeOfPlot))
   end
else
   error(sprintf('%s : light specification must be a vector',typeOfPlot))
end
else
    error(sprintf('%s : missing light vector',typeOfPlot))
end

endfunction // end of parseLight
