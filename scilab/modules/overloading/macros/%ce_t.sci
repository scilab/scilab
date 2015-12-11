// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function R=%ce_t(M)
    D = size(M);
    if size(D,"*") > 2 then
        error(msprintf(_("%s: Wrong size for input argument #%d: Transpose can not be defined.\n"),"%ce_t",1));
    end
    R = makecell([D(2) D(1)], M{:}(:));
endfunction
