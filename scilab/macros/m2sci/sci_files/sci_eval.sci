function [tree]=sci_eval(tree)
// Copyright INRIA
// M2SCI function
// Conversion function for Matlab eval()
// Input: tree = Matlab funcall tree
// Ouput: tree = Scilab equivalent for tree
// Emulation function: mtlb_eval()
// V.C.

if lhs==1 then
  // eval(expression)
  if rhs==1 then
    tree.name="mtlb_eval"
    repl1=tree
    repl1.name="evstr"
    repl2=tree
    repl2.name="execstr"
    if tree.lhs(1).name<>"" then // Value returned in a value
      repl_poss(tree,..
	  repl1,tree.rhs,"is a valid Scilab instruction")
    else
      repl_poss(tree,..
	  repl1,tree.rhs,"is a valid Scilab expression",..
	  repl2,tree.rhs,"is a valid Scilab instruction")
    end
  // eval(expression,catch_expr)
  else
    tree.name="mtlb_eval"

    set_infos([expression2code(tree)+" can be replaced by";
       "if execstr("+expression2code(tree.rhs(1))+",""errcatch"")<>0 then";
        "  execstr("+expression2code(tree.rhs(2))+")";
        "end";
        "If "+expression2code(tree.rhs(1))+" and "+expression2code(tree.rhs(2))+" are valid Scilab instructions"],1)
  end
else
  tree.name="mtlb_eval"

  // lhs names
  LHS=[]
  for k=1:lhs
    LHS=[LHS,tree.lhs(k).name]
  end
  LHS="["+strcat(LHS,",")+"]"
  if typeof(tree.rhs(1))=="cste" then
    tree.rhs=Rhs(LHS+" = "+tree.rhs(1).value)
  else
    tree.rhs=Rhs(LHS+" = "+expression2code(tree.rhs(1)))
  end
  tree.lhs=list(Variable("ans",Infer()))
  if rhs==1 then
    repl2=tree
    repl2.name="execstr"
    repl_poss(tree,..
	repl2,tree.rhs,"is a valid Scilab instruction")
  else
    set_infos([expression2code(tree)+" can be replaced by";
        "if execstr("+expression2code(tree.rhs(1))+",""errcatch"")<>0 then";
        "  execstr("+expression2code(tree.rhs(2))+")";
        "end";
        "If "+expression2code(tree.rhs(1))+" and "+expression2code(tree.rhs(2))+" are valid Scilab instructions"],1)
  end
end
endfunction
