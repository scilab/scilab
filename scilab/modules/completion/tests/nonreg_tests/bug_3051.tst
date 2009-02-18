// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- Non-regression test for bug 3051 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=3051
//
// <-- Short Description -->
// 

// <-- INTERACTIVE TEST -->

gethi<Tab>

it should return :

Fonction Scilab:
gethistory gethistoryfile
-->gethistory

geth<Tab>

it should return :

Fonction Scilab:
gethistory gethistoryfile
-->gethistory
                    
                    
cd SCI/contrib/;
cd toolbo[TAB]

it should return :
cd toolbox_skeleton/
