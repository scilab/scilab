function R=%s_l_s(A,B)
// used to give a sense to operation like A\1 where A is a matrix
  n=size(A,1)
  R=A\(eye(n,n)*B)
endfunction
