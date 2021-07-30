// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Vincent COUVERT
// Copyright (C) 2008 - DIGITEO - Vincent COUVERT
// Copyright (C) 2012 - 2016 - Scilab Enterprises
// Copyright (C) 2020 - 2021 - Stéphane MOTTELET
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function h =  findobj(varargin)

    rhs = argn(2);

    h = [];
    searchHandles= [];
    iPos = 1;
    if rhs >= 1
        if typeof(varargin(1)) == "handle"
            if and(is_handle_valid(varargin(1)))
                searchHandles = varargin(1);
            else
                msg = gettext("%s: Input argument #%d must be a valid handle.\n");
                error(msprintf(msg, "findobj", 1));
            end
            iPos = 2;
        elseif typeof(varargin(1) <> "string")
             msg = gettext("%s: Wrong type for argument #%d: A string expected.\n");
             error(msprintf(msg, "findobj", 1));
        end
    end

    if isempty(searchHandles)
        if rhs == 2 then
            if strcmp(varargin(1), "tag", "i") == 0 then
                h = get(varargin(2));
                return
            end
        end

        // Get all opened figures
        figureIds = winsid();
        if isempty(figureIds) then
            return
        end
        // Iterate over all figures
        currFigure = gcf();
        for figureindex = 1:size(figureIds,2)
            searchHandles(figureindex) = scf(figureIds(figureindex));
        end
        scf(currFigure);
    end

    try
        [testString,depth] = parse(varargin(iPos:$));
    catch
        error(lasterror())
    end

    h = findMatchingChild(searchHandles, testString, depth);

endfunction

function hFound = findMatchingChild(handles, testString, depth)
    hFound = []
    for index = 1:size(handles,1)
        h = handles(index);
        [bResult, ierr] = evstr(testString);
        if ierr==0 & bResult then
            hFound = [hFound; handles(index)];
        end
        if depth > 0
            [children,ierr] = evstr("get(handles(index), ""children"");"); // Does the child have a children property
            if get(handles(index),"type") == "Axes" // Title and Label entities of Axes are children
                children = [children
                            get(handles(index),"Title")
                            get(handles(index),"x_label")
                            get(handles(index),"y_label")
                            get(handles(index),"z_label")];
            end
            if ~isempty(children) // Yes the child can have children
                answ = findMatchingChild(children, testString, depth-1);
                if ~isempty(answ) then
                    hFound = [hFound; answ];
                end
            end
        end
    end
endfunction


function [out,depth] = parse(varargin)
    operators = ["&&","||","~" ]
    depth = %inf;
    out = "";
    currentOp = "";
    i = 1;
    while i <= length(varargin)
        op = operators(varargin(i) == ["-and","-or","-not"]);
        if ~isempty(op)
            // token is an operator
            if ~isempty(currentOp) && op <> "~"
               msg = gettext("%s: Argument #%d cannot be a binary operator.");
                error(msprintf(msg, "findobj", i));
            end
            currentOp = currentOp + op;
            if op <> "~" && isempty(out)
               msg = gettext("%s: Argument #%d cannot be a binary operator.");
                error(msprintf(msg, "findobj", i));
            end
            if i == length(varargin)
                msg = gettext("%s: Wrong number of input argument(s): At least %d expected.");
                error(msprintf(msg, "findobj", i+1));
            end
            i = i+1;
            continue
        end
        if ~isempty(out) && (isempty(currentOp) || currentOp == "~")
            // by default all successive conditions are connected with &&
            currentOp = "&&"+currentOp;
        end
        if typeof(varargin(i)) == "ce"
            // recursively parse content of cell
            cellArg = varargin(i);
            out = out + msprintf("%s(%s)",currentOp,parse(cellArg{:}(:)));
            currentOp = "";
            i = i+1;
        elseif varargin(i) == "-depth"
            // set depth
            if i < length(varargin)
                n = varargin(i+1);
                if type(n) == 1 && isscalar(n) && n >= 0 && floor(n) == n
                    depth = varargin(i+1);
                    i = i+2;
                else
                    msg = gettext("%s: Wrong type for input argument #%d: A positive integer value expected.\n");
                    error(msprintf(msg, "findobj", i+1));
                end
            else
                msg = gettext("%s: Wrong number of input argument(s): At least %d expected.");
                error(msprintf(msg, "findobj", i+1));
            end
        elseif varargin(i) == "-flat"
            // set depth at 0
            depth = 0;
            i = i+1;
        elseif varargin(i) == "-property"
            // search handles having a given property
            if i < length(varargin)
                if type(varargin(i+1)) == 10
                    out = out+msprintf("%s~execstr(""h.%s"",""errcatch"")",currentOp,varargin(i+1))
                    currentOp = "";
                    i = i+2;
                 else
                    msg = gettext("%s: Wrong type for argument #%d: A string expected.\n");
                    error(msprintf(msg, "findobj", i+1));
                 end
            else
                msg = gettext("%s: Wrong number of input argument(s): At least %d expected.");
                error(msprintf(msg, "findobj", i+1));
            end
        else // property, value sequence, likely
            if i < length(varargin)
                out = out+msprintf("%s(h.%s==%s)",currentOp,varargin(i),sci2exp(varargin(i+1)));
                currentOp = "";
                i = i+2;
            else
                msg = gettext("%s: Wrong number of input argument(s): At least %d expected.");
                error(msprintf(msg, "findobj", i+1));
            end
        end
    end
endfunction
