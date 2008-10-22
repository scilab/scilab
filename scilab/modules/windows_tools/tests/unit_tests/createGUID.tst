// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

res = createGUID(1);
if length(res) <> 36 then pause,end;
if size(tokens(res,'-'),'*') <> 5 then pause,end; 

res = createGUID(100);
if size(res,'*') <> 100 then pause,end;
