function [x,y,typ]=getoutputports(o)
// Copyright INRIA
  [x,y,typ]=getoutputs(o)
  [x2,y2,typ2]=getinputs(o)

  k=find(typ2==2)

  x=[x x2(k)]
  y=[y y2(k)]
  typ=[typ typ2(k)]
endfunction
