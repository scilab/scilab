clear;

errcatch(4,'continue','nomessage')
a
if iserror(4)==1 then errclear(-1),else pause,end
a;b=1;
errclear(-1),if b<>1 then pause,end
b=[];for k=1:3,b(1,k)=k;a,end
errclear(-1),if or(b<>(1:3)) then pause,end

deff('foo()','x=a','n')
foo();if iserror(4)==1 then errclear(-1),else pause,end
deff('foo()','x=a')
foo();if iserror(4)==1 then errclear(-1),else pause,end

deff('b=foo()','x=a;b=1','n')
b=foo();errclear(-1),if b<>1 then pause,end


deff('b=foo()','x=a;b=1');b=foo();errclear(-1)


deff('b=foo()',['x=a';'b=1'])
b=foo();errclear(-1),if b<>1 then pause,end

deff('b=foo()','b=[];for k=1:3,b(1,k)=k;a,end','n')
b=foo();errclear(-1),if  or(b<>(1:3)) then pause,end

deff('b=foo()',['b=[]';'for k=1:3';'b(1,k)=k;a';'end'])
b=foo();errclear(-1),if  or(b<>(1:3)) then pause,end

errcatch(47,'continue','nomessage')
deff('b=foo()','if %t then b=1')
if iserror(47)<>1 then pause,else errclear(-1);end

errcatch(37,'continue','nomessage')
deff('b=foo(','if %t then b=1')
if iserror(37)<>1 then pause,else errclear(-1);end


clear
errcatch(-1)
if execstr('a','errcatch')<>4 then pause,end
ierr=execstr('a;b=1','errcatch');if ierr<>4 then pause,end
ierr=execstr(['b=[]';'for k=1:3';'b(1,k)=k';'end';'a'],'errcatch');
if ierr<>4|or(b<>(1:3)) then pause,end

clear;errcatch(4,'continue','nomessage')
execstr('a;if iserror(4)==1 then errclear(-1),else pause,end')
clear;errcatch(-1)

deff('b=foo()',['b=[]';
                'errcatch(4,''continue'',''nomessage'')'
                'for k=1:3'
                '  b(1,k)=k'
                '  a'
                '  if iserror(4)==1 then errclear(-1),end'
                'end'],'n')
b=foo();if or(b<>(1:3)) then pause,end

deff('b=foo()',['b=[]';
                'errcatch(4,''continue'',''nomessage'')'
                'for k=1:3'
                '  b(1,k)=k'
                '  a'
                '  if iserror(4)==1 then errclear(-1),end'
                'end'])
b=foo();if or(b<>(1:3)) then pause,end
