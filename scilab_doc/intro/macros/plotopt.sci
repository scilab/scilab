function []=popt(ps)
// Copyright INRIA
px=0:x/25:x;
   dx=[];
   for k=1:maxi(size(px)),
      [d,d_p]=dpfun(px(k));
      dx(k)=d;
   end,
   plot2d(px',dx,[-1],"011",' ',[0,0,1,1],[4,5,4,5]);
   [ds,d_p]=dpfun(ps);
   plot2d([0 ps x]',[0 ds y]',[-2],"000"),


function [pr]=pg()
   s=poly(0,'s');
   ps=vw^2*s^2*((x-s)^2+(y-d)^2)-vs^2*(x-s)^2*(d^2+s^2);
   pr=roots(ps);

