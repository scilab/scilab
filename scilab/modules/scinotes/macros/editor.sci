// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2012 - Scilab Enterprises - Calixte DENIZET
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

// This function is designed to call the godd "editor" according the values in
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
