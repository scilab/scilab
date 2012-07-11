// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

this_file = pathconvert(TMPDIR+"/mputstr.txt",%F);

fd = mopen(this_file,"wt");
mputstr("Scilab",fd);
mclose(fd);

if mgetl(this_file) <> "Scilab" then pause,end

ierr = execstr('mputstr(""Scilab"",5)','errcatch');
if ierr <> 999 then pause,end

