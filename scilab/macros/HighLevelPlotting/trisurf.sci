function trisurf(varargin)

[lhs,rhs]=argn(0);

if rhs==0 
   load(PLOTLIB+'dinosaure.dat')
   clf()
   trisurf(nodes,x,y,z,'axis','equal','shading','interp','colorbar','on')
else

   mainPlot3d('trisurf',varargin);

end

endfunction /////
