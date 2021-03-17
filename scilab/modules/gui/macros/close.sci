// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Vincent COUVERT
// Copyright (C) 2012 - 2016 - Scilab Enterprises
// Copyright (C) 2020 - Samuel GOUGEON
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function close(h)

    // Close a graphic figure or delete an uimenu or an uicontrol
    // This function has been written for compatibility with old TK uicontrols/uimenus

    // handle has been given ?
    rhs=argn(2);

    if rhs < 1 then
        // No handle given so destroy current figure
        if ~isempty(winsid()) then
            delete(gcf());
        end
    else
        if type(h) == 9 then // Graphics handle
            delete(h)

        elseif type(h) == 1 then // vector of Figures ids
            for i = 1:length(h)
                tmp = findobj("figure_id",h(i));
                if tmp <> [] then
                    delete(tmp);
                end
            end

        else
            // Do not know what to do
            error(msprintf(gettext("%s: Wrong type for input argument #%d: A graphic handle or a real expected.\n"), "close", 1));
        end
    end
endfunction
