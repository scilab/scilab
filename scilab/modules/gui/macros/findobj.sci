// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Vincent COUVERT
// Copyright (C) 2008 - DIGITEO - Vincent COUVERT
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.
function h =  findobj(propertyName, propertyValue)

    rhs = argn(2);
    if rhs<> [1 2] then
        error(msprintf(gettext("%s: Wrong number of input arguments: %d or %d expected.\n"), "findobj", 1, 2));
        return;
    end

    if rhs == 1 then
        h = get(propertyName);
        if h <> [] then
            return;
        end
    elseif strcmp(propertyName, "tag", "i") == 0 then
        h = get(propertyValue);
        if h <> [] then
            return;
        end
    end

    // Return value
    h = []

    // Get all opened figures
    figureIds = winsid();
    if isempty(figureIds) then
        return
    end

    currentFig = gcf();

    propertyName = convstr(propertyName,"l");

    // Iterate over all figures
    for figureindex = 1:size(figureIds,2)
        // Does the figure match the propertyName/propertyValue set ?
        f=scf(figureIds(figureindex));

        %ierr = execstr("%bool = isequal(get(f, propertyName), propertyValue);", "errcatch");
        if %ierr==0 & %bool then
            h = f;
            break
        end

        answ = findMatchingChild(f.children, propertyName, propertyValue);
        if ~isempty(answ) then
            h = answ;
            break
        end
    end

    scf(currentFig);

endfunction
//-----------------------------------------------------------------------------
function hFound = findMatchingChild(children, propertyName, propertyValue)
    // Copyright DIGITEO 2008
    // Vincent COUVERT

    hFound = []

    for childIndex = 1:size(children,1)

        %ierr = execstr("%bool = isequal(get(children(childIndex), propertyName), propertyValue);", "errcatch");
        if %ierr==0 & %bool then
            hFound = children(childIndex);
            return
        end

        %ierr = execstr("get(children(childIndex), ""children"");", "errcatch"); // Does the child have a children property

        if %ierr==0 then // Yes the child can have children
            answ = findMatchingChild(children(childIndex).children, propertyName, propertyValue);
            if ~isempty(answ) then
                hFound = answ;
                return
            end
        end
    end

endfunction
//-----------------------------------------------------------------------------
