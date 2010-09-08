// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2004 - INRIA - Vincent COUVERT 
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function [tree]=sci_colordef(tree)
// M2SCI function
// Conversion function for Matlab colordef()
// Input: tree = Matlab funcall tree
// Ouput: tree = Scilab equivalent for tree

if tree.lhs(1).name=="ans" then
  f=gettempvar()
else  
  f=tree.lhs(1)
end

// colordef color_option
if rhs==1 then
  if typeof(tree.rhs(1))=="cste" then
    if or(tree.rhs(1).value==["black","none"]) then
      insert(Equal(list(f),Funcall("gcf",1,list(),list())))
      LHS=Operation("ins",list(f,Cste("background")),list())
      insert(Equal(list(LHS),Cste(-1)))
      tree=list()
    elseif tree.rhs(1).value=="white" then
      insert(Equal(list(f),Funcall("gcf",1,list(),list())))
      LHS=Operation("ins",list(f,Cste("background")),list())
      insert(Equal(list(LHS),Cste(-2)))
      tree=list()
    else
      no_equiv(expression2code(tree))
    end
  else
    tree.name="mtlb_colordef"
  end
else
  // colordef('new',color_opt)
  if tree.rhs(1).vtype==String then
    winsid_funcall=Funcall("winsid",1,list(),list())
    max_funcall=Funcall("max",1,list(winsid_funcall),list())
    addition=Operation("+",list(max_funcall,Cste(1)),list())
    scf_funcall=Funcall("scf",1,list(addition),list())
    insert(Equal(list(f),scf_funcall))
    if or(tree.rhs(2).value==["black","none"]) then
      LHS=Operation("ins",list(f,Cste("background")),list())
      insert(Equal(list(LHS),Cste(-1)))
      tree=list()
    elseif tree.rhs(2).value=="white" then
      LHS=Operation("ins",list(f,Cste("background")),list())
      insert(Equal(list(LHS),Cste(-2)))
      tree=list()
    else
      no_equiv(expression2code(tree))
    end
    
// colordef(fig,color_opt)
elseif tree.rhs(1).vtype==Handle then
    f=tree.rhs(1)
    if or(tree.rhs(2).value==["black","none"]) then
      LHS=Operation("ins",list(f,Cste("background")),list())
      insert(Equal(list(LHS),Cste(-1)))
      if tree.lhs(1).name<>"ans" then
	insert(Equal(list(tree.lhs(1)),f))
      end
      tree=list()
    elseif tree.rhs(2).value=="white" then
      LHS=Operation("ins",list(f,Cste("background")),list())
      insert(Equal(list(LHS),Cste(-2)))
      if tree.lhs(1).name<>"ans" then
	insert(Equal(list(tree.lhs(1)),f))
      end
      tree=list()
    else
      no_equiv(expression2code(tree))
    end
  else
    tree.name="mtlb_colordef"
  end
end

endfunction
