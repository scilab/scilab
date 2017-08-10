//<-- CLI SHELL MODE -->
// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
[a,b,c,d,e] = file(500);
if a <> [] then pause,end;
if b <> [] then pause,end;
if c <> [] then pause,end;
if d <> [] then pause,end;
if e <> [] then pause,end;
clear a b c d e
// =============================================================================
[a,b,c,d] = file(500);
if a <> [] then pause,end;
if b <> [] then pause,end;
if c <> [] then pause,end;
if d <> [] then pause,end;
clear a b c d 
// =============================================================================
[a,b,c] = file(500);
if a <> [] then pause,end;
if b <> [] then pause,end;
if c <> [] then pause,end;
clear a b c
// =============================================================================
[a,b] = file(500);
if a <> [] then pause,end;
if b <> [] then pause,end;
clear a b
// =============================================================================
[a] = file(500);
if a <> [] then pause,end;
clear a
// =============================================================================
file(500);
// =============================================================================
ierr = execstr("file(''toto'')","errcatch");
if (ierr <> 201) then pause,end
// =============================================================================

