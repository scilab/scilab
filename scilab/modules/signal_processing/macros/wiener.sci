function [xs,ps,xf,pf]=wiener(y,x0,p0,f,g,h,q,r)
//<xs,ps,xf,pf>=wiener(y,x0,p0,f,g,h,q,r)
//macro which gives the Wiener estimate using
//the forward-backward kalman filter formulation
//Input to the macro is:
//  f,g,h     :system matrices in the interval [t0,tf]
//  q,r       :covariance matrices of dynamics
//            :and observation noise
//  x0,p0     :initial state estimate and error variance
//  y         :observations in the interval [t0,tf]
//Output from macro is:
//  xs        :Smoothed state estimate
//  ps        :Error covariance of smoothed estimate
//  xf        :Filtered state estimate
//  pf        :Error covariance of filtered estimate
//form of y:          [y0,y1,...,yf], and yk is a column m-vector
//form matrix inputs: [f0,f1,...,ff], and fk is a nxn matrix
//                    [g0,g1,...,gf], and gk is a nxn matrix
//                    [h0,h1,...,hf], and hk is a mxn matrix
//                    [q0,q1,...,qf], and qk is a nxn matrix
//                    [r0,r1,...,rf], and gk is a mxm matrix
//form of xs and xf:  [x0,x1,...,xf], and xk is a column n-vector
//form of ps and pf:  [p0,p1,...,pf], and pk is a nxn matrix
//!
//author: C. Bunks  date: 20 August 1988
// Copyright INRIA

//obtain the dimensions of xk and yk.
//Get the time interval [t0,tf].
 
   t0=1;
   [n,x0j]=size(x0);
   [m,tf]=size(y);
 
//run the forward kalman filter taking care to save the
//state estimate and forward error covariance matrices
 
   xf1=x0;
   pf1=p0;
   xf=[];
   pf=[];
   for k=t0:tf,
      ind_nk=1+(k-1)*n:k*n;
      ind_mk=1+(k-1)*m:k*m;
      yk=y(:,k);
      fk=f(:,ind_nk);
      gk=g(:,ind_nk);
      hk=h(:,ind_nk);
      qk=q(:,ind_nk);
      rk=r(:,ind_mk);
      [x1,p1,x,p]=kalm(yk,x0,p0,fk,gk,hk,qk,rk);
      xf1=[xf1 x1];
      pf1=[pf1 p1];
      xf=[xf x];
      pf=[pf p];
      x0=x1;
      p0=p1;
   end,
 
//run the backward kalman filter to smooth the estimate
 
   x2=x;
   p2=p;
   xs=x2;
   ps=p2;
   for k=tf-1:-1:t0,
      ind_nk=1+(k-1)*n:k*n;
      ind_nk1=1+k*n:(k+1)*n;
      ak=pf(:,ind_nk)*f(:,ind_nk)'*pf1(:,ind_nk1)**(-1);
      x2=xf(:,k)+ak*(x2-xf1(:,k+1));
      p2=pf(:,ind_nk)+ak*(p2-pf1(:,ind_nk1))*ak';
      xs=[x2 xs];
      ps=[p2 ps];
    end
endfunction
