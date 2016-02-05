//<-- CLI SHELL MODE -->
// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

old = beep();  
if beep('on') <> 'on' then pause,end;
if beep('off') <> 'off' then pause,end;
if beep() <> 'off' then pause,end;
beep(old);
