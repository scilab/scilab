//<-- CLI SHELL MODE -->
// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2005-2008 - INRIA -Pierre MARECHAL <pierre.marechal@inria.fr>
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- Non-regression test for bug 1239 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=1239
//
// <-- Short Description -->
//    This bug was found by Marek. He says us in the newsgroup:
//
//    It seems that nanmax, nanmin functions are not working well,
//    or at
//    least not according to documentation(they should supposedly
//    ignore %nan
//    values). I get the wollowing results in scilab 3.0:
//
//    -->nanmax([%nan, -1])
//     ans  = Inf
//
//    -->nanmax([%nan, 1])
//     ans  = Inf
//    The same for nanmin. 
//    ========================================================
//
//    Btw, on a scilab cvs I don't see any problem with nanmin.
//
//     Bruno

if( (nanmax([%nan, -1]) <> -1) | (nanmax([%nan, 1]) <> 1) | (nanmin([%nan, -1]) <> -1) | (nanmin([%nan, 1]) <> 1) ) then pause,end
