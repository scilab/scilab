// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ???? - INRIA - Scilab
// Copyright (C) ???? - ENPC
// Copyright (C) 2009 - DIGITEO - Allan CORNET
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function [] = playsnd(y, rate, bits, aplay)
    // play signal y at sample rate rate
    // bits is unused
    [lhs,rhs] = argn(0);

    // default values
    if (rhs <= 2) then
        bits = 16;
    end

    if (rhs <= 1) then
        rate = 22050;
    end

    if rhs > 2 then

        if type(aplay) <> 10 then
            error(msprintf(_("%s: Wrong type for input argument #%d: string expected.\n"),"playsnd",3));
        end

        if size(aplay,"*") <> 1 then
            error(msprintf(_("%s: Wrong size for input argument #%d: string expected.\n"),"playsnd",3));
        end

    else
        if rhs <= 3 then
            if getos() == "Darwin" then
                aplay = "afplay"
            else
                aplay = "aplay";
            end
        end
    end

    if getos() == "Windows" then
        savewave(TMPDIR+"/_playsnd_.wav", y, rate);
        PlaySound(TMPDIR+"/_playsnd_.wav");
        return
    else

        // We should use a external C library here
        if aplay <> "/dev/audio" then
            savewave(TMPDIR+"/_playsnd_.wav", y, rate);
            cmd = msprintf("%s  %s > /dev/null 2>&1", aplay, TMPDIR + "/_playsnd_.wav");
            [res, stat, stderr] = unix_g(cmd);
            if (stat <> 0) then
                error(msprintf(_("%s: Failed to play the sound with command: %s.\n"), "playsnd",  cmd));
            end

        else
            [fp, www] = mopen("/dev/audio","wb", 0);
            if www < 0 then
                fp = -1;
            end

            if fp == (-1) then
                warning(msprintf(_("%s: Audio capabilities not available.\n"),"playsnd"));
                return
            end

            mput(lin2mu(y),"uc",fp);
            mclose(fp);

        end
    end

endfunction
