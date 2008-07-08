// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
if MSDOS then
  info = dllinfo(SCI+'\bin\scilex.exe','machine');
  if info(2) <> 'x86' & info(2) <> 'x64' then pause,end
  
  info = dllinfo(SCI+'\bin\libscilab.dll','machine');
  if info(2) <> 'x86' & info(2) <> 'x64' then pause,end
  
  info = dllinfo(SCI+'\bin\scilex.exe','exports');
  if size(info) <> 0 then pause,end
  
  info = dllinfo(SCI+'\bin\libscilab.dll','exports');
  if info(1) <> 'libscilab.dll' then pause,end
  if size(info(2),'*') < 500 then pause,end
  if type(info(2)(1)) <> 10 then pause,end
  
  info = dllinfo(SCI+'\bin\scilex.exe','imports');
  if size(info) < 4 then pause,end
  
  info = dllinfo(SCI+'\bin\libscilab.dll','imports');
  if size(info) < 4 then pause,end
  
else
  // nothing to do
end
