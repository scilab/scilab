
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2007-2008 - INRIA - Pierre MARECHAL <pierre.marechal@inria.fr>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

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
