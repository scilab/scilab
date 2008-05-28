//==========================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA - Jean-baptiste SILVY
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

// Desc   : Convert the string format defined by jpc into matrix of strings
//==========================================================================


function [strings] = arobasestring2strings( varargin )

// call : arobasestring2strings( string )

[lhs,rhs] = argn(0) ;

// check number of arguments
if ( rhs ~= 1 ) then
  error(msprintf(gettext("%s: Wrong number of input argument: %d expected.\n"),"arobasestring2strings",1));
  return ;
end

cText = varargin(1) ;

// check type
if ( type(cText) ~= 10 ) then
  error(msprintf(gettext("%s: Wrong type for input argument: String expected.\n"),"arobasestring2strings"));
  return ;
end

// check the size
if ( size( cText ) <> 1 ) then
  error(msprintf(gettext("%s: Wrong size for input argument #%d: A character string expected.\n"),"arobasestring2strings",1));
  return ;
end

// in the text, each line is separated by a '@' character
// in the resulting matrix, each line is put on a line
// of the matrix

// positions of @ in 
arobPos = strindex( cText, '@' ) ;

nbArob = size( arobPos ) ;
nbArob = nbArob(2);

// add the last character
arobPos( nbArob + 1 ) = length( cText ) + 1 ;

// get the first string
strings(1) = part( cText, [1:arobPos(1)-1] ) ;

for i = 1 : nbArob
  strings( i + 1 ) = part( cText, [ arobPos(i) + 1 : arobPos(i+1) - 1 ] ) ;
end

endfunction
