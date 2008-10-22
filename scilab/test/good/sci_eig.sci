function [tree]=sci_eig(tree)
// Copyright INRIA
// M2SCI function
// Conversion function for Matlab eig()
// Input: tree = Matlab funcall tree
// Ouput: tree = Scilab equivalent for tree
// Emulation function: mtlb_eig()
// V.C.


// eig(A)
if rhs==1 then
  // Because %c_spec and %b_spec are not defined
  A = getrhs(tree)
  A = convert2double(A)
  tree.rhs=Rhs(A)

  // d = eig(A)
  if lhs==1 then
    tree.name="spec"
    tree.lhs(1).dims=list(A.dims(1),1)
    tree.lhs(1).type=Type(Double,Unknown)
  // [V,D] = eig(A)
  else
    set_infos("mtlb_eig() called because Scilab and Matlab value do not always match for "+tree.lhs(1).name,0)
    tree.name="mtlb_eig"
    tree.lhs(1).dims=list(A.dims(1),A.dims(1))
    tree.lhs(1).type=Type(Double,Unknown)
    tree.lhs(2).dims=list(A.dims(1),A.dims(1))
    tree.lhs(2).type=Type(Double,Unknown)
  end
// eig(A,B) or eig(A,'nobalance')
elseif rhs==2 then
  [A,B] = getrhs(tree)
  
  // 'nobalance'
  if B.vtype==String then
    no_equiv("''nobalance'' option, IGNORED")
    tree.rhs=Rhs(tree.rhs(1))
    if lhs==1 then
      tree.name="spec"
      tree.lhs(1).dims=list(A.dims(1),1)
      tree.lhs(1).type=Type(Double,Unknown)
    else
      tree.rhs(1)=Operation("+",list(tree.rhs(1),Variable("%i",Infer()),list()))
      rhs2=Operation("+",list(Cste(1),Variable("%eps",Infer()),list()))
      tree.rhs=Rhs(tree.rhs(1),rhs2)
      tree.name="bdiag"
      tree.lhs(1).dims=list(A.dims(1),A.dims(1))
      tree.lhs(1).type=Type(Double,Unknown)
      tree.lhs(2).dims=list(A.dims(1),A.dims(1))
      tree.lhs(2).type=Type(Double,Unknown)
    end
    return
  end
  
  if B.vtype<>Double then
    set_infos('eig() with 2 inputs: consider generalized eigen. Check ',2)
  end
  m=A.dims(1)
  n=A.dims(2)
  if lhs==1 then
    al=gettempvar(1)
    be=gettempvar(2)
    insert(Equal(list(al,be),Funcall("spec",1,tree.rhs,list(al,be))))
    tree=Operation("./",list(al,be),tree.lhs)
    
    tree.out(1).dims=list(m,1)
    tree.out(1).type=Type(Double,Unknown)
  else
    al=gettempvar(1)
    be=gettempvar(2)

    insert(Equal(list(al,be,tree.lhs(1)),Funcall("eig",1,tree.rhs,list(al,be,tree.lhs(1)))))

    tree=Operation("./",list(al,be),list(tree.lhs(1)))
    tree=Funcall("diag",1,list(tree),tree.out)

    tree.lhs(1).dims=list(A.dims(1),A.dims(1))
    tree.lhs(1).type=Type(Double,Unknown)
    tree.lhs(2).dims=list(A.dims(1),A.dims(1))
    tree.lhs(2).type=Type(Double,Unknown)
  end
// eig(A,B,flag)
else
  no_equiv(expression2code(tree)+", flag IGNORED")
  
  tree.rhs(3)=null()
  
  tree.lhs(1).dims=list(A.dims(1),A.dims(1))
  tree.lhs(1).type=Type(Double,Unknown)
  if lhs==2 then
    tree.lhs(2).dims=list(A.dims(1),A.dims(1))
    tree.lhs(2).type=Type(Double,Unknown)
  end
end

endfunction
