
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

function demo_end()

    // Suppression des variables utilisées durant la démonstration
    clear;

    // Récupération de l'environnement
    load(TMPDIR+"/who_local.dat");

    // Suppression du fichier de sauvegarde de l'environnement
    mdelete(TMPDIR+"/who_local.dat");

    // Récupération de la pagination
    lines(demo_save_lines);

    // Récupération du mode
    mode(demo_save_mode);

    // Suppression de ces deux variables unitiles
    clear demo_save_lines;
    clear demo_save_mode;

endfunction
