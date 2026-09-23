// Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
//
// Copyright (C) 2024 - Dassault Systèmes S.E. - Antoine ELIAS
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function varargout = scilab(code, file, mode, quit, args, background)
    if isdef("code", "l") && isdef("file", "l") then
        error(sprintf(_("%s: Wrong input arguments: ''code'' and ''file'' cannot be both provided.\n"), "scilab"));
    end

    if ~isdef("code", "l") && ~isdef("file", "l") then
        error(sprintf(_("%s: Wrong input arguments: ''code'' or ''file'' must be provided.\n"), "scilab"));
    end

    if ~isdef("code", "l") then
        code = [];
    end

    if ~isdef("file", "l") then
        file = [];
    else
        if isfile(file) == %f then
            error(sprintf(_("%s: Wrong value for ''%s'' input argument: an existing file ""%s"" expected.\n"), "scilab", "file", file));
        end

        if getos() == "Windows" then
            file = sprintf("""%s""", file);
        end
    end

    if ~isdef("mode", "l") then
        mode = "nw";
    else
        if ~or(mode == ["nw" "nwni"])then
            error(sprintf(_("%s: Wrong value for ''%s'' input argument: must be in the set {%s}.\n"), "scilab", "mode", "''nw'', ''nwni''"));
        end
    end

    if ~isdef("quit", "l") then
        quit = "-quit";
    else
        if type(quit) <> 4 then
            error(sprintf(_("%s: Wrong type for ''%s'' input argument: a boolean expected.\n"), "scilab", "quit"));
        end

        if quit then
            quit = "-quit";
        else
            quit = "";
        end
    end

    if ~isdef("args", "l") then
        args = "";
    end

    if ~isdef("background", "l") then
        background = %f;
    else
        if type(background) <> 4 then
            error(sprintf(_("%s: Wrong type for ''%s'' input argument: a boolean expected.\n"), "scilab", "background"));
        end

        if background then
            if nargout <> 0 then
                error(sprintf(_("%s: Wrong number of output argument(s): %d expected.\n"), "scilab", 0));
            end
        end
    end

    start = "";
    if getos() == "Windows" then
        if background then
            start = "start /B "
        end

        if mode == "nwni" then
            bin = "scilex"
        else
            bin = "wscilex-cli"
        end

        bin = fullfile(SCI, "bin", bin);
    else
        if background then
            args = args + " &";
        end

        if mode == "nwni" then
            bin = "scilab-cli";
        else
            bin = "scilab-adv-cli";
        end

        // Scilab installed vs built
        path = strsplit(SCI, "share/scilab")(1);
        bin = fullfile(path, "bin", bin);
    end

    if code <> [] then
        code = strsubst(code, """", "\""");
        code = strsubst(code, "''", "\''");
        cmd = sprintf("%s%s -nb %s -e ""%s"" %s", start, bin, quit, code, args);
    else
        cmd = sprintf("%s%s -nb %s -f %s %s", start, bin, quit, file, args);
    end

    if background then
        host(cmd);
        varargout = list();
    else
        [varargout(1), varargout(2), varargout(3)] = host(cmd);
    end
endfunction
