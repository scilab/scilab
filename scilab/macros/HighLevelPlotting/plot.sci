function plot(varargin)

[lhs,rhs]=argn(0);

if ~rhs
   clf();
   t=linspace(0,2*%pi,64);
   subplot(2,2,1); 
   plot(t,cos(t),'title','plot(t,cos(t))'); 
   subplot(2,2,2); 
   plot(t,[cos(t);sin(t);sin(t).*cos(t)],'title','plot(t,[cos(t);sin(t);sin(t).*cos(t)])');
   subplot(2,2,3); 
   plot(t,cos(t),t,cos(t),'og','background',[0 0 1],'title','plot(t,cos(t),t,cos(t),''og'',''background'',[0 0 1])');
   subplot(2,2,4); 
   plot(cos(t),sin(t),'axis','equal','title','plot(cos(t),sin(t),''axis'',''equal'')');
   return
end

varargin(0) = 'linear';
varargin(0) = 'Xscale';
varargin(0) = 'linear';
varargin(0) = 'Yscale';

mainPlot('plot',varargin);

// end of plot
