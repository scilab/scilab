// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2007-2008 - INRIA
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

//===============================
// unit tests isalphanum
//===============================
TXT = 'A1,B2,C3';
//===============================
if isalphanum(TXT) <> [ %T %T %F %T %T %F %T %T] then pause,end
//===============================
if isalphanum('') <> [] then pause,end
//===============================

if ~isalphanum('a') then pause,end
if ~isalphanum('b') then pause,end
if ~isalphanum('c') then pause,end
if ~isalphanum('d') then pause,end
if ~isalphanum('e') then pause,end
if ~isalphanum('f') then pause,end
if ~isalphanum('g') then pause,end
if ~isalphanum('h') then pause,end
if ~isalphanum('i') then pause,end
if ~isalphanum('j') then pause,end
if ~isalphanum('k') then pause,end
if ~isalphanum('l') then pause,end
if ~isalphanum('m') then pause,end
if ~isalphanum('n') then pause,end
if ~isalphanum('o') then pause,end
if ~isalphanum('p') then pause,end
if ~isalphanum('q') then pause,end
if ~isalphanum('r') then pause,end
if ~isalphanum('s') then pause,end
if ~isalphanum('t') then pause,end
if ~isalphanum('u') then pause,end
if ~isalphanum('v') then pause,end
if ~isalphanum('x') then pause,end
if ~isalphanum('y') then pause,end
if ~isalphanum('z') then pause,end
if ~isalphanum('A') then pause,end
if ~isalphanum('B') then pause,end
if ~isalphanum('C') then pause,end
if ~isalphanum('D') then pause,end
if ~isalphanum('E') then pause,end
if ~isalphanum('F') then pause,end
if ~isalphanum('G') then pause,end
if ~isalphanum('H') then pause,end
if ~isalphanum('I') then pause,end
if ~isalphanum('J') then pause,end
if ~isalphanum('K') then pause,end
if ~isalphanum('L') then pause,end
if ~isalphanum('M') then pause,end
if ~isalphanum('O') then pause,end
if ~isalphanum('P') then pause,end
if ~isalphanum('Q') then pause,end
if ~isalphanum('R') then pause,end
if ~isalphanum('S') then pause,end
if ~isalphanum('T') then pause,end
if ~isalphanum('U') then pause,end
if ~isalphanum('V') then pause,end
if ~isalphanum('X') then pause,end
if ~isalphanum('Y') then pause,end
if ~isalphanum('Z') then pause,end

if ~isalphanum('1') then pause,end
if ~isalphanum('2') then pause,end
if ~isalphanum('3') then pause,end
if ~isalphanum('4') then pause,end
if ~isalphanum('5') then pause,end
if ~isalphanum('6') then pause,end
if ~isalphanum('7') then pause,end
if ~isalphanum('8') then pause,end
if ~isalphanum('9') then pause,end

if isalphanum('!') then pause,end
if isalphanum('#') then pause,end
if isalphanum('$') then pause,end
if isalphanum('%') then pause,end
if isalphanum('&') then pause,end
if isalphanum('(') then pause,end
if isalphanum(')') then pause,end
if isalphanum('*') then pause,end
if isalphanum('+') then pause,end
if isalphanum(',') then pause,end
if isalphanum('-') then pause,end
if isalphanum('.') then pause,end
if isalphanum('/') then pause,end
if isalphanum(':') then pause,end
if isalphanum(';') then pause,end
if isalphanum('<') then pause,end
if isalphanum('=') then pause,end
if isalphanum('>') then pause,end
if isalphanum('@') then pause,end
if isalphanum('[') then pause,end
if isalphanum('\') then pause,end
if isalphanum(']') then pause,end
if isalphanum('^') then pause,end
if isalphanum('_') then pause,end
if isalphanum('`') then pause,end
if isalphanum('{') then pause,end
if isalphanum('|') then pause,end
if isalphanum('}') then pause,end
if isalphanum('~') then pause,end
