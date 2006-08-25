function sol=chsolve(spcho,rhs)
// Cholesky solver for A*sol=rhs (A is symmetric >0)
// 1rst step: spcho=chfact(A)
// 2nd step: sol=chsolve(spcho,rhs)
// Example: N=20; A=sprand(N,N,0.1);
// A=A*A'+speye(A);
// sol=(1:N)'; rhs=A*sol;
// spcho=chfact(A); sol=chsolve(spcho,rhs)
  perm=spcho('perm');
  sol=blkslv(spcho,rhs(perm));
  sol(perm)=sol
endfunction
