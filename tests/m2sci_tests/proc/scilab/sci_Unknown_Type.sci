function [tree]=sci_Unknown_Type(tree)
// Copyright INRIA
// Variable type is set to Unknown
tree.lhs(1).dims=tree.rhs(1).dims
tree.lhs(1).type=Type(Unknown,Unknown)
endfunction
