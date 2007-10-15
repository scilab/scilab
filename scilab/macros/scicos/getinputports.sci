function [x,y,typ]=getinputports(o)
// Copyright INRIA
  [x,y,typ]=getinputs(o)
  [x2,y2,typ2]=getoutputs(o)
  k=find(typ2==2)

  x=[x x2(k)]
  y=[y y2(k)]

  typ=[typ typ2(k)]
endfunction
