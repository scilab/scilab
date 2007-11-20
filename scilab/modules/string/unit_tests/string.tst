//===============================
// unit tests string
// INRIA 2007
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
