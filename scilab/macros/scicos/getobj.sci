function [k,wh]=getobj(objs,pt)
// Copyright INRIA
n=size(objs)
wh=[];
x=pt(1);y=pt(2)
data=[]
k=[]
eps=4
for i=2:n //loop on objects
  o=objs(i)
  //if o(1)=='Block'|o(1)=='Text' then
  if typeof(o)=='Block'|typeof(o)=='Text' then
   // [orig,sz]=o(2)(1:2)
    [orig,sz]=(o.graphics.orig,o.graphics.sz)
    data=[(orig(1)-x)*(orig(1)+sz(1)-x),(orig(2)-y)*(orig(2)+sz(2)-y)]
    if data(1)<0&data(2)<0 then k=i,break,end
 // elseif o(1)=='Link' then
  elseif typeof(o)=='Link' then
    //xx=o(2);yy=o(3);
    xx=o.xx;yy=o.yy;
    [d,ptp,ind]=dist2polyline(xx,yy,pt)
    if d<eps then k=i,wh=ind,break,end
  end
end
endfunction
