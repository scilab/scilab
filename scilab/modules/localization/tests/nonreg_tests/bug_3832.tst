// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- Non-regression test for bug 3832 -->
// <-- INTERACTIVE TEST -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=3832
//
// <-- Short Description -->
// setdefaultlanguage does nothing when preceded by setlanguage

if setlanguage('en_US') <> %t then pause,end
if setdefaultlanguage('en_US') <> %t then pause,end

if setlanguage('fr_FR') <> %t then pause,end
if setdefaultlanguage('fr_FR') <> %t then pause,end

// quit scilab and relaunch
// please verify that scilab language is french