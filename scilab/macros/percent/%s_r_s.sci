function R=%s_r_s(B,A)
// used to give a sense to operation like 1/A where A is a matrix
  n=size(A,2)
  R=(B*eye(n,n))/A
endfunction
