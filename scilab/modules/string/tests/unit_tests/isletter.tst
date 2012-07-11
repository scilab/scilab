// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2007-2008 - INRIA
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

//===============================
// unit tests isletter
//===============================
TXT = 'A1,B2,C3';
//===============================
if isletter(TXT) <> [%T %F %F %T %F %F %T %F] then pause,end
//===============================
if isletter('') <> [] then pause,end
//===============================

if ~isletter('a') then pause,end
if ~isletter('b') then pause,end
if ~isletter('c') then pause,end
if ~isletter('d') then pause,end
if ~isletter('e') then pause,end
if ~isletter('f') then pause,end
if ~isletter('g') then pause,end
if ~isletter('h') then pause,end
if ~isletter('i') then pause,end
if ~isletter('j') then pause,end
if ~isletter('k') then pause,end
if ~isletter('l') then pause,end
if ~isletter('m') then pause,end
if ~isletter('n') then pause,end
if ~isletter('o') then pause,end
if ~isletter('p') then pause,end
if ~isletter('q') then pause,end
if ~isletter('r') then pause,end
if ~isletter('s') then pause,end
if ~isletter('t') then pause,end
if ~isletter('u') then pause,end
if ~isletter('v') then pause,end
if ~isletter('x') then pause,end
if ~isletter('y') then pause,end
if ~isletter('z') then pause,end
if ~isletter('A') then pause,end
if ~isletter('B') then pause,end
if ~isletter('C') then pause,end
if ~isletter('D') then pause,end
if ~isletter('E') then pause,end
if ~isletter('F') then pause,end
if ~isletter('G') then pause,end
if ~isletter('H') then pause,end
if ~isletter('I') then pause,end
if ~isletter('J') then pause,end
if ~isletter('K') then pause,end
if ~isletter('L') then pause,end
if ~isletter('M') then pause,end
if ~isletter('O') then pause,end
if ~isletter('P') then pause,end
if ~isletter('Q') then pause,end
if ~isletter('R') then pause,end
if ~isletter('S') then pause,end
if ~isletter('T') then pause,end
if ~isletter('U') then pause,end
if ~isletter('V') then pause,end
if ~isletter('X') then pause,end
if ~isletter('Y') then pause,end
if ~isletter('Z') then pause,end

if isletter('!') then pause,end
if isletter('#') then pause,end
if isletter('$') then pause,end
if isletter('%') then pause,end
if isletter('&') then pause,end
if isletter('(') then pause,end
if isletter(')') then pause,end
if isletter('*') then pause,end
if isletter('+') then pause,end
if isletter(',') then pause,end
if isletter('-') then pause,end
if isletter('.') then pause,end
if isletter('/') then pause,end
if isletter(':') then pause,end
if isletter(';') then pause,end
if isletter('<') then pause,end
if isletter('=') then pause,end
if isletter('>') then pause,end
if isletter('@') then pause,end
if isletter('[') then pause,end
if isletter('\') then pause,end
if isletter(']') then pause,end
if isletter('^') then pause,end
if isletter('_') then pause,end
if isletter('`') then pause,end
if isletter('{') then pause,end
if isletter('|') then pause,end
if isletter('}') then pause,end
if isletter('~') then pause,end

if isletter('0') then pause,end
if isletter('1') then pause,end
if isletter('2') then pause,end
if isletter('3') then pause,end
if isletter('4') then pause,end
if isletter('5') then pause,end
if isletter('6') then pause,end
if isletter('7') then pause,end
if isletter('8') then pause,end
if isletter('9') then pause,end

if isletter('''') then pause,end
if isletter('""') then pause,end
