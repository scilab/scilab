function options=default_options()
// Copyright INRIA
options=scsoptlist()
col3d=[0.8 0.8 0.8]
//if xget('use color')==1 then   //suppose always color screen
  options('3D')=list(%t,33)   // we add gray to color map
//else
//  options('3D')=list(%f,0)
//  col3d=[]
//end
options('Background')=[8 1] //white,black
options('Link')=[1,5] //black,red
options('ID')=list([5 1],[4 1])
options('Cmap')=col3d

endfunction
function options=scsoptlist(varargin)
lt=['scsopt','3D','Background','Link','ID','Cmap']
options=tlist(lt,varargin(:))
endfunction
