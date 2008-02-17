// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

//===============================
// Tests fileparts
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
