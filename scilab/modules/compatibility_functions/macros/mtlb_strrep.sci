// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2004 - INRIA - Vincent COUVERT
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function y=mtlb_strrep(s1,s2,s3)
    // Emulation function for strrep() Matlab function

    if type(s1)==10 & type(s2)==10 & type(s3)==10 then
        y=strsubst(s1,s2,s3)
    elseif type(s1)==10 | type(s2)==10 | type(s3)==10 then
        y=s1
    else
        y=matrix(s1,1,-1)
        y(y==s2)=s3
    end
endfunction
