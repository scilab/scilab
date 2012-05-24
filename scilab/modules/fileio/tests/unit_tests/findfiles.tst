// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

cd('SCI/etc');
f1 = findfiles();
f2 = findfiles(SCI+'/etc');
if f1 <> f2 then pause,end
// =============================================================================
f3 = findfiles(SCI+'/etc','*.start');
if f3 <> 'scilab.start' then pause,end
// =============================================================================
f = findfiles();
f = findfiles(SCI);
if size(f,'*') == [] then pause,end
f = findfiles(SCI+'/modules/core/macros','*.sci');
if size(f,'*') == [] then pause,end
// =============================================================================