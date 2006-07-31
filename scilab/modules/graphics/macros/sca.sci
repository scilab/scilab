function [a]=sca(h)
//set default axes, h must be a handle
  set("current_axes",h);
  a=get("current_axes");
endfunction
