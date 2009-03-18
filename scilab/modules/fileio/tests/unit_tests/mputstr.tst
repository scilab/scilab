// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

fd = mopen(TMPDIR+'/mputstr.txt','wt');
mputstr('Scilab',fd);
mclose(fd)

fd = mopen(TMPDIR+'/mputstr.txt','rt');
l = mgetstr(fd);
mclose(fd);
if l <> 'Scilab' then pause,end