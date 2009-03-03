// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - DIGITEO
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

a=amell(10,0.1);
if a > 3.693 then pause,end
if a < 3.692 then pause,end
if or(size(amell([1:10], 0.1)) <> [1 10]) then pause,end
if or(amell([1:10], 1)<> 0) then pause,end