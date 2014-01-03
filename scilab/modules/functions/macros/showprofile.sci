// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
//               Samuel GOUGEON - 2013 : scores moved in heading columns
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function showprofile(fun)
    if argn(2) < 1 then
        error(sprintf(_("%s: Wrong number of input argument(s): %d expected.\n"), "showprofile", 1));
    end
    lst = macr2lst(fun)
    count = profile(lst)
    count(:,2) = round(100*count(:,2))/100
    count = string(count)

    txt = fun2string(lst,"fun")
    m = min(size(count,1),size(txt,1))
    txt = txt(1:m);
    count = count(1:m,:)
    txt = part(txt,1:max(length(txt)))
    txt2 = []
    for k = 1:3
        txt2 = txt2 + part(count(:,k),1:max(length(count(:,k))))+"|"
    end
    txt = "|"+txt2+txt
    write(%io(2),txt,"(a)")
endfunction
