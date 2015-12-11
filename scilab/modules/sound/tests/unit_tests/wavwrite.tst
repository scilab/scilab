//<-- CLI SHELL MODE -->
// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

  A = matrix(1:6,2,3);
  wavwrite(A/6,TMPDIR+'/foo.wav');
  B = wavread(TMPDIR+'/foo.wav');
  if max(abs(A- round(B*6))) <> 0 then pause,end
