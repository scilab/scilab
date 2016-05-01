// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Vincent COUVERT <vincent.couvert@scilab.org>
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.
//

function [sizeInfo, typeInfo, propInfo] = m2sciParseInfos(strInfos)
    parts = tokens(strInfos, "|");

    if size(parts, "*") == 3 then
        // The name is not given
        sizeInfo = msscanf(parts(1), "%d %d");
        typeInfo = parts(2);
        propInfo = parts(3);
    else
        // The name is given
        sizeInfo = msscanf(parts(2), "%d %d");
        typeInfo = parts(3);
        propInfo = parts(4);

    end
endfunction
