// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

// @OBSOLETE

function []=xclip(x,y,w,h)
warnobsolete("clip_state and clip_box properties", "5.2")
// fixe une zone de clipping en cordonnees reelles
// (x,y,w,h) (Upper-Left,wide,Height)
//!
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
