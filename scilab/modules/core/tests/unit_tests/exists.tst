// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

//exists

a=1
if exists('a')<>1 then pause,end
clear a
if exists('a')<>0 then pause,end
