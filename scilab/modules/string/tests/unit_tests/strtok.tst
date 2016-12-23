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
REF = "A string of tokens and some more tokens ";
TOKENS = [];

A = "A string of ,,tokens and some  more tokens";
token = strtok(A, " ,");
TOKENS = [TOKENS token];
while token <> ''
  token = strtok(" ,");
  TOKENS = [TOKENS token];
end
if strcat(TOKENS, " ")<> REF then pause,end
if A <> "A string of ,,tokens and some  more tokens" then pause, end
//=================================================
