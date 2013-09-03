// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function R=%st_c_st(M1,M2)

    d1=M1.dims;
    d2=M2.dims;
    if size(d1,"*")<>size(d2,"*") then
        error(msprintf(_("%s: In concatenation the number of dimensions for each component must match.\n"),"%st_c_st"));
    end
    F=getfield(1,M1)
    if or(F<>getfield(1,M2)) then
        error(msprintf(_("%s: Field names mismatch.\n"),"%st_c_st"));
    end
    kd=find(d1<>d2)

    kd(find(kd==2))=[]

    if kd<>[] then
        error(msprintf(_("%s: Inconsistent dimensions.\n"),"%st_c_st"));
    end
    newdim=d1;newdim(2)=d1(2)+d2(2)
    R=mlist(F,newdim)
    for k=3:size(F,"*") //loop on fields
        x1=getfield(k,M1),if type(x1)<>15 then x1=list(x1),end
        x2=getfield(k,M2),if type(x2)<>15 then x2=list(x2),end
        setfield(k,lstcat(x1,x2),R)
    end
endfunction
