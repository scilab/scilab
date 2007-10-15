function BrowseTo_()
// Copyright INRIA
global %scicos_navig
global %diagram_path_objective
Cmenu=[]
pa=TCL_GetVar('blkox');
pa=part(pa,6:length(pa));
execstr('pa=['+pa+']');       
%scicos_navig=1
%diagram_path_objective=pa

endfunction
