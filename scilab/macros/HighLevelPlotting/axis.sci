function [vect]=axis(varargin) 

[lhs,rhs]=argn(0);

if rhs>0
   error('axis : to set the axis, use the property ''axis'' in a plot command')
end

vect=[];


//if winsid()==[]
//   return
//end

winNum=xget('window');
state=loadGraphicState(winNum);

vect=state('axis');

if length(vect)==4
   vect=vect([1 3 2 4]); // convert to [xmin xmax ymin ymax]
else
   vect=vect([1 4 2 5 3 6]);
end

