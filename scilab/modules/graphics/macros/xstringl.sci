// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Jean-Baptiste Silvy
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

//=======================================================================
// File   : xstringl.sci
// Desc   : Compute the bounding rectangle of a text
//          (old version of stringbox)
//======================================================================

function [rect] = xstringl( varargin )

// the call is rect=xstringl(x,y,str,[fontId,fontSize]) 

[lhs,rhs] = argn(0) ;
listArg = varargin ;

// rect = xstringl(x,y,str,[fontId,fontSize])

select rhs,
  case 3 then
    // stringbox(str, x, y)
    corners = stringbox(listArg (3), listArg (1), listArg (2));
  case 4 then
    // stringbox(str, x, y, angle = 0, fontId)
    corners = stringbox(listArg (3), listArg (1), listArg (2), 0, listArg (4));
  case 5 then
    // stringbox(str, x, y, angle = 0, fontId, fontSize);
    corners = stringbox(listArg (3), listArg (1), listArg (2), 0, listArg (4), listArg (5));
  else
    // incorrect number of input argmuments
    error(39);
    return ;
end;

// convert corners into position and size
// position, upper-left point
rect(1) = corners (1,2); // x
rect(2) = corners (2,2); // y

// size width, height
rect(3) = abs(corners(1,3) - corners(1,1));
rect(4) = abs(corners(2,3) - corners(2,1));


endfunction

