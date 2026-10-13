//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) - 2013 - Scilab Enterprises - Vladislav Trubkin
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
//

//fmiModelDescription

function %fmiExper_p(model)
    fn=getfield(1,model)
    txt=[]
    for k=2:size(fn,"*")
            txt=[txt
            sci2exp(model(fn(k)),fn(k))]
    end
    write(%io(2),txt,"(a)")
endfunction
