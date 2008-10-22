// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

//mtlb_mode

mtlb_mode(%t);
if ~mtlb_mode() then pause,end
if []+1<>[] then pause,end
mtlb_mode(%f);
if mtlb_mode() then pause,end
