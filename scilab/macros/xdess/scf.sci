function [f]=scf(h)
  //set default figure, h may be a handle or a figure_id
  set("current_figure",h);
  f=get("current_figure");
endfunction
