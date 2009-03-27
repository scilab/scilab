// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- JVM NOT MANDATORY -->

//===============================
d=dir('SCI/modules/core/macros/*.sci');
if d(2) == [] then pause,end
//===============================
if MSDOS then
  d = dir('c:\');
else
  d = dir('/');
end
if d(2) == [] then pause,end
//===============================
if MSDOS then
 TMP = getenv('TMP');
else
 if isdir('/tmp') then 
   TMP = '/tmp';
 else
   TMP = TMPDIR;
 end
 d = dir(TMP);
end
if d(2) == [] then pause,end
//===============================
