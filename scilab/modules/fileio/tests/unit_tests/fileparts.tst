// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- JVM NOT MANDATORY -->

//===============================
// Tests fileparts
//===============================
ierr = execstr("[path,fname,extension] = fileparts([]);","errcatch");
if ierr <> 999 then pause,end
//===============================
ierr = execstr("extension = fileparts(SCI+''/etc/scilab.start'',''rpr'');","errcatch");
if ierr <> 999 then pause,end
//===============================
ierr = execstr("extension = fileparts(SCI+''/etc/scilab.start'',10);","errcatch");
if ierr <> 999 then pause,end
//===============================
ierr = execstr("extension = fileparts(10,10);","errcatch");
if ierr <> 999 then pause,end
//===============================
[path,fname,extension] = fileparts(SCI+'/etc/scilab.start');
if (path <> pathconvert(SCI+'/etc/',%f,%f)) then pause,end
if (fname <> 'scilab') then pause,end
if (extension <> '.start') then pause,end
//===============================
r = fileparts(SCI+'/etc/scilab.start','path');
if (r <> pathconvert(SCI+'/etc/',%f,%f)) then pause,end
//===============================
r = fileparts(SCI+'/etc/scilab.start','fname');
if (r <> 'scilab') then pause,end
//===============================
r = fileparts(SCI+'/etc/scilab.start','extension');
if (r <> '.start') then pause,end
//===============================
