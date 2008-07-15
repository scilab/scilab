// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

//predef

N = predef();
a = 1;
predef("all");
errcatch(13,'continue','nomessage')
clear a
if iserror()<>1 then pause,end

errclear();
errcatch()
predef('clear');
errcatch(13,'continue','nomessage')
clear a
if iserror()<>0 then pause,end
	errclear();
	errcatch()
	predef(N);
	
if predef()<>N then pause,end
