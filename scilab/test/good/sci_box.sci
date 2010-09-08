// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2004 - INRIA - Vincent COUVERT 
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function [tree]=sci_box(tree)
// M2SCI function
// Conversion function for Matlab box()
// Input: tree = Matlab funcall tree
// Ouput: tree = Scilab equivalent for tree

// box toggle
if rhs<=0 then
  tree.name="mtlb_box"
// box on/off  
elseif rhs==1 then
  if typeof(tree.rhs(1))=="cste" then
    a=gettempvar()
    insert(Equal(list(a),Funcall("gca",1,list(),list())))
    
    LHS=Operation("ins",list(a,Cste("box")),list())
    insert(Equal(list(LHS),Cste(convstr(tree.rhs(1).value,"l"))))

    tree=list()
  else
    tree.name="mtlb_box"
  end

// box(axes_handle,...)
else
  if typeof(tree.rhs(2))=="cste" then
    LHS=Operation("ins",list(tree.rhs(1),Cste("box")),list())
    insert(Equal(list(LHS),Cste(convstr(tree.rhs(2).value,"l"))))
    
    tree=list()
  else
    tree.name="mtlb_box"
  end
end
endfunction
