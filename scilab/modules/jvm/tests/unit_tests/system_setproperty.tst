// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - DIGITEO
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

ierr = execstr('system_setproperty();','errcatch');
if ierr <> 77 then pause,end

ierr = execstr('a = system_setproperty(''TOTO'',2);','errcatch');
if ierr <> 999 then pause,end

r = system_getproperty('TOTO');
if r <> 'unknown' then pause,end

ierr = execstr('a = system_setproperty(''TOTO'',''TATA'');','errcatch');
if ierr <> 0 then pause,end
if a <> [] then pause,end
r = system_getproperty('TOTO');
if r <> 'TATA' then pause,end


ierr = execstr('a = system_setproperty(''TOTO'',''TITI'');','errcatch');
if ierr <> 0 then pause,end
if a <> 'TATA' then pause,end
r = system_getproperty('TOTO');
if r <> 'TITI' then pause,end
