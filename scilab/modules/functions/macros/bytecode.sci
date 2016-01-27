// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2003-2008 - INRIA - Serge STEER <serge.steer@inria.fr>
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.
// === LICENSE_END ===

function x=bytecode(f)
    //returns the function f bytecode array in x
    //should be hard-coded

    if argn(2) < 1 then
        error(sprintf(_("%s: Wrong number of input argument(s): %d expected.\n"), "bytecode", 1));
    end

    nsiz=6
    if type(f)==8 then //bytecode to function
        u=mopen(TMPDIR+"/%fun.bin","wb");
        name_x=[673720353 673720360 673720360 673720360 673720360 673720360]
        mput(int32([name_x  13]),"i",u)
        mput(f,"i",u)
        mclose(u)
        load(TMPDIR+"/%fun.bin")
    elseif type(f)==13 then //function to bytecode
        warnMode = warning("query");
        warning("off");
        save(TMPDIR+"/%fun.bin",f)
        warning(warnMode);
        I=fileinfo(TMPDIR+"/%fun.bin");
        u=mopen(TMPDIR+"/%fun.bin","rb");
        mgeti(1+nsiz,"i",u);
        x=mgeti(I(1)-(1+nsiz),"i",u);
        mclose(u)
    else
        error(msprintf(gettext("%s: Wrong type for input argument #%d.\n"),"bytecode",1))
    end
endfunction
