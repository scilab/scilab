function triplot(varargin)

[lhs,rhs]=argn(0);

if rhs==0   
	clf()
	load(PLOTLIB+'tridem.dat')
	triplot(nodes,xy(1,:),xy(2,:),...
	'edgecolor',[1 1 0],'background',[0 0 1],'axis','equal');
else
   mainPlot3d('triplot',varargin);
end

endfunction /////
