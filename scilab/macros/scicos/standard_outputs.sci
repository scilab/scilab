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

  graphics=o.graphics
  model=o.model
  orig=graphics.orig;sz=graphics.sz;orient=graphics.flip;

  out=size(model.out,1);clkout=size(model.evtout,1);

  if orient then
    xo=orig(1)+sz(1)
    dx=xf/7
  else
    xo=orig(1)
    dx=-xf/7
  end

typ=[];

  // output port location
  if out==0 then
    x=[];y=[];
  else
    y=orig(2)+sz(2)-(sz(2)/(out+1))*(1:out)
    x=(xo+dx)*ones(y)
      for k=1:out
         if o.graphics.out_implicit==[] then
          typ=ones(x)
         else
            if o.graphics.out_implicit(k)=='E' then
                     typ=[typ ones(x(k))]
            elseif  o.graphics.out_implicit(k)=='I' then
                       typ=[typ 2*ones(x(k))]
            end
         end
      end
  end


  // clock output  port location
  if clkout<>0 then
    x=[x,orig(1)+(sz(1)/(clkout+1))*(1:clkout)]
    y=[y,(orig(2)-yf/7)*ones(1,clkout)]
    typ=[typ,-ones(1,clkout)]
  end
endfunction

