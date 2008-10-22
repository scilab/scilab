// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2004 - INRIA - Vincent COUVERT 
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function [tree]=sci_hankel(tree)
// M2SCI function
// Conversion function for Matlab hankel()
// Input: tree = Matlab funcall tree
// Ouput: tree = Scilab equivalent for tree

tree.name="hank"

// H = hankel(c)
if rhs==1 then
  // %c_hank and %b_hank are not defined
  if tree.rhs(1).vtype<>Double then
    tree.rhs(1)=convert2double(tree.rhs(1))
  end
  if typeof(tree.rhs(1))<>"variable" then
    c=gettempvar();
    insert(Equal(list(c),tree.rhs(1)))
  else
    c=tree.rhs(1);
  end
  
  n=gettempvar()
  insert(Equal(list(n),Funcall("size",1,Rhs_tlist(c,"*"),list())));
  
  if is_a_scalar(tree.rhs(1)) then
    v=c
  else
    // v=[matrix(c,1,-1),zeros(1,n)]
    matrix_funcall=Funcall("matrix",1,Rhs_tlist(c,1,-1),list())
    zeros_funcall=Funcall("zeros",1,Rhs_tlist(1,n),list())
    v=Operation("rc",list(matrix_funcall,zeros_funcall),list())
  end

  dim=1
  for kd=1:lstsize(tree.rhs(1).dims)
    if tree.rhs(1).dims(kd)<>Unknown then
      dim=dim*tree.rhs(1).dims(kd)
    else
      dim=Unknown
      break
    end
  end
  
  tree.lhs(1).dims=list(dim,dim)
  tree.lhs(1).type=tree.rhs(1).type

  tree.rhs=Rhs_tlist(n,n,v);
// H = hankel(c,r)
else
  // %c_hank and %b_hank are not defined
  if tree.rhs(1).vtype<>Double then
    tree.rhs(1)=convert2double(tree.rhs(1))
  end
  
  if typeof(tree.rhs(1))<>"variable" then
    c=gettempvar();
    insert(Equal(list(c),tree.rhs(1)))
  else
    c=tree.rhs(1);
  end
  if tree.rhs(2).vtype<>Double then
    tree.rhs(2)=convert2double(tree.rhs(2))
  end
  
  if typeof(tree.rhs(2))<>"variable" then
    r=gettempvar();
    insert(Equal(list(r),tree.rhs(2)))
  else
    r=tree.rhs(2);
  end

  m=gettempvar();
  n=gettempvar();
  insert(Equal(list(m),Funcall("size",1,Rhs_tlist(c,"*"),list())));
  insert(Equal(list(n),Funcall("size",1,Rhs_tlist(r,"*"),list())));
  
  
  // v=[matrix(c,1,-1),matrix(r(2:$),1,-1)]
  cmatrix_funcall=Funcall("matrix",1,Rhs_tlist(c,1,-1),list())
  imp=Operation(":",list(Cste(2),Cste("$")),list())
  ext=Operation("ext",list(r,imp),list())
  rmatrix_funcall=Funcall("matrix",1,Rhs_tlist(ext,1,-1),list())
  v=Operation("rc",list(cmatrix_funcall,rmatrix_funcall),list())

  dim1=1
  for kd=1:lstsize(tree.rhs(1).dims)
    if tree.rhs(1).dims(kd)<>Unknown then
      dim1=dim1*tree.rhs(1).dims(kd)
    else
      dim1=Unknown
      break
    end
  end
  dim2=1
  for kd=1:lstsize(tree.rhs(2).dims)
    if tree.rhs(2).dims(kd)<>Unknown then
      dim2=dim2*tree.rhs(2).dims(kd)
    else
      dim2=Unknown
      break
    end
  end
  
  tree.lhs(1).dims=list(dim1,dim2)
  tree.lhs(1).type=tree.rhs(1).type
  
  tree.rhs=Rhs_tlist(m,n,v);
end
endfunction
