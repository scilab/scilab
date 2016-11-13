// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function L=dir(str)

    mask = int32(61440);
    dirtype = 16384;

    if argn(2) == 1 then
        if type(str) <> 10 then
            error(999, msprintf(_("%s: Wrong type for input argument #%d: string expected.\n"), "dir", 1));
        end

        files = listfiles(str);
        basepath = str;
    else
        files = listfiles();
        basepath = "";
    end

    n = size(files, "*");

    if n==0 then
        files = [];
        dt    = [];
        bytes = [];
        isd   = [];
    else
        if getos() == "Windows" then
            [tmp,k] = gsort(convstr(files, "l"), "lr", "i")
            files = files(k)
        else
            files = gsort(files, "lr", "i");
        end

        dt = zeros(n, 1);
        bytes = zeros(n, 1);
        isd(n, 1) = %f;

        for k=1:n
            if (basepath == "") then
                [x,ierr] = fileinfo(files(k));
            else
                [x,ierr] = fileinfo(basepath + "/" + files(k));
            end

            if x == [] & ierr== -1 then
                [x,ierr] = fileinfo(files(k));
            end

            if x <> [] then
                dt(k)    = x(6);
                bytes(k) = x(1);
                isd(k)   = (double(int32(x(2)) & mask) == dirtype);
            end
        end //for
    end //if n==0

    L = tlist(["dir", "name", "date", "bytes", "isdir"], files, dt, bytes, isd);
endfunction
