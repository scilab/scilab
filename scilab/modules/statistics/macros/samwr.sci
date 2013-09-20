// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2003 - INRIA - Carlos Klimann
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
//

function s=samwr(sizam,numsamp,X)
    //
    //This  function gives  a  s matrix  sizsam  x numsamp.   It
    //contains  numsamp  random samples  (the  columns) each  of
    //sizam  (<size(X,'*'))  extractions,  without  replacement,
    //from the  vector X.
    //
    if argn(2)<>3 then
        error(msprintf(gettext("%s: Wrong number of input argument: %d expected.\n"),"samwr",3)),
    end
    if X==[]|sizam==0|numsamp==0 then s=[]; return;end
    sizx=size(X,"*")
    if sizam>sizx then
        error(msprintf(gettext("%s: Wrong value of input argument #%d: Less or equal than length of last input argument expected.\n"),"samwr",1)),
    end

    perm=grand(numsamp,"prm",(1:sizx)')
    s=matrix(X(perm),-1,numsamp)
    s=s(1:sizam,:)
endfunction
