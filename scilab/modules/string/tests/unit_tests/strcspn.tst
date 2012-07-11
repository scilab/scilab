// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2007-2008 - INRIA
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

//===============================
if strcspn('fcba73','1234567890') <> 4 then pause,end
//===============================
if strcspn('fcba73','a') <> 3 then pause,end
//===============================
if strcspn('fcba73','') <> length('fcba73') then pause,end
//===============================
STR1 = 'Scilab';
STR2 = 'The Numerical Tool for Engineers';
STR = [STR1,STR2;STR2,STR1];
if strcspn(STR,'i') <> [ 2 , 9 ; 9 , 2 ] then pause,end
if strcspn(STR,'T') <> [ 6 , 0 ; 0 , 6 ] then pause,end
if strcspn(STR,['i','T';'u','a']) <> [ 2 , 0 ; 5 , 4 ] then pause,end
//===============================
STR1 = 'Scilab';
STR2 = 'The Numerical Tool for Engineers';
STR = [STR1,STR2];
if strcspn(STR,['i','T']) <> [ 2 , 0 ] then pause,end
//===============================
