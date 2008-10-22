// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

fptr_cos = funptr("cos");
newfun("cosAlias",fptr_cos);

if cos(2*%pi) <> cosAlias(2*%pi)                then pause,end
if clearfun("cosAlias") <> %T                   then pause,end
if execstr("cosAlias(2*%pi)","errcatch") ==  0  then pause,end
