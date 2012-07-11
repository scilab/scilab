// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
// <-- CLI SHELL MODE -->
// =============================================================================
a = [223;111;2;4;2;2];
b = [2;3;21;223;123;22];
// =============================================================================
REF_V = [4; 111];
REF_K = [4; 2];
[v,k]= setdiff(a,b);
if and(v == REF_V) <> %t then pause,end
if and(k == REF_K) <> %t then pause,end
// =============================================================================
REF_V = ['111' ; '4'];
REF_K = [2; 4];
[v,k] = setdiff(string(a),string(b));
if and(v == REF_V) <> %t then pause,end
if and(k == REF_K) <> %t then pause,end
// =============================================================================