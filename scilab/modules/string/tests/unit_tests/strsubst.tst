//===============================
// unit tests strsubst
// INRIA 2007
//===============================
if strsubst('abc,abd,aa,bxe',',',';')<>'abc;abd;aa;bxe'  then pause,end
//===============================
if strsubst('abc,abd,aa,bxe',',',emptystr())<>'abcabdaabxe'  then pause,end
//===============================
if strsubst(',abc,abd,aa,bxe',',',emptystr())<>'abcabdaabxe'  then pause,end
//===============================
if strsubst('abc',',',';')<>'abc'  then pause,end
//===============================
if strsubst('SCI/demos/scicos','SCI','.') <> './demos/scicos'  then pause,end
//===============================
if strsubst('SCI/demos/scicos','/',' ') <> 'SCI demos scicos'  then pause,end
//===============================
if strsubst([],'/',' ') <> [] then pause,end
//===============================
if strsubst([],' ','') <> [] then pause,end
//===============================
if strsubst('',' ','') <> '' then pause,end
//===============================
if strsubst("2","3","1") <> "2" then pause,end
//===============================
if strsubst(["2","3"],"3","1") <> ['2' '1'] then pause,end
//===============================
r = strsubst(['Scilab','Mathlab';'Octlab','Rlab'],'lab','');
ref  = ['Sci','Math';'Oct','R'];
if (r <> ref) then  pause,end
//===============================

if strsubst('abc','/abc/',' ','r') <>  ' '   then pause,end
if strsubst('xbc','/abc/',' ','r') <>  'xbc'   then pause,end
if strsubst('axc','/abc/',' ','r') <>  'axc'   then pause,end
if strsubst('xaccy','/abc/',' ','r') <>  'xaccy'   then pause,end
if strsubst('abc','/ab*c/',' ','r') <>  ' '   then pause,end
if strsubst('abc','/ab*bc/',' ','r') <>  ' '   then pause,end
if strsubst('abbc','/ab*bc/',' ','r') <>  ' '   then pause,end
if strsubst('abbbbc','/ab*bc/',' ','r') <>  ' '   then pause,end
if strsubst('abbbbc','/.{1}/',' ','r') <>  ' bbbbc'   then pause,end
if strsubst('abbbbc','/.{3,4}/',' ','r') <>  ' bc'   then pause,end
if strsubst('abbbbc','/ab{0,}bc/','^','r') <>  '^'   then pause,end
if strsubst('abbc','/ab+bc/','^','r') <>  '^'   then pause,end
if strsubst('abc','/ab+bc/','^','r') <>  'abc'   then pause,end
if strsubst('abq','/ab+bc/','^','r') <>  'abq'   then pause,end
if strsubst('abbc','/ab{1,}bc/','^','r') <>  '^'   then pause,end
if strsubst('abbbbc','/ab+bc/','^','r') <>  '^'   then pause,end
if strsubst('abbbbc','/ab{1,}bc/','^','r') <>  '^'   then pause,end
if strsubst('abbc','/ab{1,3}bc/','^','r') <>  '^'   then pause,end
if strsubst('abbbbc','/ab{3,4}bc/','^','r') <>  '^'   then pause,end
if strsubst('abbbbc','/ab{4,5}bc/','^','r') <>  'abbbbc'   then pause,end
if strsubst('abc','/abc/',' ','r') <>  ' '   then pause,end
if strsubst('xbc','/abc/',' ','r') <>  'xbc'   then pause,end
if strsubst('axc','/abc/',' ','r') <>  'axc'   then pause,end
if strsubst('xaccy','/abc/',' ','r') <>  'xaccy'   then pause,end
if strsubst('abc','/ab*c/',' ','r') <>  ' '   then pause,end
if strsubst('abc','/ab*bc/',' ','r') <>  ' '   then pause,end
if strsubst('abbc','/ab*bc/',' ','r') <>  ' '   then pause,end
if strsubst('abbbbc','/ab*bc/',' ','r') <>  ' '   then pause,end
if strsubst('abbbbc','/.{1}/',' ','r') <>  ' bbbbc'   then pause,end
if strsubst('abbbbc','/.{3,4}/',' ','r') <>  ' bc'   then pause,end
if strsubst('abbbbc','/ab{0,}bc/','^','r') <>  '^'   then pause,end
if strsubst('abbc','/ab+bc/','^','r') <>  '^'   then pause,end
if strsubst('abc','/ab+bc/','^','r') <>  'abc'   then pause,end
if strsubst('abq','/ab+bc/','^','r') <>  'abq'   then pause,end
if strsubst('abbc','/ab{1,}bc/','^','r') <>  '^'   then pause,end
if strsubst('abbbbc','/ab+bc/','^','r') <>  '^'   then pause,end
if strsubst('abbbbc','/ab{1,}bc/','^','r') <>  '^'   then pause,end
if strsubst('abbc','/ab{1,3}bc/','^','r') <>  '^'   then pause,end
if strsubst('abbbbc','/ab{3,4}bc/','^','r') <>  '^'   then pause,end
if strsubst('abbbbc','/ab{4,5}bc/','^','r') <>  'abbbbc'   then pause,end
if strsubst('abc','/abc/',' ','r') <>  ' '   then pause,end
if strsubst('xbc','/abc/',' ','r') <>  'xbc'   then pause,end
if strsubst('axc','/abc/',' ','r') <>  'axc'   then pause,end
if strsubst('xaccy','/abc/',' ','r') <>  'xaccy'   then pause,end
if strsubst('abc','/ab*c/',' ','r') <>  ' '   then pause,end
if strsubst('abc','/ab*bc/',' ','r') <>  ' '   then pause,end
if strsubst('abbc','/ab*bc/',' ','r') <>  ' '   then pause,end
if strsubst('abbbbc','/ab*bc/',' ','r') <>  ' '   then pause,end
if strsubst('abbbbc','/.{1}/',' ','r') <>  ' bbbbc'   then pause,end
if strsubst('abbbbc','/.{3,4}/',' ','r') <>  ' bc'   then pause,end
if strsubst('abbbbc','/ab{0,}bc/','^','r') <>  '^'   then pause,end
if strsubst('abbc','/ab+bc/','^','r') <>  '^'   then pause,end
if strsubst('abc','/ab+bc/','^','r') <>  'abc'   then pause,end
if strsubst('abq','/ab+bc/','^','r') <>  'abq'   then pause,end
if strsubst('abbc','/ab{1,}bc/','^','r') <>  '^'   then pause,end
if strsubst('abbbbc','/ab+bc/','^','r') <>  '^'   then pause,end
if strsubst('abbbbc','/ab{1,}bc/','^','r') <>  '^'   then pause,end
if strsubst('abbc','/ab{1,3}bc/','^','r') <>  '^'   then pause,end
if strsubst('abbbbc','/ab{3,4}bc/','^','r') <>  '^'   then pause,end
if strsubst('abbbbc','/ab{4,5}bc/','^','r') <>  'abbbbc'   then pause,end




