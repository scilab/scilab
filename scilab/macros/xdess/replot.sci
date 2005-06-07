function replot(rect,handl)
// replots the graphic window win 
// using rect as x and y bounds 
// Copyright INRIA

[lhs,rhs]=argn(0);

win=xget("window");

isold=get('figure_style')=='old'

if isold
  xclear();xtape('replaysc',win,rect);
else
  
  if rhs == 1
    a=gca();
  else
    if (type(handl) == 9)
      if (handl.type == 'Axes')
	a = handl;
      else
	error("Error : handle must be of type Axes");
      end
    else
      error("Error : Incorrect input, second argument must be of type graphic Axes handle");
    end
  end
  a.data_bounds = [rect(1) rect(2) ; rect(3) rect(4)]
end
endfunction
