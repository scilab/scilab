function [x,y,typ]=standard_outputs(o)
//get position of inputs ports and clock inputs port for a standard block
//  the output ports are located on the right (or left if tilded) vertical
//    side of the block, regularly located from bottom to top
//  the clock output ports are located on the bottom horizontal side
//     of the block, regularly located from left to right
//!
// Copyright INRIA
xf=60
yf=40
graphics=o(2)
model=o(3)
orig=graphics(1);sz=graphics(2);orient=graphics(3);
out=size(model(3),1);clkout=size(model(5),1);


if orient then
  xo=orig(1)+sz(1)
  dx=xf/7
else
  xo=orig(1)
  dx=-xf/7
end


// output port location
if out==0 then
  x=[];y=[],typ=[]
else
  y=orig(2)+sz(2)-(sz(2)/(out+1))*(1:out)
  x=(xo+dx)*ones(y)
  typ=ones(x)
end


// clock output  port location
if clkout<>0 then
  x=[x,orig(1)+(sz(1)/(clkout+1))*(1:clkout)]
  y=[y,(orig(2)-yf/7)*ones(1,clkout)]
  typ=[typ,-ones(1,clkout)]
end


