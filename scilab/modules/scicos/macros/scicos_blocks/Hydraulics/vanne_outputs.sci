function [x,y,typ]=vanne_outputs(o)
// Copyright INRIA
xf=60
yf=40
[orig,sz,orient]=(o.graphics.orig,o.graphics.sz,o.graphics.flip)
//[orig,sz,orient]=o(2)(1:3);
 out=size(o.model.out,1);clkout=size(o.model.evtout,1);
if orient then
  x1=orig(1)
  dx1=-xf/7
  x2=orig(1)+sz(1)
  dx2=xf/7
else
  x1=orig(1)+sz(1)
  dx1=yf/7
  x2=orig(1)
  dx2=-xf/7
end

y=[orig(2)+2*sz(2)/10]
x=[(x2+dx2) ]
typ=[2]

endfunction
