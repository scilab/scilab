function  tree=sci_pie(tree)
// Copyright INRIA
// M2SCI function
// Conversion function for Matlab pie()
// Input: tree = Matlab funcall tree
// Ouput: tree = Scilab equivalent for tree
// F.B.

global("m2sci_to_insert_a")
if tree.lhs(1).name <> "ans" then
 m2sci_to_insert_a($+1)=list("EOL")
 m2sci_to_insert_a($+1)=Equal(list(tree.lhs(1)),Funcall("gce",1,list(),list()))
 tree.lhs(1)=Variable("ans",tlist(["infer","dims","type","contents"],list(1,1),Type(Handle,Unknown),Contents()))
 else
 tree.lhs(1).type=Type(Handle,Unknown)
end
endfunction