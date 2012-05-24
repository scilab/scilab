// ============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// ============================================================================

// <-- CLI SHELL MODE -->

// ============================================================================
// Unitary tests for basename function
// ============================================================================

files = basename('SCI/modules/fileio/macros/.myfile');
if files <> '.myfile' then pause,end

files = basename('SCI/modules/fileio/macros/poo.sci');
if files <> 'poo' then pause,end

files = basename('SCI/modules\fileio/macros/poo.sci');
if files <> 'poo' then pause,end

files = basename('SCI/modules\fileio/macros/poo.sci.k');
if files <> 'poo.sci' then pause,end

files = basename('');
if files <> '' then pause,end

files = basename('SCI/modules\.fileio/macros/.sci');
if files <> '.sci' then pause,end

if basename([]) <> [] then pause,end

ierr = execstr('basename(3)','errcatch');
if ierr <> 999 then pause,end

ierr = execstr('basename(SCI,1)','errcatch');
if ierr <> 999 then pause,end

ierr = execstr('basename(SCI,%t,1)','errcatch');
if ierr <> 999 then pause,end
