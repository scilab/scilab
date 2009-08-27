// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- Non-regression test for bug 3580 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=3580
//
// <-- Short Description -->
// a gateway declaration containing a false primitive ID or gateway ID crashs scilab

// <-- INTERACTIVE TEST -->

// edit SCI/modules/fileio/sci_gateway/fileio_gateway.xml (or another .xml associated to a module)
// add new definitions

// <PRIMITIVE gatewayId="34" primitiveId="50" primitiveName="CRASH_BUG_3580_1" /> 
// <PRIMITIVE gatewayId="80" primitiveId="50" primitiveName="CRASH_BUG_3580_2" /> 

// launch scilab
// call at prompt

CRASH_BUG_3580_1

CRASH_BUG_3580_2

// scilab must return a error and not a crash
