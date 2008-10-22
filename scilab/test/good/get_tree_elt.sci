function  value=get_tree_elt(tree,path)
// tree  : a recursive list
// path  : a vector of index giving top to bottom path
// value : new value for the pointed tree element
// Copyright INRIA
n=prod(size(path))


for k=1:n
  tree=tree(path(k))
end
value=tree
endfunction
