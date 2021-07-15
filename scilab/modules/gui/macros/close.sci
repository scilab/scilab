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

    // Close a graphic figure, or delete an uimenu or an uicontrol,
    // or close editvar, browsevar, helpbrowser, or all xcos GUI

    // This function has been written for compatibility with old TK uicontrols/uimenus

    // handle has been given ?
    rhs = argn(2);

    if rhs < 1 then
        // No handle given so destroy current figure
        if ~isempty(winsid()) then
            delete(gcf());
        end
    else
        if type(h) == 9 then // Graphics handle
            delete(h);

        elseif type(h) == 1 then
            for i = 1:length(h)
                tmp = findobj("figure_id",h(i));
                if tmp <> [] then
                    delete(tmp);
                end
            end

        elseif type(h)==10
            for i = 1:size(h,"*")
                gui = h(i)
                select gui
                case "help"
                    jimport org.scilab.modules.gui.helpbrowser.ScilabHelpBrowser
                    hb = ScilabHelpBrowser.getHelpBrowserWithoutCreation()
                    if type(hb) <> 1
                        hb.close()
                    end

                case "editvar"
                    jimport org.scilab.modules.ui_data.variableeditor.ScilabVariableEditor
                    ve = ScilabVariableEditor.getVariableEditor()
                    if type(ve) <> 1    // <=>  ve <> []
                        ve.close()
                    end

                case "browsevar"
                    jimport org.scilab.modules.ui_data.variablebrowser.ScilabVariableBrowser
                    if ScilabVariableBrowser.isBrowseVarOpened()
                        vb = ScilabVariableBrowser.getVariableBrowser()
                        vb.close()
                    end

                case "xcos"  // ask for confirmation in case of unsaved changes
                    javaclasspath(SCI + "/modules/xcos/jar/org.scilab.modules.xcos.jar");
                    jimport org.scilab.modules.xcos.Xcos
                    try
                        // Closes all opened Xcos instances: ask for confirmation
                        Xcos.closeSession(%t)
                    end

                case "xcos!"  // Do not ask for confirmation in case of unsaved changes
                    javaclasspath(SCI + "/modules/xcos/jar/org.scilab.modules.xcos.jar");
                    jimport org.scilab.modules.xcos.Xcos
                    try
                        inst = Xcos.getInstance()
                        inst.closeXcosFromScilab()
                    end

                else
                    msg = _("%s: Argument #%d: Unknown GUI name ''%s''.\n")
                    warning(msprintf(msg, "close", 1, h(i)))
                end
            end
        else
            // Do not know what to do
            error(msprintf(gettext("%s: Wrong type for input argument #%d: A graphic handle or a real expected.\n"), "close", 1));
        end
    end
endfunction

