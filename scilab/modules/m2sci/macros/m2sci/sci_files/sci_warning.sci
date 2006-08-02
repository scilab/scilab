function [tree]=sci_warning(tree)
// Copyright INRIA
// M2SCI function
// Conversion function for Matlab warning()
// Input: tree = Matlab funcall tree
// Ouput: tree = Scilab equivalent for tree
// V.C.

// warning
if rhs==0 then
  // s = warning
  if lhs==1 then
    tree=Cste("on")
  // [s,f] = warning
  else
    insert(Equal(list(tree.lhs(1)),Cste("on")))
    insert(Equal(list(tree.lhs(2)),Cste("once")))
    tree=list()
  end
elseif rhs<=2 then
  if rhs==2 then
  // First arg is a msg_id
    set_infos("message_id ignored in following line",2)
    tree.rhs(1)=Cste(tree.rhs(1).value+" "+tree.rhs(2).value)
    tree.rhs(2)=null()
  end

  if typeof(tree.rhs(1))=="cste" & or(convstr(tree.rhs(1).value)==["""on""","""off""","""backtrace""","""debug""","""once""","""always"""]) then
    if tree.lhs(1).name<>"ans" then
      no_equiv(expression2code(tree)+": Output value set to ""on"".")
      tree=Cste("on")
    else
      no_equiv(expression2code(tree))
    end
  end
// rhs >= 3
else
  no_equiv(expression2code(tree))
end
endfunction
