function mesh(varargin)
// Copyright INRIA
//
// Syntax:
// mesh(z)
// mesh(x,y,z)
// mesh(...,<GlobalProperty>)  
// mesh(...,<color>,<GlobalProperty>)  
// mesh(<axes_handle>,...)  
//
// MESH function draws white surfaces, it's based on surf function with the properties e.color_mode = color('white') and e.color_flag = 0.   
// Inputs
// x,y,z : matrices which define the grid
// F.B

// Copy RHS arguments
ListArg = varargin;

drawlater();
surf(ListArg(:));
e = gce();
e.color_mode = color('white');
e.color_flag = 0;
drawnow();
endfunction