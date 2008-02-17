// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2007-2008 - INRIA
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

//===============================
// unit tests tokens
//===============================
if tokens('This is a character string') <> ['This'; 'is'; 'a'; 'character'; 'string'] then pause,end
if tokens('SCI/demos/scicos','/') <> ['SCI';'demos';'scicos'] then pause,end
if tokens('y=a+b*2',['=','+','*']) <> ['y';'a';'b';'2'] then pause,end
if length(tokens('',['=','+','*'])) <> 0 then pause,end
//===============================
if tokens(' ',' ') <> [] then pause,end
//===============================
s = tokens(' '+ascii(9)+' ',ascii(9));
if  s <> [' '; ' '] then pause,end
//===============================
s = tokens(' '+ascii(9)+' ',' ');
if  s <> ascii(9) then pause,end
//===============================
s = tokens(' '+ascii(9)+' ',ascii(9));
if  s <> [' '; ' '] then pause,end
//===============================
s = tokens(' '+ascii(9)+' ',' ');
if  s <> ascii(9) then pause,end
//===============================
