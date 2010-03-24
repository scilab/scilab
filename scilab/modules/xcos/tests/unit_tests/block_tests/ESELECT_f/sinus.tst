// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 DIGITEO Clément DAVID
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- ENGLISH IMPOSED -->
// <-- XCOS TEST -->
//
// <-- Short Description -->
// This script validate the eselect functionnality with a sinus input.
// The associated diagram (sinus.xcos) is run and export data to the workspace.
// These data are then compared to the default.
//

// Import the reference datas (A_ref)
ierr = import_from_hdf5("sinus_ref.h5");
if ierr <> %t then pause; end

// Import the diagram structure
ierr = importXcosDiagram("sinus.xcos");
if ierr <> %t then pause; end

// launch the simulation
scicos_simulate(scs_m);

if norm(A_ref('values') - A('values')) / length(A_ref('values')) > 1e-6 then pause; end

