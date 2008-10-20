function [tree]=sci_primes(tree)
// Copyright INRIA
// M2SCI function
// Conversion function for Matlab primes()
// Input: tree = Matlab funcall tree
// Ouput: tree = Scilab equivalent for tree
// F.B.

tree.lhs(1).dims(1)=1
tree.lhs(1).type=Type(Double,Real)
endfunction