//==========================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA - Jean-baptiste SILVY
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

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
    arobPos = strindex( cText, "@" ) ;

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
