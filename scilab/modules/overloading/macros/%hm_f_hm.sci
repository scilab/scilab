// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function M1=%hm_f_hm(M1,M2)

    d1=size(M1);
    d2=size(M2);
    if size(d1,"*")<>size(d2,"*") then
        error(msprintf(_("%s: In concatenation the number of dimensions for each component must match.\n"),"%hm_f_hm"));
    end

    kd=find(d1(2:$)<>d2(2:$))
    if kd<>[] then
        error(msprintf(_("%s: Inconsistent dimensions.\n"),"%hm_f_hm"));
    end

    d3 = d1;
    d3(1) = d1(1) + d2(1);
    M1=matrix([matrix(M1,double(d1(1)),-1) ; matrix(M2,double(d2(1)),-1)], d3)
endfunction
