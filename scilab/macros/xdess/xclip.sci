function []=xclip(x,y,w,h)
// fixe une zone de clipping en cordonnees reelles
// (x,y,w,h) (Upper-Left,wide,Height)
//!
// Copyright INRIA
[lhs,rhs]=argn(0)
if rhs<=0, xset('clipoff');return;end
if rhs==1,if typeof(x)<>"string" then 
		xset('clipping',x(1),x(2),x(3),x(4));
	else 
		xset(x);
	end
else 
	xset('clipping',x,y,w,h);
end
endfunction
