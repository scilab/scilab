
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2007-2008 - INRIA - Pierre MARECHAL <pierre.marechal@inria.fr>
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function demo_begin()

    // Sauvegarde du mode
    demo_save_mode  = mode();

    // Sauvegarde de la pagination
    demo_save_lines = lines();

    // Sauvegarde des variables locales
    local_variables  = who("local");

    // Construction de la commande pour sauvegarder les variables locales
    local_cmd_str = "save(TMPDIR+''/who_local.dat''";
    for i=1:size(local_variables,"r")
        local_cmd_str = local_cmd_str + "," + local_variables(i);
    end
    local_cmd_str = local_cmd_str + ");";

    // Sauvegarde des variables locales dans le fichier TMPDIR/who_local.dat
    execstr(local_cmd_str);

    // Suppression des variables afin d'avoir un environnement propre pour les
    // Démos
    clear

    // Suppression de la pagination
    lines(0);

endfunction
