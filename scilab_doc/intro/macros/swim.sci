function [t,t_p,ind]=swim(p,ind)
// Copyright INRIA
[d,d_p]=dpfun(p);
   as=sqrt(d^2+p^2);
   aw=sqrt((x-p)^2+(y-d)^2);
   t=as/vs+aw/vw;
   t_p=(p+d*d_p)/(vs*as)+((p-x)+(d-y)*d_p)/(vw*aw);


function [d,d_p]=dpfun(p);
   d=1.75-exp(log(1.55)*p);
   d_p=-log(1.55)*exp(log(1.55)*p);

