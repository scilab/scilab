// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA - Michael Baudin
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

utdir = SCI+"/modules/optimization/tests/unit_tests/optimbase"
exec(utdir+"/optimbase_checkbounds.tst")
exec(utdir+"/optimbase_checkx0.tst")
exec(utdir+"/optimbase_hasbounds.tst")
exec(utdir+"/optimbase_isfeasible.tst")
exec(utdir+"/optimbase_newdestroy.tst")
exec(utdir+"/optimbase_proj2bnds.tst")

