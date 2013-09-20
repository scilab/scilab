// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2004 - INRIA - Vincent COUVERT
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

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
