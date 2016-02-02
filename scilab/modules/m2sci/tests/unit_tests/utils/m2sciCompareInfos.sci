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

function sameInfos = m2sciCompareInfos(varName, m2sciInfos, matInfos)

    sameInfos = %T;

    [m2sciSizeInfo, m2sciTypeInfo, m2sciPropInfo] = m2sciParseInfos(m2sciInfos);
    [matSizeInfo, matTypeInfo, matPropInfo] = m2sciParseInfos(matInfos);

    // Size comparison
    m2sciSizeInfoSave = m2sciSizeInfo;
    m2sciSizeInfo(m2sciSizeInfo==-1) = matSizeInfo(m2sciSizeInfo==-1);
    if or(m2sciSizeInfo<>matSizeInfo) then
        disp(varName + ": " + sci2exp(m2sciSizeInfoSave) + " <> " + sci2exp(matSizeInfo));
        sameInfos = %F;
        return
    end

    // Type comparison
    if m2sciTypeInfo<>matTypeInfo & m2sciTypeInfo<>"Unknown" then
        disp(varName + ": " + string(m2sciTypeInfo) + " <> " + string(matTypeInfo))
        sameInfos = %F;
        return
    end

    // Property comparison
    if m2sciPropInfo<>matPropInfo & m2sciPropInfo<>"Unknown" then
        disp(varName + ": " + string(m2sciPropInfo) + " <> " + string(matPropInfo))
        sameInfos = %F;
        return
    end

endfunction
