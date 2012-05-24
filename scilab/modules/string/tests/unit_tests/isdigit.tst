// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2007-2008 - INRIA
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

//===============================
// unit tests isdigit
//===============================
TXT = 'A1,B2,C3';
//===============================
if isdigit(TXT) <> [%F %T %F %F %T %F %F %T] then pause,end
//===============================
if isdigit('') <> [] then pause,end
//===============================

if isdigit('a') then pause,end
if isdigit('b') then pause,end
if isdigit('c') then pause,end
if isdigit('d') then pause,end
if isdigit('e') then pause,end
if isdigit('f') then pause,end
if isdigit('g') then pause,end
if isdigit('h') then pause,end
if isdigit('i') then pause,end
if isdigit('j') then pause,end
if isdigit('k') then pause,end
if isdigit('l') then pause,end
if isdigit('m') then pause,end
if isdigit('n') then pause,end
if isdigit('o') then pause,end
if isdigit('p') then pause,end
if isdigit('q') then pause,end
if isdigit('r') then pause,end
if isdigit('s') then pause,end
if isdigit('t') then pause,end
if isdigit('u') then pause,end
if isdigit('v') then pause,end
if isdigit('x') then pause,end
if isdigit('y') then pause,end
if isdigit('z') then pause,end
if isdigit('A') then pause,end
if isdigit('B') then pause,end
if isdigit('C') then pause,end
if isdigit('D') then pause,end
if isdigit('E') then pause,end
if isdigit('F') then pause,end
if isdigit('G') then pause,end
if isdigit('H') then pause,end
if isdigit('I') then pause,end
if isdigit('J') then pause,end
if isdigit('K') then pause,end
if isdigit('L') then pause,end
if isdigit('M') then pause,end
if isdigit('O') then pause,end
if isdigit('P') then pause,end
if isdigit('Q') then pause,end
if isdigit('R') then pause,end
if isdigit('S') then pause,end
if isdigit('T') then pause,end
if isdigit('U') then pause,end
if isdigit('V') then pause,end
if isdigit('X') then pause,end
if isdigit('Y') then pause,end
if isdigit('Z') then pause,end

if isdigit('!') then pause,end
if isdigit('#') then pause,end
if isdigit('$') then pause,end
if isdigit('%') then pause,end
if isdigit('&') then pause,end
if isdigit('(') then pause,end
if isdigit(')') then pause,end
if isdigit('*') then pause,end
if isdigit('+') then pause,end
if isdigit(',') then pause,end
if isdigit('-') then pause,end
if isdigit('.') then pause,end
if isdigit('/') then pause,end
if isdigit(':') then pause,end
if isdigit(';') then pause,end
if isdigit('<') then pause,end
if isdigit('=') then pause,end
if isdigit('>') then pause,end
if isdigit('@') then pause,end
if isdigit('[') then pause,end
if isdigit('\') then pause,end
if isdigit(']') then pause,end
if isdigit('^') then pause,end
if isdigit('_') then pause,end
if isdigit('`') then pause,end
if isdigit('{') then pause,end
if isdigit('|') then pause,end
if isdigit('}') then pause,end
if isdigit('~') then pause,end

if isdigit('""') then pause,end
if isdigit('''') then pause,end

if ~isdigit('1') then pause,end
if ~isdigit('2') then pause,end
if ~isdigit('3') then pause,end
if ~isdigit('4') then pause,end
if ~isdigit('5') then pause,end
if ~isdigit('6') then pause,end
if ~isdigit('7') then pause,end
if ~isdigit('8') then pause,end
if ~isdigit('9') then pause,end
