// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- JVM NOT MANDATORY -->


if mulf('1','1')<>'1' then pause,end
if mulf('1','0')<>'0' then pause,end
if mulf('0','1')<>'0' then pause,end
if mulf('0','0')<>'0' then pause,end
if mulf('1','-1')<>'-1' then pause,end
if mulf('-1','1')<>'-1' then pause,end
if mulf('-1','0')<>'0' then pause,end
if mulf('0','-1')<>'0' then pause,end

if mulf('1','a')<>'a' then pause,end
if mulf('a','1')<>'a' then pause,end
if mulf('a','0')<>'0' then pause,end
if mulf('0','a')<>'0' then pause,end
if mulf('a','-1')<>'-a' then pause,end
if mulf('-1','a')<>'-a' then pause,end
if mulf('a','b')<>'a*b' then pause,end
if mulf('a+b','c')<>'(a+b)*c' then pause,end
if mulf('c','a+b')<>'c*(a+b)' then pause,end
if mulf('a+b','a+b')<>'(a+b)*(a+b)' then pause,end
if mulf('2*a+b','a-b')<>'(2*a+b)*(a-b)' then pause,end
