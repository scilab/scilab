// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

// <-- Non-regression test for bug 3512 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=3512
//
// Short description:
// regexp may give erroneous results
//==============================================================
ref_start_pos = [1 5 9 13 17];
ref_end_pos = [2 6 10 14 18];
ref_match_str = [ 'aa';'aa';'aa';'aa';'aa'];

t = 'aaa aab aac aad aae';
pattern = '/aa/';
[start_pos, end_pos, match_str] = regexp(t,pattern);

if length(t)<> 19 then pause,end
if or(ref_start_pos <> start_pos) then pause,end
if or(ref_end_pos <> end_pos) then pause,end
if or(ref_match_str <> match_str) then pause,end
//==============================================================
ref_start_pos = [3 6 10];
ref_end_pos = [4 7 11];
ref_match_str = [ 'aa';'aa';'aa'];

t='a aa aab aaa';
pattern = '/aa/';
[start_pos, end_pos, match_str] = regexp(t,pattern);

if length(t)<> 12 then pause,end
if or(ref_start_pos <> start_pos) then pause,end
if or(ref_end_pos <> end_pos) then pause,end
if or(ref_match_str <> match_str) then pause,end
//==============================================================
[start_pos, end_pos, match_str] = regexp(' aab','/aa/');
if or(start_pos<> 2) then pause,end
if or(end_pos<> 3) then pause,end
if match_str<>'aa' then pause,end
//==============================================================
[start_pos, end_pos, match_str] = regexp('kAab','/Aa/');
if or(start_pos<> 2) then pause,end
if or(end_pos<> 3) then pause,end
if match_str<>'Aa' then pause,end
//==============================================================
ref_start_pos = 17;
ref_end_pos = 19;
ref_match_str = 'acK';

t='                acK                  baK                     cbK';
pattern = '/\bacK/';
[start_pos, end_pos, match_str] = regexp(t,pattern);

if ref_start_pos <> start_pos then pause,end
if ref_end_pos <> end_pos then pause,end
if ref_match_str <> match_str then pause,end
//==============================================================
ref_start_pos = [17 38];
ref_end_pos = [19 40];
ref_match_str = [ 'acK';'acK'];

t='                acK                  acK                     cbK';
pattern = '/\bacK/';
[start_pos, end_pos, match_str] = regexp(t,pattern);

if or(ref_start_pos <> start_pos) then pause,end
if or(ref_end_pos <> end_pos) then pause,end
if or(ref_match_str <> match_str) then pause,end
//==============================================================
ref_start_pos = 38;
ref_end_pos = 40;
ref_match_str = 'baK';

t='                acK                  baK                     cbK';
pattern = '/\bbaK/';
[start_pos, end_pos, match_str] = regexp(t,pattern);

if ref_start_pos <> start_pos then pause,end
if ref_end_pos <> end_pos then pause,end
if ref_match_str <> match_str then pause,end
//==============================================================
ref_start_pos = 62;
ref_end_pos = 64;
ref_match_str = 'c K';

t='                acK                  baK                     c K';
pattern = '/\bc K/';
[start_pos, end_pos, match_str] = regexp(t,pattern);

if ref_start_pos <> start_pos then pause,end
if ref_end_pos <> end_pos then pause,end
if ref_match_str <> match_str then pause,end
//==============================================================
