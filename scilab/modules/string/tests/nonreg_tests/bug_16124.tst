// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2019 - Samuel GOUGEON
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/16124
//
// <-- Short Description -->
// sci2exp({}) result could not be executed

assert_checkequal(sci2exp({}), "{}");
