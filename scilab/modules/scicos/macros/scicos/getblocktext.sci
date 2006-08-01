function k=getblocktext(scs_m,pt)
// Copyright INRIA
n=lstsize(scs_m.objs)
xf=60;
yf=40;
x=pt(1);y=pt(2)
data=[]
for i=n:-1:1
  k=i
  o=scs_m.objs(i)
  if typeof(o)=='Block'| typeof(o)=='Text' then
    [orig,sz]=(o.graphics.orig,o.graphics.sz);dx=xf/7;dy=yf/7
    data=[(orig(1)-dx-x)*(orig(1)+sz(1)+dx-x),..
	  (orig(2)-dy-y)*(orig(2)+sz(2)+dy-y)]
    if data(1)<0&data(2)<0 then return,end
  end
end
k=[]
endfunction
