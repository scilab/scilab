//==========================================================================
// Copyrigth INRIA 2005
// File   : arobasestring2strings.sci
// Author : Jean-Baptiste Silvy
// Desc   : Convert the string format defined by jpc into matrix of strings
//==========================================================================


function [strings] = arobasestring2strings( varargin )

// call : arobasestring2strings( string )

[lhs,rhs] = argn(0) ;

// check number of arguments
if ( rhs ~= 1 ) then
  error(39);
  return ;
end

cText = varargin(1) ;

// check type
if ( type(cText) ~= 10 ) then
  error(246) ;
  return ; 
end

// check the size
if ( size( cText ) <> 1 ) then
  error("function only works with an unique string");
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
