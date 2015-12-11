// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function R=%st_c_st(M1,M2)

    d1=size(M1);
    d2=size(M2);
    if size(d1,"*")<>size(d2,"*") then
        error(msprintf(_("%s: In concatenation the number of dimensions for each component must match.\n"),"%st_c_st"));
    end

    F1=getfield(1,M1)
    F2=getfield(1,M2)
    if or(F1<>F2) then
        error(msprintf(_("%s: Field names mismatch.\n"),"%st_c_st"));
    end

    kd=find(d1<>d2)
    kd(find(kd==2))=[]
    if kd<>[] then
        error(msprintf(_("%s: Inconsistent dimensions.\n"),"%st_c_st"));
    end

    newdim=d1;newdim(2)=d1(2)+d2(2);

    R=struct();
    R(1:size(M1, "*")) = M1;
    s=size(R, "*");
    R((s+1):s+size(M2, "*")) = M2;
    R = matrix(R, newdim);
endfunction
