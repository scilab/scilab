function trisurfl(varargin)

[lhs,rhs]=argn(0);

if rhs==0
   load(PLOTLIB+'dinosaure.dat')
   clf()
   colormap green
   trisurfl(nodes,x,y,z,'axis','equal','shading','interp')
else
   mainPlot3d('trisurfl',varargin);
end

endfunction /////
