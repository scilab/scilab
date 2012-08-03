// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2007-2008 - INRIA
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

//=================================================
if strtok('') <> '' then pause,end
//=================================================
if strtok('','') <> '' then pause,end
//=================================================
if strtok('','a') <> '' then pause,end
if strtok('a') <> '' then pause,end
//=================================================
TOKENS = [];
token = strtok("A string of ,,tokens and some  more tokens"," ,");
TOKENS = [TOKENS,token];
while( token <> '' )
  token = strtok(" ,");
  TOKENS = [TOKENS,token];
end
REF = 'A string of tokens and some more tokens ';
if strcat(TOKENS,' ')<> REF then pause,end
//=================================================
