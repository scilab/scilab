function trimesh(varargin)

[lhs,rhs]=argn(0);

if rhs==0
   load(PLOTLIB+'dinosaure.dat')
   clf()
   trimesh(nodes,x,y,z,'hidden','off','axis','equal')
else
   state=loadGraphicState(gcw());
   cmap=state('colormap');

   varargin(0)=state('background');
   varargin(0)='facecolor';
   varargin(0)=cmap(1,:);
   varargin(0)='edgecolor';
   mainPlot3d('trimesh',varargin);

end

endfunction /////
