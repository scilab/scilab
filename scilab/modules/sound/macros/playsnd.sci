// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ???? - INRIA - Scilab
// Copyright (C) ???? - ENPC
// Copyright (C) 2009 - DIGITEO - Allan CORNET
// Copyright (C) 2012 - 2016 - Scilab Enterprises
// Copyright (C) 2019 - Samuel GOUGEON
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function playsnd(y, rate, nbiter, playerCmd)

    [lhs,rhs] = argn(0);
    fname = "playsnd"
    File = ""
    if argn(2) < 1 | argn(2) > 4
        msg = _("%s: Wrong number of input arguments: %d or %d expected.\n")
        error(msprintf(msg, fname, 1, 4))
    end

    // PARSING INPUT ARGUMENTS and SETTING DEFAULT VALUES
    // ==================================================
    // rate / speed
    // ------------
    if ~isdef("rate","l") then
        if type(y)==1
            rate = 22050;
        else
            rate = 1
        end
    end

    // nbiter
    // ------
    iPlayerCmd = 4    // index of playerCmd (if any)
    if ~isdef("nbiter","l")
        nbiter = -1
        iPlayerCmd = 3
    else
        if and(type(nbiter)<>[1 10])
            msg = _("%s: Argument #%d: %s expected.\n")
            error(msprintf(msg, fname, 3, "Decimal numbers or Text"))
        end
        nbiter = nbiter(1)
        if type(nbiter)==10
            playerCmd = nbiter
            iPlayerCmd = 3
            nbiter = -1
        elseif nbiter < 1
            msg = _("%s: Argument #%d: Must be >= %d.\n")
            error(msprintf(msg, fname, 3, 1))
        else
            nbiter = int(nbiter)
        end
    end

    // playerCmd
    // ---------
    if ~isdef("playerCmd","l") then
        if getos() == "Darwin" then
            playerCmd = "afplay"
        else
            playerCmd = "aplay";
        end
    else
        if type(playerCmd) <> 10 then
            error(msprintf(_("%s: Wrong type for input argument #%d: string expected.\n"), fname, iPlayerCmd));
        end
        if size(playerCmd,"*") <> 1 then
            error(msprintf(_("%s: Wrong size for input argument #%d: string expected.\n"), fname, iPlayerCmd));
        end
    end

    // Input sound data
    // ----------------
    if and(type(y) <> [1 10]) then
        msg = _("%s: Argument #%d: %s expected.\n")
        error(msprintf(msg, fname, 1, "Decimal numbers or Text"))
    end
    if type(y)==10 then
        File = pathconvert(y(1),%f)
        if isdir(File)
            msg = _("%s: Argument #%d: ''%s'' is a directory. File expected.\n")
            error(msprintf(msg, fname, 1, File))
        end
        if ~isfile(File)
            msg = _("%s: Argument #%d: The file ''%s'' does not exist.\n")
            error(msprintf(msg, fname, 1, File))
        end
        if type(rate) <> 1
            msg = _("%s: Argument #%d: Decimal number expected.\n")
            error(msprintf(msg, fname, 2))
        end
        rate = rate(1)
        if rate <= 0
            msg = _("%s: Argument #%d: Must be > %d.\n")
            error(msprintf(msg, fname, 2, 0))
        end
        info = wavread(y,"info");
        // The only case where the file can be used as is without being
        // loaded is when the speed = 1 & OS = Linux | MacOS &
        // playerCmd <> "dev/audio"
        // Note : On linux and MacOS: aplay and afplay have a -r option
        //    but it does not work
        if getos()=="Windows" | rate <> 1 | grep(playerCmd,"/dev/audio")<>[]
            y = wavread(y);
        end
        rate = rate * info(3);
        duration = info(8) / rate   // [s]
    end

    // wav file
    // ========
    if type(y)==1 & y <> [] & grep(playerCmd,"/dev/audio")==[] then
        File = pathconvert("TMPDIR/_playsnd_.wav", %f)
        [fid, typ, names] = file();
        tmp = find(names==File,1)
        if tmp <> []
            mclose(fid(tmp))
        end
        savewave(File, y, rate);
        duration = size(y,2)/rate
    elseif y<>[] & isdef("File","l")
        info = wavread(File,"info");
        duration = info(8)/rate
    end

    // Play
    // ====
    if getos() == "Windows" then
        PlaySound("")
        if y <> []
            if nbiter==-1
                PlaySound(File) // Asynchronous
            else
                realtimeinit(1)
                realtime(0)
                for i = 1:nbiter
                    PlaySound(File);
                    realtime(i*duration)
                end
            end
        end
    else
        // We should use a external C library here
        if playerCmd <> "/dev/audio" then
            player = strtok(playerCmd, " ")
            options = strchr(playerCmd," ")
            unix("killall -9 " + player + " 2> /dev/null")
            //cmd = "%s --disable-resample -r %d %s ''%s'' > /dev/null 2>&1 &"
            cmd = "%s %s ''%s'' > /dev/null 2>&1 &"
            cmd = msprintf(cmd, player, options, File);
            if y <> []
                msg = msprintf(_("%s: Failed to play the sound with command: %s.\n"), fname,  cmd)
                if nbiter==-1
                    unix_g(cmd);
                else
                    realtimeinit(1)
                    realtime(0)
                    for i = 1:nbiter
                        unix_g(cmd);
                        realtime(i*duration)
                    end
                end
            end
        else
            [fp, www] = mopen("/dev/audio","wb", 0);
            if www < 0 then
                fp = -1;
            end
            if fp == -1 then
                warning(msprintf(_("%s: Audio capabilities not available.\n"), fname));
            else
                y = lin2mu(y)
                // Here: Don't know how to interrupt a running audio stream
                if nbiter==-1
                    mput(y,"uc",fp);
                else
                    realtimeinit(1)
                    realtime(0)
                    for i = 1:nbiter
                        mput(y,"uc",fp);
                        realtime(i*duration)
                    end
                end
                mclose(fp);
            end
        end
    end

    // Cleaning
    // ========
    sleep(100)
    deletefile(TMPDIR+"/_playsnd_.wav")
endfunction
