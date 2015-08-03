// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2007-2008 - INRIA
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

//===============================
// unit tests strcmp
//===============================
TXT1 = ['scilab','SciLab';'Strcmp','STRcmp'];
TXT2 = ['ScIlAb','sciLab';'sTrCmP','StrCMP'];
TXT = 'Scilab';
//===============================
if strcmp(TXT1,TXT2) <> [ 1 , -1 ; -1 , -1 ] then pause,end
//===============================
if strcmp(TXT1,TXT2,'s') <> [ 1 , -1 ; -1 , -1 ] then pause,end
//===============================
if strcmp(TXT1,TXT2,'i') <> [ 0 , 0 ; 0 , 0 ] then pause,end
//===============================
if strcmp(TXT1,TXT1,'i') <> [ 0 , 0 ; 0 , 0 ] then pause,end
//===============================
if strcmp(TXT2,TXT2,'s') <> [ 0 , 0 ; 0 , 0 ] then pause,end
//===============================
if strcmp(TXT1,TXT,'s') <> [ 1 , -1 ; 1 , -1 ] then pause,end
//===============================
if strcmp(TXT1,TXT,'i') <> [ 0 , 0 ; 0 , 0 ] then pause,end
//===============================
if strcmp('','','i') <> 0 then pause,end
//===============================
if strcmp('','','s') <> 0 then pause,end
//===============================
