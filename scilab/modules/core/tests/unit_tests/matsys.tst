// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- NO TRY CATCH -->
// <-- ENGLISH IMPOSED -->

// test of matsys functions

// debug
// if debug()<>0 then pause,end
// debug(4)
// if debug()<>4 then pause,end
// debug(0)

//errcatch,errclear,iserror
clear
errcatch(4,'continue')
unknown
if iserror()<>1 then pause,end
errclear()
if iserror()<>0 then pause,end
unknown
if iserror(4)<>1 then pause,end
errclear(4)
if iserror(4)<>0 then pause,end
errcatch(4,'continue','nomessage')
unknown
if iserror()<>1 then pause,end
errclear()
if iserror()<>0 then pause,end
unknown
if iserror(4)<>1 then pause,end
errclear(4)
if iserror(4)<>0 then pause,end
errcatch()

//error
errcatch(4,'continue','nomessage')
error(4)
if iserror()<>1 then pause,end
errclear();errcatch()
errcatch(-1,'continue','nomessage')
error('test erreur');
errclear();
error('test erreur',999);
errclear();
errcatch()

// lasterror
errcatch(4,'continue','nomessage')
unknown
if stripblanks(lasterror(%f))<>"Undefined variable: unknown" then pause,end
[m,n]=lasterror();
if stripblanks(m)<>"Undefined variable: unknown" then pause,end
if n<>4 then pause,end
if lasterror()<>[] then pause,end
[m,n]=lasterror();
if m<>[]|n<>0 then pause,end




