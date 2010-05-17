// ============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Clément DAVID
//
//  This file is distributed under the same license as the Scilab package.
// ============================================================================

loadScicosLibs;
// Stubbing the x_mdialog method
function [result]=x_mdialog(dialog_title,labels,default_inputs_vector)
	result = default_inputs_vector;
endfunction

// variables
hdf5FileToLoad = TMPDIR + "/hdf5FileToLoad.h5";
hdf5FileToSave = TMPDIR + "/hdf5FileToSave.h5";
interfaceAlias = BIGSOM_f;
job = "set";
hdf5ContextFile = TMPDIR + "/hdf5ContextFile.h5";

// initialize the test
execstr("scs_m = interfaceAlias(''define'', [], [])");
export_to_hdf5(hdf5FileToLoad, "scs_m");
clear scs_m;
context = "";
export_to_hdf5(hdf5ContextFile, "context");

// run the test
xcosBlockInterface(hdf5FileToLoad, hdf5FileToSave, interfaceAlias, job, hdf5ContextFile)

// clear test variables
mdelete(hdf5FileToLoad);
mdelete(hdf5FileToSave);
mdelete(hdf5ContextFile);
clear hdf5FileToLoad hdf5FileToSave interfaceAlias job hdf5ContextFile;

