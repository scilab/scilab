function [tree]=sci_cla(tree)
// Copyright INRIA
// M2SCI function
// Conversion function for Matlab cla()
// Input: tree = Matlab funcall tree
// Ouput: tree = Scilab equivalent for tree
// V.C.

if rhs<1 then
  set_infos("All children will be deleted, no HandleVisibility property in Scilab graphics.",2);
end

// a=gca()
a=gettempvar()
insert(Equal(list(a),Funcall("gca",1,list(),list())))

tree.name="delete"
tree.rhs=list(Operation("ext",list(a,Cste("children")),list()))
endfunction
