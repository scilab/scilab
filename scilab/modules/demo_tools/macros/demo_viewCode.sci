//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Pierre MARECHAL
// Copyright (C) 2010 - DIGITEO - Allan CORNET
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.
//

function demo_viewCode(this_script)

    if (~isdef("editor") & (funptr("editor")==0)) then
        return
    end

    if isfile(this_script) then
        path = fullpath(this_script);
    else
        path = get_absolute_file_path(this_script) + this_script;
    end

    // Récupération du handle sur la fenêtre courante
    my_fig = gcf();

    //Création du nouveau menu
    if getos() == "Darwin" then
        my_menu = uimenu( "parent", my_fig, ..
        "label"       , gettext(" -- View Code -- "));
        my_submenu = uimenu("parent", my_menu , ..
        "label"       , gettext("Open in Editor"), ..
        "Foregroundcolor" , [ 0/255 81/255 6/255 ]     , ..
        "callback"    , "editor("""+path+""", ""readonly"")" );
    else
        my_menu = uimenu( "parent"      , my_fig             , ..
        "label"       , gettext(" -- View Code -- ") , ..
        "Foregroundcolor" , [ 0/255 81/255 6/255 ]     , ..
        "callback"    , "editor("""+path+""", ""readonly"")" );
    end
endfunction

