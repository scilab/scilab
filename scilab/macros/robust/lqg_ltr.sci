function [kf,kc]=lqg_ltr(sl,mu,ro)
// [kf,kc]=lqg_ltr(sl,mu,ro) returns the Kalman gains for
//           .
//           x = a*x + b*u + l*w1   
//  (sl)
//           y = c*x + mu*I*w2
//
//           z = h*x
//
// Cost function:
//                _inf
//      J    = E(/ [z(t)'*z(t) + ro**2*u(t)'*u(t)]dt)
//       lqg    /
//            -0
// The lqg/ltr approach looks for L,mu,H,ro such that:
//J(lqg) = J(freq) where
//               _+oo       *  *           *
//      J    =  / tr[S  W  W  S ] + tr[T  T]dw
//       freq  /
//           -0
// and 
// S = (I + G.K)^(-1)  
// T = G.K.(I+G.K)^(-1) 
// Inputs:
//-- sl linear system in state-space form (syslin list)
//-- mu , ro real positive numbers chosen ``small enough''
//outputs:
//-- kf , kc = controller and observer Kalman gains.
//!
// Copyright INRIA
[m,p]=size(sl);
     [a,b,c,d]=abcd(sl);
     r1=c'*c,r2=ro**2*eye(p,p),
     q=b*b',r=mu**2*eye(m,m),
//     kc=lqr(a,b,r1,r2,'c'),
//     kf=lqe(a,c,q,r,'c')
     kc=lqr(syslin('c',a,b,c,ro*eye(p,p)));
     kf=lqe(syslin('c',a,b,c,mu*eye(m,m)));
endfunction
