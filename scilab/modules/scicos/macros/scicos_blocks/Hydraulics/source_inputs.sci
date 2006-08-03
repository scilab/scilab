function [x,y,typ]=source_inputs(o)
// Copyright INRIA
xf=60
yf=40
[orig,sz,orient]=(o.graphics.orig,o.graphics.sz,o.graphics.flip)
//[orig,sz,orient]=o(2)(1:3);
  inp=size(o.model.in,1);clkinp=size(o.model.evtin,1);

if orient then
  x1=orig(1)
  dx1=-xf/7
  x2=orig(1)+sz(1)
  dx2=xf/7
  x3=orig(1)+2*sz(1)/7
else
  x1=orig(1)+sz(1)
  dx1=xf/7
  x2=orig(1)
  dx2=-xf/7
  x3=orig(1)+5*sz(1)/7
end

//y=[orig(2)+sz(2)-(sz(2)/2) ,orig(2)+yf/7+sz(2)]
y=[orig(2)+sz(2)+yf/7  orig(2)+sz(2)/2  orig(2)-yf/7]
x=[x3  (x1+dx1)  x3]
typ=[1 1 1]

endfunction
