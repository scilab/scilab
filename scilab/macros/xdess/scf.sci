function [f]=scf(h)
//set default figure, h may be a handle or a figure_id
  if argn(2)<1 then  h=max(winsid())+1,end
  set("current_figure",h);
  f=get("current_figure");
endfunction
