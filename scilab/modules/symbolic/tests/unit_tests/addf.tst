// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2007-2008 - INRIA
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================


//formal
if addf('1','1')<>'2' then pause,end
if addf('1','0')<>'1' then pause,end
if addf('0','1')<>'1' then pause,end
if addf('0','0')<>'0' then pause,end
if addf('1','-1')<>'0' then pause,end
if addf('-1','1')<>'0' then pause,end
if addf('-1','0')<>'-1' then pause,end
if addf('0','-1')<>'-1' then pause,end

if addf('1','a')<>'a+1' then pause,end
if addf('a','1')<>'a+1' then pause,end
if addf('a','0')<>'a' then pause,end
if addf('0','a')<>'a' then pause,end
if addf('a','-1')<>'a-1' then pause,end
if addf('-1','a')<>'a-1' then pause,end
if addf('a','b')<>'a+b' then pause,end
if addf('a+b','c')<>'a+b+c' then pause,end
if addf('c','a+b')<>'c+a+b' then pause,end
if addf('a+b','a+b')<>'a+b+a+b' then pause,end
if addf('a+b','a-b')<>'a+a' then pause,end
if addf('2*a+b','a-b')<>'2*a+a' then pause,end
