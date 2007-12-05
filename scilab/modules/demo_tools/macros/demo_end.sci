// =============================================================================
// Author : Pierre MARECHAL
// Scilab Project
// Copyright INRIA 2007
// =============================================================================

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
