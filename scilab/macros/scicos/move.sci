function [x,y]=move(x,mx,y,my)
// Copyright INRIA
rep(3)=-1
xpolys(x,y,1)
xset('alufunction',6)
[btn,x0,y0]=xclick()
while rep(3)==-1 do
  rep=xgetmouse()
  xc=rep(1);yc=rep(2)
  xpolys(x,y,1)
  x=x+mx*(xc-x0)
  y=y+my*(yc-y0)
  xpolys(x,y,1)
  x0=xc;y0=yc
end
xset('alufunction',3)
endfunction
