// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA -
// Copyright (C) DIGITEO - 2010 - Allan CORNET
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt


function %i_mfprintf(id, fmt, varargin);

    if type(id) == 8 then
        id = int(id);
    end

    for k=1:size(varargin)
        if type(varargin(k)) == 8 then
            varargin(k) = double(varargin(k))
        end
    end

    mfprintf(id, fmt, varargin(:));

endfunction
