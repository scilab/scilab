function plot3(varargin)

[lhs,rhs]=argn(0);

if rhs==0
   t=linspace(0,8*%pi,256);
   plot3(cos(t),sin(t),t);
else
   mainPlot3d('plot3',varargin);
end

endfunction /////

