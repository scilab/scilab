// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2004 - INRIA - Vincent COUVERT 
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function [tree]=sci_cla(tree)
// M2SCI function
// Conversion function for Matlab cla()
// Input: tree = Matlab funcall tree
// Ouput: tree = Scilab equivalent for tree

if rhs<1 then
  set_infos(gettext("All children will be deleted, no HandleVisibility property in Scilab graphics."),2);
end

// a=gca()
a=gettempvar()
insert(Equal(list(a),Funcall("gca",1,list(),list())))

tree.name="delete"
tree.rhs=list(Operation("ext",list(a,Cste("children")),list()))
endfunction
