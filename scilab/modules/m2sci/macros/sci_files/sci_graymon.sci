// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2004 - INRIA - Vincent COUVERT 
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function [tree]=sci_graymon(tree)
// M2SCI function
// Conversion function for Matlab graymon()
// Input: tree = Matlab funcall tree
// Ouput: tree = Scilab equivalent for tree

// set(gdf(),"color_map",[.75 .5 .25]'*ones(1,3))

tree.name="set"

rc=Operation("rc",list(Cste(0.75),Cste(0.5)),list());
rc=Operation("rc",list(rc,Cste(0.25)),list());
transp=Operation("''",list(rc),list())
ones_funcall=Funcall("ones",1,Rhs_tlist(1,3),list())
mult=Operation("*",list(transp,ones_funcall),list())

gdf_funcall=Funcall("gdf",1,list(),list())

tree.rhs=Rhs_tlist(gdf_funcall,"color_map",mult);
endfunction
