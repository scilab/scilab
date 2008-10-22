// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Pierre MARECHAL <pierre.marechal@inria.fr>
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->
//
// <-- Non-regression test for bug 2708 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=2708
//
// <-- Short Description -->
// It is no more possible to fill the user_data field of a graphic object with lists

a=gca();
set(a,'user_data',tlist(['gindex','node','edge'],[],[]));
if or( a.user_data <> tlist(['gindex','node','edge'],[],[]) ) then pause, end

set(a,'user_data',list());
if or( a.user_data <> list() ) then pause, end
