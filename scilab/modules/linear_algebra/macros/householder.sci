function u=householder(v,w)
//Syntax 
//u=householder(v [,w])
//Description
//given 2 column vectors v w of same size householder(v,w) returns a unitary 
//column vector u, such that (eye-2*u*u')*v is proportional to w.
//(eye-2*u*u') is the orthogonal Householder reflexion matrix 
//
// w default value is eye(v). In this case vector (eye-2*u*u')*v is the 
// vector  eye(v)*(+-norm(v))
// Copyright INRIA
  [lhs,rhs]=argn(0)
  if rhs<2 then w=eye(v),end
  a=-sqrt((v'*v)/(w'*w))    
  u=v+a*w 
  u=u/norm(u)
endfunction
