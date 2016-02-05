//<-- CLI SHELL MODE -->
// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <!-- ENGLISH IMPOSED -->

funcprot(0);

clear //sdfdgd
clear//sdfdgd
a=1;
clear a//sdfdgd
deff('y=foo(a,b,c)','y=a+b+c')
foo aaa bbb   cc
if ans<>'aaabbbcc' then pause,end
foo aaa bbb   cc;
if ans<>'aaabbbcc' then pause,end
foo 'aaa' bbb 'cc'
if ans<>'aaabbbcc' then pause,end
foo aaa 'bbb' cc
if ans<>'aaabbbcc' then pause,end
foo aaa bbb cc //qdsddfd
if ans<>'aaabbbcc' then pause,end
foo aaa bbb cc//qdsddfd
if ans<>'aaabbbcc' then pause,end
deff('y=foo(a,b,c)','y=a+b+c')
foo aaa bbb   cc
if ans<>'aaabbbcc' then pause,end
foo 'aaa' bbb 'cc'
if ans<>'aaabbbcc' then pause,end
foo aaa 'bbb' cc
if ans<>'aaabbbcc' then pause,end
foo aaa bbb cc //qdsddfd
if ans<>'aaabbbcc' then pause,end
foo aaa bbb cc//qdsddfd
if ans<>'aaabbbcc' then pause,end

ascii foo;
if ans<>[102,111,111] then pause,end
