function [x1,y]=system(x0,f,g,h,q,r)
//<x1,y>=system(x0,f,g,h,q,r)
//define system macro which generates the next
//observation given the old state
//  x0 :Input state vector
//  f  :System matrix
//  g  :Input matrix
//  h  :Output matrix
//  q  :Input noise covariance matrix
//  r  :Output noise covariance matrix
//  x1 :Output state vector
//  y  :Output observation
//System recursively calculates
//
//     x1=f*x0+g*u
//      y=h*x0+v
//
//where u is distributed N(0,q)
//and v is distribute N(0,r).
//!
//author: C. Bunks  date: August 1988
// Copyright INRIA
rand('normal');
   q2=chol(q);
   r2=chol(r);
   u=q2'*rand(ones(x0));
   v=r2'*rand(ones(x0));
   x1=f*x0+g*u;
   y=h*x0+v;
endfunction
