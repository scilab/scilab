function [tree]=sci_graymon(tree)
// Copyright INRIA
// M2SCI function
// Conversion function for Matlab graymon()
// Input: tree = Matlab funcall tree
// Ouput: tree = Scilab equivalent for tree
// V.C.

// set(gdf(),"color_map",[.75 .5 .25]'*ones(1,3))

tree.name="set"

rc=Operation("rc",list(Cste(0.75),Cste(0.5)),list());
rc=Operation("rc",list(rc,Cste(0.25)),list());
transp=Operation("''",list(rc),list())
ones_funcall=Funcall("ones",1,Rhs(1,3),list())
mult=Operation("*",list(transp,ones_funcall),list())

gdf_funcall=Funcall("gdf",1,list(),list())

tree.rhs=Rhs(gdf_funcall,"color_map",mult);
endfunction
