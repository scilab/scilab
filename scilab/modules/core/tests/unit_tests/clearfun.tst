// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

//================================================
// test clearfun , funptr, newfun
//================================================
if clearfun('myfunctionname') <> %F then pause,end
//================================================
ptr = funptr('pwd');
newfun("MY_PWD",ptr);

if clearfun("MY_PWD") <> %T then pause,end
//================================================
