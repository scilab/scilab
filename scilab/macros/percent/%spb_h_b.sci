function r=%spb_h_b(a,b)
//  perform logical elementwise and a&b where a is a boolean sparse matrix 
//  and b a boolean matrix
// Copyright INRIA
r=a&sparse(b)
endfunction
