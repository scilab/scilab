function k=getblock(objs,pt)
// Copyright INRIA
n=size(objs)
xf=60;
yf=40;
x=pt(1);y=pt(2)
data=[]
for i=n:-1:2
  k=i
  o=objs(i)
  if o(1)=='Block' then
    [orig,sz]=o(2)(1:2);dx=xf/7;dy=yf/7
    data=[(orig(1)-dx-x)*(orig(1)+sz(1)+dx-x),..
	  (orig(2)-dy-y)*(orig(2)+sz(2)+dy-y)]
    if data(1)<0&data(2)<0 then return,end
  end
end
k=[]


