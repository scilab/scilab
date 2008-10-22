// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2007-2008 - INRIA
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

//===============================
// unit tests string
//===============================
if string(1)<>'1' then pause,end
//===============================
if string(1.5)<>'1.5' then pause,end
//===============================
if string([])<>[] then pause,end
//===============================
if string('')<>'' then pause,end
//===============================
if or(string(1:3)<>['1','2','3']) then pause,end
//===============================
if or(string([1;2;3])<>['1';'2';'3']) then pause,end
//===============================
if string('foo')<>'foo' then pause,end
//===============================
deff('y=mymacro(x)','y=x+1');
[out,in,text]=string(mymacro);
if out<>'y'|in<>'x'|text<>[] then pause,end
//===============================
mymacro=null();deff('y=mymacro(x)','y=x+1','n');
[out,in,text]=string(mymacro);
if out<>'y'|in<>'x'|text<>'y=x+1' then pause,end
//===============================
R = string(corelib);
if strsubst(R(1),'\','/') <> 'SCI/modules/core/macros/' then pause,end
//===============================

A = floor(abs(2^8  * rand(10,10)));
B = floor(abs(2^16 * rand(10,10)));
C = floor(abs(2^32 * rand(10,10)));

D = floor(abs(2^7  * rand(10,10)));
E = floor(abs(2^15 * rand(10,10)));
F = floor(abs(2^31 * rand(10,10)));

if or(string(A) <> string(uint8(A)))  then pause, end
if or(string(B) <> string(uint16(B))) then pause, end
if or(string(C) <> string(uint32(C))) then pause, end

if or(string(D) <> string(int8(D)))   then pause, end
if or(string(E) <> string(int16(E)))  then pause, end
if or(string(F) <> string(int32(F)))  then pause, end
