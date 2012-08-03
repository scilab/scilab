// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2007-2008 - INRIA
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

//===============================
// unit tests strcmpi
//===============================
TXT1 = ['scilab','SciLab';'Strcmp','STRcmp'];
TXT2 = ['ScIlAb','sciLab';'sTrCmP','StrCMP'];
TXT = 'Scilab';
//===============================
if strcmpi(TXT1,TXT2) <> [ 0 , 0 ; 0 , 0 ] then pause,end
//===============================
if strcmpi(TXT1,TXT) <> [ 0 , 0 ; 17 , 17 ] then pause,end
//===============================
if strcmpi('','') <> 0 then pause,end
//===============================
 