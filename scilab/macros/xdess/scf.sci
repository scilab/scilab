function [f]=scf(h)
//set default figure, h may be a handle or a figure_id
// (small correction to set up win number 0 when no
//  gr window is opened, Bruno 22 nov 2004)
  if argn(2)<1 then 
     num=winsid()
     if num == [] then
	h = 0
     else
	h = max(num)+1
     end
  end
  set("current_figure",h);
  f=get("current_figure");
endfunction
