// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2004 - INRIA - Vincent COUVERT 
// 
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function [tree]=sci_MFUN(tree)
// M2SCI function
// Conversion function for Matlab MFUN()
// Input: tree = Matlab funcall tree
// Output: tree = Scilab equivalent for tree

if rhs==2 then
  tree.rhs(2)=null()
end

if lhs==1 then
  V=gettempvar(2)
  S=tree.lhs(1)
  if S.name=="ans" then
    S=gettempvar(1)
    tree.lhs=list(V,S)
  else
    tree.lhs=list(V,S)
  end
  m2sci_insert(Equal(tree.lhs,tree))
  tree=list()
  
  if rhs==2 then
    m2sci_insert(Equal(list(),Funcall("disp",1,list(S),list())),1)
  end
else
  tree.lhs=list(tree.lhs($:-1:1))

  if rhs==2 then
    m2sci_insert(Equal(list(),Funcall("disp",1,list(tree.lhs(1),tree.lhs(2)),list())),1)
  end

  tree.lhs(1).dims=list(Unknown,Unknown)
  tree.lhs(1).type=Type(String,Real)
  
  tree.lhs(2).dims=list(1,1)
  tree.lhs(2).type=Type(Double,Real)
  
end
endfunction
