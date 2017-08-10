//======================================================================================== 
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2007-2008 - INRIA
//
//  This file is distributed under the same license as the Scilab package.
//======================================================================================== 

// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->

//======================================================================================== 
// unit tests strsubst
//======================================================================================== 
assert_checkequal( strsubst('abc,abd,aa,bxe',',',';'),'abc;abd;aa;bxe'  );
//======================================================================================== 
assert_checkequal( strsubst('abc,abd,aa,bxe',',',emptystr()),'abcabdaabxe'  );
//======================================================================================== 
assert_checkequal( strsubst(',abc,abd,aa,bxe',',',emptystr()),'abcabdaabxe'  );
//======================================================================================== 
assert_checkequal( strsubst('abc',',',';'),'abc'  );
//======================================================================================== 
assert_checkequal( strsubst('SCI/demos/scicos','SCI','.') , './demos/scicos'  );
//======================================================================================== 
assert_checkequal( strsubst('SCI/demos/scicos','/',' ') , 'SCI demos scicos'  );
//======================================================================================== 
assert_checkequal( strsubst([],'/',' ') , [] );
//======================================================================================== 
assert_checkequal( strsubst([],' ','') , [] );
//======================================================================================== 
assert_checkequal( strsubst('',' ','') , '' );
//======================================================================================== 
assert_checkequal( strsubst("2","3","1") , "2" );
//======================================================================================== 
assert_checkequal( strsubst(["2","3"],"3","1") , ['2' '1'] );
//======================================================================================== 
r = strsubst(['Scilab','Mathlab';'Octlab','Rlab'],'lab','');
ref  = ['Sci','Math';'Oct','R'];
assert_checkequal( r , ref);
//======================================================================================== 
assert_checkequal( strsubst('xbc','/abc/',' ','r') ,  'xbc'   );
assert_checkequal( strsubst('axc','/abc/',' ','r') ,  'axc'   );
assert_checkequal( strsubst('xaccy','/abc/',' ','r') ,  'xaccy'   );
assert_checkequal( strsubst('abc','/ab*c/',' ','r') ,  ' '   );
assert_checkequal( strsubst('abc','/ab*bc/',' ','r') ,  ' '   );
assert_checkequal( strsubst('abbc','/ab*bc/',' ','r') ,  ' '   );
assert_checkequal( strsubst('abbbbc','/ab*bc/',' ','r') ,  ' '   );
assert_checkequal( strsubst('abbbbc','/.{1}/',' ','r') ,  '      '   );
assert_checkequal( strsubst('abbbbc','/.{3,4}/',' ','r') ,  ' bc'   );
assert_checkequal( strsubst('abbbbc','/ab{0,}bc/','^','r') ,  '^'   );
assert_checkequal( strsubst('abbc','/ab+bc/','^','r') ,  '^'   );
assert_checkequal( strsubst('abc','/ab+bc/','^','r') ,  'abc'   );
assert_checkequal( strsubst('abq','/ab+bc/','^','r') ,  'abq'   );
assert_checkequal( strsubst('abbc','/ab{1,}bc/','^','r') ,  '^'   );
assert_checkequal( strsubst('abbbbc','/ab+bc/','^','r') ,  '^'   );
assert_checkequal( strsubst('abbbbc','/ab{1,}bc/','^','r') ,  '^'   );
assert_checkequal( strsubst('abbc','/ab{1,3}bc/','^','r') ,  '^'   );
assert_checkequal( strsubst('abbbbc','/ab{3,4}bc/','^','r') ,  '^'   );
assert_checkequal( strsubst('abbbbc','/ab{4,5}bc/','^','r') ,  'abbbbc'   );
assert_checkequal( strsubst('abbc','/ab?bc/',' ','r') ,  ' '   );
assert_checkequal( strsubst('abc','/ab?bc/',' ','r') ,  ' '   );
assert_checkequal( strsubst('abc','/ab{0,1}bc/',' ','r') ,  ' '   );
assert_checkequal( strsubst('xaccy','/ab?bc/',' ','r') ,  'xaccy'   );
assert_checkequal( strsubst('abc','/ab?c/',' ','r') ,  ' '   );
assert_checkequal( strsubst('abc','/ab{0,1}c/',' ','r') ,  ' '   );
assert_checkequal( strsubst('abc','/^abc$/',' ','r') ,  ' '   );
assert_checkequal( strsubst('abcc','/^abc$/',' ','r') ,  'abcc'   );
assert_checkequal( strsubst('aabc','/^abc&/',' ','r') ,  'aabc'   );
assert_checkequal( strsubst('aabc','/abc$/','^','r') ,  'a^'   );
assert_checkequal( strsubst('xy','/\By\b/','^','r') ,  'x^'   );
assert_checkequal( strsubst('yz','/\by\B/','^','r') ,  '^z'   );
assert_checkequal( strsubst('e','/a|b|c|d|e/',' ','r') ,  ' '   );
assert_checkequal( strsubst('abcdefg','/abcd*efg/',' ','r') ,  ' '   );
assert_checkequal( strsubst('xabyabbbz','/ab*/',' ','r') ,  'x y z'   );
assert_checkequal( strsubst('hij','/[abhgefdc]ij/',' ','r') ,  ' '   );
assert_checkequal( strsubst('abcde','/^(ab|cd)e/',' ','r') ,  'abcde'   );
assert_checkequal( strsubst('effg','/(bc+d$|ef*g.|h?i(j|k))/','^','r') ,  'effg'   );
assert_checkequal( strsubst('aa','/((((((((((a))))))))))\10/','^','r') ,  '^'   );
assert_checkequal( strsubst('a!','/((((((((((a))))))))))\041/','^','r') ,  '^'   );
assert_checkequal( strsubst('a','/(((((((((a)))))))))/','^','r') ,  '^'   );
assert_checkequal( strsubst('multiple words, yeah','/multiple words/','^','r') ,  '^, yeah'   );
assert_checkequal( strsubst('ac','/a[-]?c/','^','r') ,  '^'   );
assert_checkequal( strsubst('a','/(a)|\1/','^','r') ,  '^'   );
assert_checkequal( strsubst('A-','/a[-b]/i','^','r') ,  '^'   );
assert_checkequal( strsubst('A-','/a[b-]/i','^','r') ,  '^'   );
assert_checkequal( strsubst('A]','/a]/i','^','r') ,  '^'   );
assert_checkequal( strsubst('abc','/ab?bc/',' ','r') ,  ' '   );
assert_checkequal( strsubst('abc','/ab{0,1}bc/',' ','r') ,  ' '   );
assert_checkequal( strsubst('xaccy','/ab?bc/',' ','r') ,  'xaccy'   );
assert_checkequal( strsubst('abc','/ab?c/',' ','r') ,  ' '   );
assert_checkequal( strsubst('abc','/ab{0,1}c/',' ','r') ,  ' '   );
assert_checkequal( strsubst('abc','/^abc$/',' ','r') ,  ' '   );
assert_checkequal( strsubst('abcc','/^abc$/',' ','r') ,  'abcc'   );
assert_checkequal( strsubst('aabc','/^abc&/',' ','r') ,  'aabc'   );
assert_checkequal( strsubst('xy','/\By\b/','^','r') ,  'x^'   );
assert_checkequal( strsubst('xy','/\By\b/','^','r') ,  'x^'   );
assert_checkequal( strsubst('abc','/abc/',' ','r') ,  ' '   );
//======================================================================================== 
assert_checkequal( strsubst('......abef','/.*a(?!(b|cd)*e).*f/'                 ,' ' ,'r') ,  '......abef'  );
assert_checkequal( strsubst('fools','/(foo|fool|x.|money|parted)$/'        ,' ' ,'r') ,  'fools'  );
assert_checkequal( strsubst('fools','/(x.|foo|fool|x.|money|parted|y.)$/'  ,' ' ,'r') ,  'fools'  );
assert_checkequal( strsubst('fools','/(foo|fool|money|parted)$/'           ,' ' ,'r') ,  'fools'  );
assert_checkequal( strsubst('Oo','/^(o)(?!.*\1)/i'                      ,' ' ,'r') ,  'Oo'  );
assert_checkequal( strsubst('AbCd','/ab(?i)cd/'                           ,' ' ,'r') ,  'AbCd'  );
assert_checkequal( strsubst('a\nb\n','/a\Z/'                                ,' ' ,'r') , 'a\nb\n'  );
assert_checkequal( strsubst('a\nb\n','/a\z/'                                ,' ' ,'r') , 'a\nb\n'  );
assert_checkequal( strsubst('a\nb\n','/a$/'                                 ,' ' ,'r') ,  'a\nb\n' );
assert_checkequal( strsubst('b\na\n','/a\z/'                                ,' ' ,'r') ,  'b\na\n'  );
assert_checkequal( strsubst('a\nb\n','/a\Z/m'                               ,' ' ,'r') ,  'a\nb\n'  );
assert_checkequal( strsubst('a\nb\n','/a\z/m'                               ,' ' ,'r') , 'a\nb\n'  );
assert_checkequal( strsubst('b\na\n','/a\z/m'                               ,' ' ,'r') ,  'b\na\n'  );
assert_checkequal( strsubst('aa\nb\n','/aa\Z/'                               ,' ' ,'r') ,  'aa\nb\n' );
assert_checkequal( strsubst('aa\nb\n','/aa\z/'                               ,' ' ,'r') ,  'aa\nb\n'  );
assert_checkequal( strsubst('aa\nb\n','/aa$/'                                ,' ' ,'r') ,  'aa\nb\n'  );
assert_checkequal( strsubst('b\naa\n','/aa\z/'                               ,' ' ,'r') ,  'b\naa\n'  );
assert_checkequal( strsubst('aa\nb\n','/aa\Z/m'                              ,' ' ,'r') ,  'aa\nb\n'  );
assert_checkequal( strsubst('aa\nb\n','/aa\z/m'                              ,' ' ,'r') ,  'aa\nb\n'  );
assert_checkequal( strsubst('b\naa\n','/aa\z/m'                              ,' ' ,'r') ,  'b\naa\n'  );
assert_checkequal( strsubst('ac\nb\n','/aa\Z/'                               ,' ' ,'r') ,  'ac\nb\n'  );
assert_checkequal( strsubst('ac\nb\n','/aa\z/'                               ,' ' ,'r') ,  'ac\nb\n'  );
assert_checkequal( strsubst('ac\nb\n','/aa$/'                                ,' ' ,'r') ,  'ac\nb\n'  );
assert_checkequal( strsubst('b\nac\n','/aa\Z/'                               ,' ' ,'r') ,  'b\nac\n'  );
assert_checkequal( strsubst('b\nac\n','/aa\z/'                               ,' ' ,'r') ,  'b\nac\n'  );
assert_checkequal( strsubst('b\nac\n','/aa$/'                                ,' ' ,'r') ,  'b\nac\n'  );
assert_checkequal( strsubst('b\nac'  ,'/aa\Z/'                               ,' ' ,'r') ,  'b\nac'  );
assert_checkequal( strsubst('b\nac'  ,'/aa\z/'                               ,' ' ,'r') ,  'b\nac'  );
assert_checkequal( strsubst('b\nac'  ,'/aa$/'                                ,' ' ,'r') ,  'b\nac'  );
assert_checkequal( strsubst('ac\nb\n','/aa\Z/m'                              ,' ' ,'r') ,  'ac\nb\n'  );
assert_checkequal( strsubst('ac\nb\n','/aa\z/m'                              ,' ' ,'r') ,  'ac\nb\n'  );
assert_checkequal( strsubst('ac\nb\n','/aa$/m'                               ,' ' ,'r') ,  'ac\nb\n'  );
assert_checkequal( strsubst('b\nac\n','/aa\Z/m'                              ,' ' ,'r') ,  'b\nac\n' );
assert_checkequal( strsubst('b\nac\n','/aa\z/m'                              ,' ' ,'r') ,  'b\nac\n'  );
assert_checkequal( strsubst('b\nac\n','/aa$/m'                               ,' ' ,'r') ,  'b\nac\n' );
assert_checkequal( strsubst('b\nac'  ,'/aa\Z/m'                              ,' ' ,'r') ,  'b\nac'  );
assert_checkequal( strsubst('b\nac'  ,'/aa\z/m'                              ,' ' ,'r') ,  'b\nac'  );
assert_checkequal( strsubst('b\nac'  ,'/aa$/m'                               ,' ' ,'r') ,  'b\nac' );
assert_checkequal( strsubst('ca\nb\n','/aa\Z/'                               ,' ' ,'r') ,  'ca\nb\n'  );
assert_checkequal( strsubst('ca\nb\n','/aa\z/'                               ,' ' ,'r') ,  'ca\nb\n'  );
assert_checkequal( strsubst('ca\nb\n','/aa$/'                                ,' ' ,'r') ,  'ca\nb\n'  );
assert_checkequal( strsubst('b\nca\n','/aa\Z/'                               ,' ' ,'r') ,  'b\nca\n'  );
assert_checkequal( strsubst('b\nca\n','/aa\z/'                               ,' ' ,'r') ,  'b\nca\n' );
assert_checkequal( strsubst('b\nca\n','/aa$/'                                ,' ' ,'r') ,  'b\nca\n'  );
assert_checkequal( strsubst('b\nca'  ,'/aa\Z/'                               ,' ' ,'r') ,  'b\nca'   );
assert_checkequal( strsubst('b\nca'  ,'/aa\z/'                               ,' ' ,'r') , 'b\nca'   );
assert_checkequal( strsubst('b\nca'  ,'/aa$/'                                ,' ' ,'r') ,  'b\nca'  );
assert_checkequal( strsubst('ca\nb\n','/aa\Z/m'                              ,' ' ,'r') , 'ca\nb\n'  );
assert_checkequal( strsubst('ca\nb\n','/aa\z/m'                              ,' ' ,'r') ,  'ca\nb\n'  );
assert_checkequal( strsubst('ca\nb\n','/aa$/m'                               ,' ' ,'r') ,  'ca\nb\n' );
assert_checkequal( strsubst('b\nca\n','/aa\Z/m'                              ,' ' ,'r') , 'b\nca\n'  );
assert_checkequal( strsubst('b\nca\n','/aa\z/m'                              ,' ' ,'r') , 'b\nca\n'  );
assert_checkequal( strsubst('b\nca\n','/aa$/m'                               ,' ' ,'r') , 'b\nca\n'  );
assert_checkequal( strsubst('b\nca'  ,'/aa\Z/m'                              ,' ' ,'r') ,  'b\nca'  );
assert_checkequal( strsubst('b\nca'  ,'/aa\z/m'                              ,' ' ,'r') ,  'b\nca'   );
assert_checkequal( strsubst('b\nca'  ,'/aa$/m'                               ,' ' ,'r') ,  'b\nca'   );
assert_checkequal( strsubst('ab\nb\n','/ab\Z/'                               ,' ' ,'r') ,  'ab\nb\n'  );
assert_checkequal( strsubst('ab\nb\n','/ab\z/'                               ,' ' ,'r') ,  'ab\nb\n' );
assert_checkequal( strsubst('ab\nb\n','/ab$/'                                ,' ' ,'r') , 'ab\nb\n'  );
assert_checkequal( strsubst('b\nab\n','/ab\z/'                               ,' ' ,'r') ,  'b\nab\n' );
assert_checkequal( strsubst('ab\nb\n','/ab\Z/m'                              ,' ' ,'r') ,  'ab\nb\n'  );
assert_checkequal( strsubst('ab\nb\n','/ab\z/m'                              ,' ' ,'r') ,  'ab\nb\n'  );
assert_checkequal( strsubst('b\nab\n','/ab\z/m'                              ,' ' ,'r') , 'b\nab\n'  );
assert_checkequal( strsubst('ac\nb\n','/ab\Z/'                               ,' ' ,'r') , 'ac\nb\n'  );
assert_checkequal( strsubst('ac\nb\n','/ab\z/'                               ,' ' ,'r') , 'ac\nb\n' );
assert_checkequal( strsubst('ac\nb\n','/ab$/'                                ,' ' ,'r') ,  'ac\nb\n'  );
assert_checkequal( strsubst('b\nac\n','/ab\Z/'                               ,' ' ,'r') ,  'b\nac\n'  );
assert_checkequal( strsubst('b\nac\n','/ab\z/'                               ,' ' ,'r') ,  'b\nac\n' );
assert_checkequal( strsubst('b\nac\n','/ab$/'                                ,' ' ,'r') ,  'b\nac\n'  );
assert_checkequal( strsubst('b\nac'  ,'/ab\Z/'                               ,' ' ,'r') ,  'b\nac'  );
assert_checkequal( strsubst('b\nac'  ,'/ab\z/'                               ,' ' ,'r') ,  'b\nac' );
assert_checkequal( strsubst('b\nac'  ,'/ab$/'                                ,' ' ,'r') , 'b\nac' );
assert_checkequal( strsubst('ac\nb\n','/ab\Z/m'                              ,' ' ,'r') ,  'ac\nb\n'  );
assert_checkequal( strsubst('ac\nb\n','/ab\z/m'                              ,' ' ,'r') , 'ac\nb\n'  );
assert_checkequal( strsubst('ac\nb\n','/ab$/m'                               ,' ' ,'r') ,  'ac\nb\n'  );
assert_checkequal( strsubst('b\nac\n','/ab\Z/m'                              ,' ' ,'r') , 'b\nac\n'  );
assert_checkequal( strsubst('b\nac\n','/ab\z/m'                              ,' ' ,'r') , 'b\nac\n' );
assert_checkequal( strsubst('b\nac\n','/ab$/m'                               ,' ' ,'r') ,  'b\nac\n'  );
assert_checkequal( strsubst('b\nac'  ,'/ab\Z/m'                              ,' ' ,'r') ,  'b\nac'   );
assert_checkequal( strsubst('b\nac'  ,'/ab\z/m'                              ,' ' ,'r') ,  'b\nac'  );
assert_checkequal( strsubst('b\nac'  ,'/ab$/m'                               ,' ' ,'r') ,  'b\nac'   );
assert_checkequal( strsubst('ca\nb\n','/ab\Z/'                               ,' ' ,'r') ,  'ca\nb\n' );
assert_checkequal( strsubst('ca\nb\n','/ab\z/'                               ,' ' ,'r') ,  'ca\nb\n'  );
assert_checkequal( strsubst('ca\nb\n','/ab$/'                                ,' ' ,'r') ,  'ca\nb\n'  );
assert_checkequal( strsubst('b\nca\n','/ab\Z/'                               ,' ' ,'r') ,  'b\nca\n'  );
assert_checkequal( strsubst('b\nca\n','/ab\z/'                               ,' ' ,'r') , 'b\nca\n'  );
assert_checkequal( strsubst('b\nca\n','/ab$/'                                ,' ' ,'r') ,  'b\nca\n'  );
assert_checkequal( strsubst('b\nca'  ,'/ab\Z/'                               ,' ' ,'r') ,  'b\nca'  );
assert_checkequal( strsubst('b\nca'  ,'/ab\z/'                               ,' ' ,'r') ,  'b\nca'  );
assert_checkequal( strsubst('b\nca'  ,'/ab$/'                                ,' ' ,'r') , 'b\nca'  );
assert_checkequal( strsubst('ca\nb\n','/ab\Z/m'                              ,' ' ,'r') ,  'ca\nb\n'  );
assert_checkequal( strsubst('ca\nb\n','/ab\z/m'                              ,' ' ,'r') ,  'ca\nb\n' );
assert_checkequal( strsubst('ca\nb\n','/ab$/m'                               ,' ' ,'r') , 'ca\nb\n'  );
assert_checkequal( strsubst('b\nca\n','/ab\Z/m'                              ,' ' ,'r') , 'b\nca\n' );
assert_checkequal( strsubst('b\nca\n' ,'/ab\z/m'                              ,' ' ,'r') , 'b\nca\n'  );
assert_checkequal( strsubst('b\nca\n','/ab$/m'                               ,' ' ,'r') ,  'b\nca\n'  );
assert_checkequal( strsubst('b\nca'     ,'/ab\Z/m'                              ,' ' ,'r') , 'b\nca'   );
assert_checkequal( strsubst('b\nca'     ,'/ab\z/m'                              ,' ' ,'r') ,  'b\nca'   );
assert_checkequal( strsubst('b\nca'     ,'/ab$/m'                               ,' ' ,'r') ,  'b\nca'   );
assert_checkequal( strsubst('abb\nb\n'  ,'/abb\Z/'                              ,' ' ,'r') ,  'abb\nb\n'  );
assert_checkequal( strsubst('abb\nb\n'  ,'/abb\z/'                              ,' ' ,'r') ,  'abb\nb\n'  );
assert_checkequal( strsubst('abb\nb\n'  ,'/abb$/'                               ,' ' ,'r') ,  'abb\nb\n'  );
assert_checkequal( strsubst('b\nabb\n'  ,'/abb\z/'                              ,' ' ,'r') ,  'b\nabb\n'   );
assert_checkequal( strsubst('abb\nb\n'  ,'/abb\Z/m'                             ,' ' ,'r') , 'abb\nb\n'  );
assert_checkequal( strsubst('abb\nb\n'  ,'/abb\z/m'                             ,' ' ,'r') ,  'abb\nb\n'  );
assert_checkequal( strsubst('b\nabb\n','/abb\z/m'                             ,' ' ,'r') ,  'b\nabb\n'  );
assert_checkequal( strsubst('ac\nb\n'  ,'/abb\Z/'                              ,' ' ,'r') ,  'ac\nb\n'  );
assert_checkequal( strsubst('ac\nb\n'  ,'/abb\z/'                              ,' ' ,'r') ,  'ac\nb\n' );
assert_checkequal( strsubst('ac\nb\n'  ,'/abb$/'                               ,' ' ,'r') , 'ac\nb\n'  );
assert_checkequal( strsubst('b\nac\n' ,'/abb\Z/'                              ,' ' ,'r') , 'b\nac\n'  );
assert_checkequal( strsubst('b\nac\n'  ,'/abb\z/'                              ,' ' ,'r') ,  'b\nac\n' );
assert_checkequal( strsubst('b\nac\n'   ,'/abb$/'                               ,' ' ,'r') ,  'b\nac\n'  );
assert_checkequal( strsubst('b\nac'     ,'/abb\Z/'                              ,' ' ,'r') ,  'b\nac'  );
assert_checkequal( strsubst('b\nac'   ,'/abb\z/'                              ,' ' ,'r') , 'b\nac'  );
assert_checkequal( strsubst('b\nac'   ,'/abb$/'                               ,' ' ,'r') ,  'b\nac'  );
assert_checkequal( strsubst('ac\nb\n'   ,'/abb\Z/m'                             ,' ' ,'r') ,  'ac\nb\n'   );
assert_checkequal( strsubst('ac\nb\n'     ,'/abb\z/m'                             ,' ' ,'r') , 'ac\nb\n'   );
assert_checkequal( strsubst('ac\nb\n'  ,'/abb$/m'                              ,' ' ,'r') ,  'ac\nb\n'  );
assert_checkequal( strsubst('b\nac\n' ,'/abb\Z/m'                             ,' ' ,'r') ,  'b\nac\n'   );
assert_checkequal( strsubst('b\nac\n'    ,'/abb\z/m'                             ,' ' ,'r') , 'b\nac\n'   );
assert_checkequal( strsubst('b\nac\n'    ,'/abb$/m'                              ,' ' ,'r') ,  'b\nac\n'   );
assert_checkequal( strsubst('b\nac'     ,'/abb\Z/m'                             ,' ' ,'r') ,  'b\nac'   );
assert_checkequal( strsubst('b\nac'   ,'/abb\z/m'                             ,' ' ,'r') ,  'b\nac'   );
assert_checkequal( strsubst('b\nac'    ,'/abb$/m'                              ,' ' ,'r') ,  'b\nac'   );
assert_checkequal( strsubst('ca\nb\n'   ,'/abb\Z/'                              ,' ' ,'r') ,  'ca\nb\n'  );
assert_checkequal( strsubst('ca\nb\n' ,'/abb\z/'                              ,' ' ,'r') ,  'ca\nb\n'  );
assert_checkequal( strsubst('ca\nb\n'  ,'/abb$/'                               ,' ' ,'r') ,  'ca\nb\n'  );
assert_checkequal( strsubst('b\nca\n' ,'/abb\Z/'                              ,' ' ,'r') ,  'b\nca\n'   );
assert_checkequal( strsubst('b\nca\n'   ,'/abb\z/'                              ,' ' ,'r') ,  'b\nca\n'   );
assert_checkequal( strsubst('b\nca\n'   ,'/abb$/'                               ,' ' ,'r') ,  'b\nca\n'   );
assert_checkequal( strsubst('b\nca'       ,'/abb\Z/'                              ,' ' ,'r') ,  'b\nca'   );
assert_checkequal( strsubst('b\nca'         ,'/abb\z/'                              ,' ' ,'r') , 'b\nca'   );
assert_checkequal( strsubst('b\nca'        ,'/abb$/'                               ,' ' ,'r') ,  'b\nca'   );
assert_checkequal( strsubst('ca\nb\n'     ,'/abb\Z/m'                             ,' ' ,'r') ,  'ca\nb\n'   );
assert_checkequal( strsubst('ca\nb\n'   ,'/abb\z/m'                             ,' ' ,'r') , 'ca\nb\n'   );
assert_checkequal( strsubst('ca\nb\n'    ,'/abb$/m'                              ,' ' ,'r') , 'ca\nb\n'   );
assert_checkequal( strsubst('b\nca\n'     ,'/abb\Z/m'                             ,' ' ,'r') ,  'b\nca\n'   );
assert_checkequal( strsubst('b\nca\n'      ,'/abb\z/m'                             ,' ' ,'r') ,  'b\nca\n'   );
assert_checkequal( strsubst('b\nca\n'  ,'/abb$/m'                              ,' ' ,'r') ,  'b\nca\n'  );
assert_checkequal( strsubst('b\nca'       ,'/abb\Z/m'                             ,' ' ,'r') ,  'b\nca' );
assert_checkequal( strsubst('b\nca'       ,'/abb\z/m'                             ,' ' ,'r') ,  'b\nca'  );
assert_checkequal( strsubst('b\nca'            ,'/abb$/m'                              ,' ' ,'r') ,  'b\nca'  );
assert_checkequal( strsubst('aaab'        ,'/^(?=(a+?))\1ab/'                     ,' ' ,'r') ,  'aaab'   );
assert_checkequal( strsubst('aaab'             ,'/^(?=(a+?))\1ab/'                     ,' ' ,'r') ,  'aaab'   );
assert_checkequal( strsubst('abcd:'               ,'/([\w:]+::)?(\w+)$/'                  ,' ' ,'r') ,  'abcd:'   );
assert_checkequal( strsubst('abcd:'            ,'/([\w:]+::)?(\w+)$/'                  ,' ' ,'r') ,   'abcd:'  );
assert_checkequal( strsubst('aaab'                  ,'/(>a+)ab/'                            ,' ' ,'r') ,  'aaab'   );
//======================================================================================== 
v = "世界您好";
c = "您";
assert_checkequal( strsubst(v,c,'ç'),'世界ç好' );

v = "азеазея";
c = "з";
assert_checkequal( strsubst(v,c,'ç'),'аçеаçея'  );

v = "ድቅስድቅስግ";
c = "ቅ";
assert_checkequal( strsubst(v,c,'ç'), 'ድçስድçስግ'  );

v = "ハロー・ワールド";
c = "ド";
assert_checkequal( strsubst(v,c,'ç'), 'ハロー・ワールç'  );

v = "เฮลโลเวิลด์";
c = "เ"; 
assert_checkequal( strsubst(v,c,'ç'), 'çฮลโลçวิลด์' );

v = "حريات وحقوق";
c = "قوق";
assert_checkequal( strsubst(v,c,'ç'), 'حريات وحç'  );

v = "ഹലോ വേള്‍ഡ";
c = "‍ഡ"; 
assert_checkequal( strsubst(v,c,'ç'),'ഹലോ വേള്ç'  );

v = "תוכנית";
c = "י";
assert_checkequal( strsubst(v,c,'ç'),'תוכנçת'  );
//===============================
// Chinese
str = '世界您好';
assert_checkequal( strsubst(str,'/^世界/','ç','r') , 'ç您好' );

assert_checkequal( strsubst(str,'/世界$/','ç','r') , str );

assert_checkequal( strsubst(str,'/您好$/','ç','r') , '世界ç' );

assert_checkequal( strsubst(str,'/^您好/','ç','r') , str );

assert_checkequal( strsubst(str,'/界您/','ç','r') , '世ç好' );

assert_checkequal( strsubst(str,'/界_您/','ç','r') , str );
//======================================================================================== 
// Russian
str = 'привет мир';
assert_checkequal( strsubst(str,'/^привет/','ç','r') , 'ç мир' );

assert_checkequal( strsubst(str,'/привет$/','ç','r') , str );

assert_checkequal( strsubst(str,'/мир$/','ç','r') , 'привет ç' );

assert_checkequal( strsubst(str,'/^мир/','ç','r') , str );

assert_checkequal( strsubst(str,'/вет\sм/','ç','r') , 'приçир' );

assert_checkequal( strsubst(str,'/вет_м/','ç','r') , 'привет мир' );
//======================================================================================== 
// Cyrilic
str = 'АБВГДЄЖЅЗИІКЛМНОПҀРСТѸФХѠЦЧШЩЪЪІЬѢЮѦѨѪѬѮѰѲѴѤ';

assert_checkequal( strsubst(str,'/^АБВГДЄЖЅЗИІКЛМНОПҀР/','ç','r') , 'çСТѸФХѠЦЧШЩЪЪІЬѢЮѦѨѪѬѮѰѲѴѤ' );

assert_checkequal( strsubst(str,'/АБВГДЄЖЅЗИІКЛМНОПҀР$/','ç','r') , str );

assert_checkequal( strsubst(str,'/ЧШЩЪЪІЬѢЮѦѨѪѬѮѰѲѴѤ$/','ç','r') , 'АБВГДЄЖЅЗИІКЛМНОПҀРСТѸФХѠЦç' );

assert_checkequal( strsubst(str,'/^ЧШЩЪЪІЬѢЮѦѨѪѬѮѰѲѴѤ/','ç','r') , str );

assert_checkequal( strsubst(str,'/ИІКЛМНОПҀРСТѸФХѠЦЧШЩЪ/','ç','r') , 'АБВГДЄЖЅЗçЪІЬѢЮѦѨѪѬѮѰѲѴѤ' );

assert_checkequal( strsubst(str,'/ИІКЛМНОПҀ_РСТѸФХѠЦЧШЩЪ/','ç','r') , str );
//======================================================================================== 
// Japanese
str = '丑丞串乃之乎也云亘亙些亦亥亨亮仔伊伎伍伽佃佑伶侃侑俄侠俣俐侶倭俺倶倦倖偲僅傭儲允兎兜其冥冴冶凄凌凜凛凧凪凰凱函刹劉劫勁勃';

assert_checkequal( strsubst(str,'/^丑丞串乃之乎也云亘亙些亦/','ç','r') , 'ç亥亨亮仔伊伎伍伽佃佑伶侃侑俄侠俣俐侶倭俺倶倦倖偲僅傭儲允兎兜其冥冴冶凄凌凜凛凧凪凰凱函刹劉劫勁勃' );
assert_checkequal( strsubst(str,'/丑丞串乃之乎也云亘亙些亦$/','ç','r') , str );

assert_checkequal( strsubst(str,'/凰凱函刹劉劫勁勃$/','ç','r') , '丑丞串乃之乎也云亘亙些亦亥亨亮仔伊伎伍伽佃佑伶侃侑俄侠俣俐侶倭俺倶倦倖偲僅傭儲允兎兜其冥冴冶凄凌凜凛凧凪ç' );

assert_checkequal( strsubst(str,'/^凰凱函刹劉劫勁勃/','ç','r') , str );

assert_checkequal( strsubst(str,'/亨亮仔伊伎伍伽佃佑伶侃/','ç','r') , '丑丞串乃之乎也云亘亙些亦亥ç侑俄侠俣俐侶倭俺倶倦倖偲僅傭儲允兎兜其冥冴冶凄凌凜凛凧凪凰凱函刹劉劫勁勃' );

assert_checkequal( strsubst(str,'/亨亮仔伊_伎伍伽佃佑伶侃/','ç','r') , str );
//======================================================================================== 
// Thaï
str = 'มกระดุกกระดิก';
assert_checkequal( strsubst(str,'/^มกระดุกกร/','ç','r'),'çะดิก' );

assert_checkequal( strsubst(str,'/มกระดุกกร$/','ç','r') , str );

assert_checkequal( strsubst(str,'/กกระดิก$/','ç','r'),'มกระดุç' );

assert_checkequal( strsubst(str,'/^กกระดิก/','ç','r') , str );

assert_checkequal( strsubst(str,'/ดุกก/','ç','r'),'มกระçระดิก' );

assert_checkequal( strsubst(str,'/ดุก_ก/','ç','r') , str );
//======================================================================================== 
