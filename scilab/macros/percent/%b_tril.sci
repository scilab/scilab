function [d]=%b_tril(a,k)
// Copyright INRIA (modified by bruno, June 10 2004)
   [lhs,rhs] = argn()
   if rhs==1 then k=0,end
   [m,n] = size(a)
   d(m,n) = %f  // create an m x n boolean mat with %f elts
   i = find(tril(ones(a),k))
   d(i) = a(i)
endfunction
