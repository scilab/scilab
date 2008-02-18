//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
//
// This file is distributed under the same license as the Scilab package.
//

function bezier3dtest ()
  expr=["// Show a Beziercurve of dimension 3"
	"p=[-1,-1,-1;0,-1,-1;1,0,0;1,1,0;0,1,1;-1,1,0]'';"
	"bezier3d(p);"];
  x_message(expr);
  execstr(expr);
endfunction

clf();
bezier3dtest();
