// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2012 - Scilab Enterprises - Calixte DENIZET
// Copyright (C) 2015 - Samuel GOUGEON
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

// This function is designed to call the good "editor" according the values in
// the preferences file.
// This function can take any type of arguments.
function editor(varargin)
    if ~isfile(SCIHOME + "/XConfiguration.xml") then
        cmd = makeCommand("scinotes", varargin);
        execstr(cmd);
        return;
    end

    doc = xmlRead(SCIHOME + "/XConfiguration.xml");
    prefsNode = xmlXPath(doc, "//scinotes/body/scinotes-preferences");
    prefsNode = prefsNode(1);
    if prefsNode.attributes.scinotes == "true" then
        xmlDelete(doc);
        cmd = makeCommand("scinotes", varargin);
        execstr(cmd);
    elseif prefsNode.attributes("external-cmd") == "true" then
        cmd = prefsNode.attributes.cmd;
        xmlDelete(doc);
        if ~isempty(varargin) then
            cmd = cmd + " """ + string(varargin(1)) + """";
        end
        if getos()=="Windows"
            cmd = "start """" "+cmd
        else
            cmd = cmd + " &"
        end
        unix_w(cmd);
    else
        cmd = makeCommand(prefsNode.attributes.macro, varargin);
        xmlDelete(doc);
        execstr(cmd);
    end
endfunction

// Private function
// Params:
//   - name: the command name
//   - args: a list of arguments
// Return:
//   - cmd: the complete command to call with execstr
//
function cmd = makeCommand(name, args)
    cmd = name + "(";
    N = size(args);
    if N ~= 0 then
        for i=1:(N-1)
            arg = string(args(i));
            if type(args(i)) == 10 then
                arg = """" + arg + """";
            end
            cmd = cmd + arg + ",";
        end

        arg = string(args(N));
        if type(args(N)) == 10 then
            arg = """" + arg + """";
        end
        cmd = cmd + arg
    end
    cmd = cmd + ")";
endfunction

