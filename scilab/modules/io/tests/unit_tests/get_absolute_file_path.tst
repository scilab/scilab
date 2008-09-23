// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
warning('off');

ierr = execstr("get_absolute_file_path();","errcatch");
if ierr <> 77 then pause,end

ierr = execstr("get_absolute_file_path(''test.sce'',''test.sce'');","errcatch");
if ierr <> 77 then pause,end

current_dir = pwd()+ filesep();

a = mopen(TMPDIR+'test.sce','wt');
d1 = get_absolute_file_path('test.sce');
d2 = get_absolute_file_path('test-not-exists.sce');
mclose(a);

// file 'test.sce' closed then not found
d3 = get_absolute_file_path('test.sce');

if d1 <> TMPDIR then pause,end
if d2 <> current_dir then pause,end
if d3 <> current_dir then pause,end

warning('on');
