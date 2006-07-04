function tree=change_tree_elt(tree,path,value)
// tree  : a recursive list
// path  : a vector of index giving top to bottom path
// value : new value for the pointed tree element
// Copyright INRIA
n=prod(size(path))
if n==1 then tree(path)=value,return,end
kk=path(1)
tree(kk)=change_tree_elt(tree(kk),path(2:n),value)
endfunction
