// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->


// <-- Non-regression test for bug 3228 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=3228
//
// <-- Short Description -->
// ascii('é') returns a incorrect value

// UTF-8 encoding
if ascii('à') <> [195 160] then pause,end
if ascii('ç') <> [195 167] then pause,end
if ascii('è') <> [195 168] then pause,end
if ascii('é') <> [195 169] then pause,end
if ascii('ù') <> [195 185] then pause,end

if ascii([195 160]) <> 'à' then pause,end
if ascii([195 167]) <> 'ç' then pause,end
if ascii([195 168]) <> 'è' then pause,end
if ascii([195 169]) <> 'é' then pause,end
if ascii([195 185]) <> 'ù' then pause,end

if msprintf("%s", ascii(224)) <> ascii(224) then pause,end
if msprintf("%s", ascii([195 160])) <> ascii([195 160]) then pause,end
if msprintf("%s", ascii(231)) <> ascii(231) then pause,end
if msprintf("%s", ascii([195 167])) <> ascii([195 167]) then pause,end
if msprintf("%s", ascii(232)) <> ascii(232) then pause,end
if msprintf("%s", ascii([195 168])) <> ascii([195 168]) then pause,end
if msprintf("%s", ascii(233)) <> ascii(233) then pause,end
if msprintf("%s", ascii([195 169])) <> ascii([195 169]) then pause,end
if msprintf("%s", ascii(249)) <> ascii(249) then pause,end
if msprintf("%s", ascii([195 185])) <> ascii([195 185]) then pause,end
 