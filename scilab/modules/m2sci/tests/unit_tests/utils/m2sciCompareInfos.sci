// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Vincent COUVERT <vincent.couvert@scilab.org>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
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
