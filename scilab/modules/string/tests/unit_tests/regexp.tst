// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2007-2008 - INRIA
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
// <-- CLI SHELL MODE -->
//===================================================================
// unit tests regexp
//===================================================================
lf = ascii(10);
if regexp('abc','/abc/','o') <>  1 then pause,end
ierr = execstr("regexp(''abc'',''/abc/'',''r'');","errcatch");
if ierr <> 999 then pause,end
t = 'aaa aab aac aad aae';
pattern = '/aa/';
[start_pos, end_pos, match_str] = regexp(t,pattern,'o');
if size(start_pos,'*') <> 1 then pause,end
if size(end_pos,'*') <> 1 then pause,end
if start_pos <> 1 then pause,end
if end_pos <> 2 then pause,end
//===================================================================
if regexp('abc'                                     ,'/abc/'                                 ) <>  1   then pause,end
if regexp('xbc'                                     ,'/abc/'                                 ) <>  []  then pause,end
if regexp('axc'                                     ,'/abc/'                                 ) <>  []  then pause,end
if regexp('abx'                                     ,'/abc/'                                 ) <>  []  then pause,end
if regexp('xabcy'                                   ,'/abc/'                                 ) <>  2   then pause,end
if regexp('ababc'                                   ,'/abc/'                                 ) <>  3   then pause,end
if regexp('abc'                                     ,'/ab*c/'                                ) <>  1   then pause,end
if regexp('abc'                                     ,'/ab*bc/'                               ) <>  1   then pause,end
if regexp('abbc'                                    ,'/ab*bc/'                               ) <>  1   then pause,end
if regexp('abbbbc'                                  ,'/ab*bc/'                               ) <>  1   then pause,end
if regexp('abbbbc'                                  ,'/.{1}/'                                ) <>  1   then pause,end
if regexp('abbbbc'                                  ,'/.{3,4}/'                              ) <>  1   then pause,end
if regexp('abbbbc'                                  ,'/ab{0,}bc/'                            ) <>  1   then pause,end
if regexp('abbc'                                    ,'/ab+bc/'                               ) <>  1   then pause,end
if regexp('abc'                                     ,'/ab+bc/'                               ) <>  []  then pause,end
if regexp('abq'                                     ,'/ab+bc/'                               ) <>  []  then pause,end
if regexp('abq'                                     ,'/ab{1,}bc/'                            ) <>  []  then pause,end
if regexp('abbbbc'                                  ,'/ab+bc/'                               ) <>  1   then pause,end
if regexp('abbbbc'                                  ,'/ab{1,}bc/'                            ) <>  1   then pause,end
if regexp('abbbbc'                                  ,'/ab{1,3}bc/'                           ) <>  1   then pause,end
if regexp('abbbbc'                                  ,'/ab{3,4}bc/'                           ) <>  1   then pause,end
if regexp('abbbbc'                                  ,'/ab{4,5}bc/'                           ) <>  []  then pause,end
if regexp('abbc'                                    ,'/ab?bc/'                               ) <>  1   then pause,end
if regexp('abc'                                     ,'/ab?bc/'                               ) <>  1   then pause,end
if regexp('abc'                                     ,'/ab{0,1}bc/'                           ) <>  1   then pause,end
if regexp('abbbbc'                                  ,'/ab?bc/'                               ) <>  []  then pause,end
if regexp('abc'                                     ,'/ab?c/'                                ) <>  1   then pause,end
if regexp('abc'                                     ,'/ab{0,1}c/'                            ) <>  1   then pause,end
if regexp('abc'                                     ,'/^abc$/'                               ) <>  1   then pause,end
if regexp('abcc'                                    ,'/^abc$/'                               ) <>  []  then pause,end
if regexp('abcc'                                    ,'/^abc/'                                ) <>  1   then pause,end
if regexp('aabc'                                    ,'/^abc$/'                               ) <>  []  then pause,end
if regexp('aabc'                                    ,'/abc$/'                                ) <>  2   then pause,end
if regexp('aabcd'                                   ,'/abc$/'                                ) <>  []  then pause,end
if regexp('abc'                                     ,'/a.c/'                                 ) <>  1   then pause,end
if regexp('axc'                                     ,'/a.c/'                                 ) <>  1   then pause,end
if regexp('axyzc'                                   ,'/a.*c/'                                ) <>  1   then pause,end
if regexp('axyzd'                                   ,'/a.*c/'                                ) <>  []  then pause,end
if regexp('abc'                                     ,'/a[bc]d/'                              ) <>  []  then pause,end
if regexp('abd'                                     ,'/a[bc]d/'                              ) <>  1   then pause,end
if regexp('abd'                                     ,'/a[b-d]e/'                             ) <>  []  then pause,end
if regexp('ace'                                     ,'/a[b-d]e/'                             ) <>  1   then pause,end
if regexp('aac'                                     ,'/a[b-d]/'                              ) <>  2   then pause,end
if regexp('a-'                                      ,'/a[-b]/'                               ) <>  1   then pause,end
if regexp('a-'                                      ,'/a[b-]/'                               ) <>  1   then pause,end
if regexp('a]'                                      ,'/a]/'                                  ) <>  1   then pause,end
if regexp('a]b'                                     ,'/a[]]b/'                               ) <>  1   then pause,end
if regexp('aed'                                     ,'/a[^bc]d/'                             ) <>  1   then pause,end
if regexp('abd'                                     ,'/a[^bc]d/'                             ) <>  []  then pause,end
if regexp('adc'                                     ,'/a[^-b]c/'                             ) <>  1   then pause,end
if regexp('a-c'                                     ,'/a[^-b]c/'                             ) <>  []  then pause,end
if regexp('a]c'                                     ,'/a[^]b]c/'                             ) <>  []  then pause,end
if regexp('adc'                                     ,'/a[^]b]c/'                             ) <>  1   then pause,end
if regexp('a-'                                      ,'/\ba\b/'                               ) <>  1   then pause,end
if regexp('-a'                                      ,'/\ba\b/'                               ) <>  2   then pause,end
if regexp('-a-'                                     ,'/\ba\b/'                               ) <>  2   then pause,end
if regexp('xy'                                      ,'/\by\b/'                               ) <>  []  then pause,end
if regexp('yz'                                      ,'/\by\b/'                               ) <>  []  then pause,end
if regexp('xyz'                                     ,'/\by\b/'                               ) <>  []  then pause,end
if regexp('a-'                                      ,'/\Ba\B/'                               ) <>  []  then pause,end
if regexp('-a'                                      ,'/\Ba\B/'                               ) <>  []  then pause,end
if regexp('-a-'                                     ,'/\Ba\B/'                               ) <>  []  then pause,end
if regexp('xy'                                      ,'/\By\b/'                               ) <>  2   then pause,end
if regexp('xy'                                      ,'/\By\b/'                               ) <>  2   then pause,end
if regexp('yz'                                      ,'/\by\B/'                               ) <>  1   then pause,end
if regexp('xyz'                                     ,'/\By\B/'                               ) <>  2   then pause,end
if regexp('a'                                       ,'/\w/'                                  ) <>  1   then pause,end
if regexp('-'                                       ,'/\w/'                                  ) <>  []  then pause,end
if regexp('a'                                       ,'/\W/'                                  ) <>  []  then pause,end
if regexp('-'                                       ,'/\W/'                                  ) <>  1   then pause,end
if regexp('a b'                                     ,'/a\sb/'                                ) <>  1   then pause,end
if regexp('a-b'                                     ,'/a\sb/'                                ) <>  []  then pause,end
if regexp('a b'                                     ,'/a\Sb/'                                ) <>  []  then pause,end
if regexp('a-b'                                     ,'/a\Sb/'                                ) <>  1   then pause,end
if regexp('1'                                       ,'/\d/'                                  ) <>  1   then pause,end
if regexp('-'                                       ,'/\d/'                                  ) <>  []  then pause,end
if regexp('1'                                       ,'/\D/'                                  ) <>  []  then pause,end
if regexp('-'                                       ,'/\D/'                                  ) <>  1   then pause,end
if regexp('a'                                       ,'/[\w]/'                                ) <>  1   then pause,end
if regexp('-'                                       ,'/[\w]/'                                ) <>  []  then pause,end
if regexp('a'                                       ,'/[\W]/'                                ) <>  []  then pause,end
if regexp('-'                                       ,'/[\W]/'                                ) <>  1   then pause,end
if regexp('a b'                                     ,'/a[\s]b/'                              ) <>  1   then pause,end
if regexp('a-b'                                     ,'/a[\s]b/'                              ) <>  []  then pause,end
if regexp('a b'                                     ,'/a[\S]b/'                              ) <>  []  then pause,end
if regexp('a-b'                                     ,'/a[\S]b/'                              ) <>  1   then pause,end
if regexp('1'                                       ,'/[\d]/'                                ) <>  1   then pause,end
if regexp('-'                                       ,'/[\d]/'                                ) <>  []  then pause,end
if regexp('1'                                       ,'/[\D]/'                                ) <>  []  then pause,end
if regexp('-'                                       ,'/[\D]/'                                ) <>  1   then pause,end
if regexp('abc'                                     ,'/ab|cd/'                               ) <>  1   then pause,end
if regexp('abcd'                                    ,'/ab|cd/'                               ) <>  1   then pause,end
if regexp('b'                                       ,'/$b/'                                  ) <>  []  then pause,end
if regexp('ab'                                      ,'/a\(*b/'                               ) <>  1   then pause,end
if regexp('a((b'                                    ,'/a\(*b/'                               ) <>  1   then pause,end
if regexp('aabbabc'                                 ,'/a+b+c/'                               ) <>  5   then pause,end
if regexp('aabbabc'                                 ,'/a{1,}b{1,}c/'                         ) <>  5   then pause,end
if regexp('abcabc'                                  ,'/a.+?c/'                               ) <>  1   then pause,end
if regexp('cde'                                     ,'/[^ab]*/'                              ) <>  1   then pause,end
if regexp(''                                        ,'/abc/'                                 ) <>  []  then pause,end
if regexp(''                                        ,'/a*/'                                  ) <>  []  then pause,end
if regexp('e'                                       ,'/a|b|c|d|e/'                           ) <>  1   then pause,end
if regexp('abcdefg'                                 ,'/abcd*efg/'                            ) <>  1   then pause,end
if regexp('xabyabbbz'                               ,'/ab*/'                                 ) <>  [2 5]   then pause,end
if regexp('xayabbbz'                                ,'/ab*/'                              ) <>  [2 4]   then pause,end
if regexp('hij'                                     ,'/[abhgefdc]ij/'                        ) <>  1   then pause,end
if regexp('abcde'                                   ,'/^(ab|cd)e/'                           ) <>  []  then pause,end
if regexp('adcdcde'                                 ,'/a[bcd]*dcdcde/'                       ) <>  1   then pause,end
if regexp('adcdcde'                                 ,'/a[bcd]+dcdcde/'                       ) <>  []  then pause,end
if regexp('alpha'                                   ,'/[a-zA-Z_][a-zA-Z0-9_]*/'              ) <>  1   then pause,end
if regexp('effg'                                    ,'/(bc+d$|ef*g.|h?i(j|k))/'              ) <>  []  then pause,end
if regexp('bcdd'                                    ,'/(bc+d$|ef*g.|h?i(j|k))/'              ) <>  []  then pause,end
if regexp('aa'                                      ,'/((((((((((a))))))))))\10/'            ) <>  1   then pause,end
if regexp('aa'                                      ,'/((((((((((a))))))))))\041/'           ) <>  []  then pause,end
if regexp('a!'                                      ,'/((((((((((a))))))))))\041/'           ) <>  1   then pause,end
if regexp('a'                                       ,'/(((((((((a)))))))))/'                 ) <>  1   then pause,end
if regexp('uh-uh'                                   ,'/multiple words of text/'              ) <>  []  then pause,end
if regexp('multiple words, yeah'                    ,'/multiple words/'                      ) <>  1   then pause,end
if regexp('ab'                                      ,'/[k]/'                                 ) <>  []  then pause,end
if regexp('ac'                                      ,'/a[-]?c/'                              ) <>  1   then pause,end
if regexp('a'                                       ,'/(a)|\1/'                              ) <>  1   then pause,end
if regexp('x'                                       ,'/(a)|\1/'                              ) <>  []  then pause,end
if regexp('aaxabxbaxbbx'                            ,'/((\3|b)\2(a)x)+/'                     ) <>  []  then pause,end
if regexp('ABC'                                     ,'/abc/i'                                ) <>  1   then pause,end
if regexp('XBC'                                     ,'/abc/i'                                ) <>  []  then pause,end
if regexp('AXC'                                     ,'/abc/i'                                ) <>  []  then pause,end
if regexp('ABX'                                     ,'/abc/i'                                ) <>  []  then pause,end
if regexp('XABCY'                                   ,'/abc/i'                                ) <>  2   then pause,end
if regexp('ABABC'                                   ,'/abc/i'                                ) <>  3   then pause,end
if regexp('ABC'                                     ,'/ab*c/i'                               ) <>  1   then pause,end
if regexp('ABC'                                     ,'/ab*bc/i'                              ) <>  1   then pause,end
if regexp('ABBC'                                    ,'/ab*bc/i'                              ) <>  1   then pause,end
if regexp('ABBBBC'                                  ,'/ab*?bc/i'                             ) <>  1   then pause,end
if regexp('ABBBBC'                                  ,'/ab{0,}?bc/i'                          ) <>  1   then pause,end
if regexp('ABBC'                                    ,'/ab+?bc/i'                             ) <>  1   then pause,end
if regexp('ABC'                                     ,'/ab+bc/i'                              ) <>  []  then pause,end
if regexp('ABQ'                                     ,'/ab+bc/i'                              ) <>  []  then pause,end
if regexp('ABQ'                                     ,'/ab{1,}bc/i'                           ) <>  []  then pause,end
if regexp('ABBBBC'                                  ,'/ab+bc/i'                              ) <>  1   then pause,end
if regexp('ABBBBC'                                  ,'/ab{1,}?bc/i'                          ) <>  1   then pause,end
if regexp('ABBBBC'                                  ,'/ab{1,3}?bc/i'                         ) <>  1   then pause,end
if regexp('ABBBBC'                                  ,'/ab{3,4}?bc/i'                         ) <>  1   then pause,end
if regexp('ABBBBC'                                  ,'/ab{4,5}?bc/i'                         ) <>  []  then pause,end
if regexp('ABBC'                                    ,'/ab??bc/i'                             ) <>  1   then pause,end
if regexp('ABC'                                     ,'/ab??bc/i'                             ) <>  1   then pause,end
if regexp('ABC'                                     ,'/ab{0,1}?bc/i'                         ) <>  1   then pause,end
if regexp('ABBBBC'                                  ,'/ab??bc/i'                             ) <>  []  then pause,end
if regexp('ABC'                                     ,'/ab??c/i'                              ) <>  1   then pause,end
if regexp('ABC'                                     ,'/ab{0,1}?c/i'                          ) <>  1   then pause,end
if regexp('ABC'                                     ,'/^abc$/i'                              ) <>  1   then pause,end
if regexp('ABCC'                                    ,'/^abc$/i'                              ) <>  []  then pause,end
if regexp('ABCC'                                    ,'/^abc/i'                               ) <>  1   then pause,end
if regexp('AABC'                                    ,'/^abc$/i'                              ) <>  []  then pause,end
if regexp('AABC'                                    ,'/abc$/i'                               ) <>  2   then pause,end
if regexp('ABC'                                     ,'/a.c/i'                                ) <>  1   then pause,end
if regexp('AXC'                                     ,'/a.c/i'                                ) <>  1   then pause,end
if regexp('AXYZC'                                   ,'/a.*?c/i'                              ) <>  1   then pause,end
if regexp('AXYZD'                                   ,'/a.*c/i'                               ) <>  []  then pause,end
if regexp('ABC'                                     ,'/a[bc]d/i'                             ) <>  []  then pause,end
if regexp('ABD'                                     ,'/a[bc]d/i'                             ) <>  1   then pause,end
if regexp('ABD'                                     ,'/a[b-d]e/i'                            ) <>  []  then pause,end
if regexp('ACE'                                     ,'/a[b-d]e/i'                            ) <>  1   then pause,end
if regexp('AAC'                                     ,'/a[b-d]/i'                             ) <>  2   then pause,end
if regexp('A-'                                      ,'/a[-b]/i'                              ) <>  1   then pause,end
if regexp('A-'                                      ,'/a[b-]/i'                              ) <>  1   then pause,end
if regexp('A]'                                      ,'/a]/i'                                 ) <>  1   then pause,end
if regexp('A]B'                                     ,'/a[]]b/i'                              ) <>  1   then pause,end
if regexp('AED'                                     ,'/a[^bc]d/i'                            ) <>  1   then pause,end
if regexp('ABD'                                     ,'/a[^bc]d/i'                            ) <>  []  then pause,end
if regexp('ADC'                                     ,'/a[^-b]c/i'                            ) <>  1   then pause,end
if regexp('A-C'                                     ,'/a[^-b]c/i'                            ) <>  []  then pause,end
if regexp('A]C'                                     ,'/a[^]b]c/i'                            ) <>  []  then pause,end
if regexp('ADC'                                     ,'/a[^]b]c/i'                            ) <>  1   then pause,end
if regexp('ABC'                                     ,'/ab|cd/i'                              ) <>  1   then pause,end
if regexp('ABCD'                                    ,'/ab|cd/i'                              ) <>  1   then pause,end
if regexp('B'                                       ,'/$b/i'                                 ) <>  []  then pause,end
if regexp('AB'                                      ,'/a\(*b/i'                              ) <>  1   then pause,end
if regexp('A((B'                                    ,'/a\(*b/i'                              ) <>  1   then pause,end
if regexp('AABBABC'                                 ,'/a+b+c/i'                              ) <>  5   then pause,end
if regexp('AABBABC'                                 ,'/a{1,}b{1,}c/i'                        ) <>  5   then pause,end
if regexp('ABCABC'                                  ,'/a.+?c/i'                              ) <>  [1 4]   then pause,end
if regexp('ABCABC'                                  ,'/a.*?c/i'                              ) <>  1   then pause,end
if regexp('ABCABC'                                  ,'/a.{0,5}?c/i'                          ) <>  1   then pause,end
if regexp('CDE'                                     ,'/[^ab]*/i'                             ) <>  1   then pause,end
if regexp(''                                        ,'/abc/i'                                ) <>  []  then pause,end
if regexp(''                                        ,'/a*/i'                                 ) <>  []  then pause,end
if regexp('E'                                       ,'/a|b|c|d|e/i'                          ) <>  1   then pause,end
if regexp('ABCDEFG'                                 ,'/abcd*efg/i'                           ) <>  1   then pause,end
if regexp('XABYABBBZ'                               ,'/ab*/i'                                ) <>  [2 5]   then pause,end
if regexp('XAYABBBZ'                                ,'/ab*/i'                                ) <>  [2 4]   then pause,end
if regexp('HIJ'                                     ,'/[abhgefdc]ij/i'                       ) <>  1   then pause,end
if regexp('ABCDE'                                   ,'/^(ab|cd)e/i'                          ) <>  []  then pause,end
if regexp('ADCDCDE'                                 ,'/a[bcd]*dcdcde/i'                      ) <>  1   then pause,end
if regexp('ADCDCDE'                                 ,'/a[bcd]+dcdcde/i'                      ) <>  []  then pause,end
if regexp('ALPHA'                                   ,'/[a-zA-Z_][a-zA-Z0-9_]*/i'             ) <>  1   then pause,end
if regexp('EFFG'                                    ,'/(bc+d$|ef*g.|h?i(j|k))/i'             ) <>  []  then pause,end
if regexp('BCDD'                                    ,'/(bc+d$|ef*g.|h?i(j|k))/i'             ) <>  []  then pause,end
if regexp('AA'                                      ,'/((((((((((a))))))))))\10/i'           ) <>  1   then pause,end
if regexp('AA'                                      ,'/((((((((((a))))))))))\041/i'          ) <>  []  then pause,end
if regexp('A!'                                      ,'/((((((((((a))))))))))\041/i'          ) <>  1   then pause,end
if regexp('A'                                       ,'/(((((((((a)))))))))/i'                ) <>  1   then pause,end
if regexp('UH-UH'                                   ,'/multiple words of text/i'             ) <>  []  then pause,end
if regexp('MULTIPLE WORDS, YEAH'                    ,'/multiple words/i'                     ) <>  1   then pause,end
if regexp('AB'                                      ,'/[k]/i'                                ) <>  []  then pause,end
if regexp('AC'                                      ,'/a[-]?c/i'                             ) <>  1   then pause,end
if regexp('abad'                                    ,'/a(?!b)./'                             ) <>  3   then pause,end
if regexp('abad'                                    ,'/a(?=d)./'                             ) <>  3   then pause,end
if regexp('abad'                                    ,'/a(?=c|d)./'                           ) <>  3   then pause,end
if regexp('<&OUT'                                   ,'/^[<>]&/'                              ) <>  1   then pause,end
if regexp('aaaaaaaaa'                               ,'/^(a\1?){4}$/'                         ) <>  []  then pause,end
if regexp('aaaaaaaaaaa'                             ,'/^(a\1?){4}$/'                         ) <>  []  then pause,end
if regexp('aaaaaaaaa'                               ,'/^(a(?(1)\1)){4}$/'                    ) <>  []  then pause,end
if regexp('aaaaaaaaaaa'                             ,'/^(a(?(1)\1)){4}$/'                    ) <>  []  then pause,end
if regexp('ab'                                      ,'/(?<=a)b/'                             ) <>  2   then pause,end
if regexp('cb'                                      ,'/(?<=a)b/'                             ) <>  []  then pause,end
if regexp('b'                                       ,'/(?<=a)b/'                             ) <>  []  then pause,end
if regexp('ab'                                      ,'/(?<!c)b/'                             ) <>  2   then pause,end
if regexp('cb'                                      ,'/(?<!c)b/'                             ) <>  []  then pause,end
if regexp('b'                                       ,'/(?<!c)b/'                             ) <>  1   then pause,end
if regexp('b'                                       ,'/(?<!c)b/'                             ) <>  1   then pause,end
if regexp('aba'                                     ,'/(?:..)*a/'                            ) <>  1   then pause,end
if regexp('aba'                                     ,'/(?:..)*?a/'                           ) <>  1   then pause,end
if regexp('abc'                                     ,'/^(?:b|a(?=(.)))*\1/'                  ) <>  1   then pause,end
if regexp('ab'                                      ,'/(?:(?i)a)b/'                          ) <>  1   then pause,end
if regexp('Ab'                                      ,'/(?:(?i)a)b/'                          ) <>  1   then pause,end
if regexp('aB'                                      ,'/(?:(?i)a)b/'                          ) <>  []  then pause,end
if regexp('aB'                                      ,'/((?i)a)b/'                            ) <>  []  then pause,end
if regexp('ab'                                      ,'/(?i:a)b/'                             ) <>  1   then pause,end
if regexp('Ab'                                      ,'/(?i:a)b/'                             ) <>  1   then pause,end
if regexp('aB'                                      ,'/(?i:a)b/'                             ) <>  []  then pause,end
if regexp('aB'                                      ,'/((?i:a))b/'                           ) <>  []  then pause,end
if regexp('ab'                                      ,'/(?:(?-i)a)b/i'                        ) <>  1   then pause,end
if regexp('aB'                                      ,'/(?:(?-i)a)b/i'                        ) <>  1   then pause,end
if regexp('Ab'                                      ,'/(?:(?-i)a)b/i'                        ) <>  []  then pause,end
if regexp('Ab'                                      ,'/((?-i)a)b/i'                          ) <>  []  then pause,end
if regexp('aB'                                      ,'/(?:(?-i)a)b/i'                        ) <>  1   then pause,end
if regexp('AB'                                      ,'/(?:(?-i)a)b/i'                        ) <>  []  then pause,end
if regexp('AB'                                      ,'/((?-i)a)b/i'                          ) <>  []  then pause,end
if regexp('ab'                                      ,'/(?-i:a)b/i'                           ) <>  1   then pause,end
if regexp('aB'                                      ,'/(?-i:a)b/i'                           ) <>  1   then pause,end
if regexp('Ab'                                      ,'/(?-i:a)b/i'                           ) <>  []  then pause,end
if regexp('Ab'                                      ,'/((?-i:a))b/i'                         ) <>  []  then pause,end
if regexp('aB'                                      ,'/(?-i:a)b/i'                           ) <>  1   then pause,end
if regexp('AB'                                      ,'/(?-i:a)b/i'                           ) <>  []  then pause,end
if regexp('AB'                                      ,'/((?-i:a))b/i'                         ) <>  []  then pause,end
if regexp('a'+lf+'B'                                ,'/((?-i:a.))b/i'                        ) <>  []  then pause,end
if regexp('B'+lf+'B'                                ,'/((?s-i:a.))b/i'                       ) <>  []  then pause,end
if regexp('cabbbb'                                  ,'/(?:c|d)(?:)(?:a(?:)(?:b)(?:b(?:))(?:b(?:)(?:b)))/') <>  1   then pause,end
if regexp('caaaaaaaabbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb','/(?:c|d)(?:)(?:aaaaaaaa(?:)(?:bbbbbbbb)(?:bbbbbbbb(?:))(?:bbbbbbbb(?:)(?:bbbbbbbb)))/') <>  1   then pause,end
if regexp('foobar1234baz'                           ,'/foo\w*\d{4}baz/'                      ) <>  1   then pause,end
if regexp('x~~'                                     ,'/x(~~)*(?:(?:F)?)?/'                   ) <>  1   then pause,end
if regexp('aaac'                                    ,'/^a(?#xxx){3}c/'                       ) <>  1   then pause,end
if regexp('aaac'                                    ,'/^a (?#xxx) (?#yyy) {3}c/x'            ) <>  1   then pause,end
if regexp('dbcb'                                    ,'/(?<![cd])b/'                          ) <>  []  then pause,end
if regexp('dbaacb'                                  ,'/(?<![cd])[ab]/'                       ) <>  [3 4]   then pause,end
if regexp('dbcb'                                    ,'/(?<!(c|d))b/'                         ) <>  []  then pause,end
if regexp('dbaacb'                                  ,'/(?<!(c|d))[ab]/'                      ) <>  [3 4]   then pause,end
if regexp('cdaccb'                                  ,'/(?<!cd)[ab]/'                         ) <>  6   then pause,end
if regexp('a--'                                     ,'/^(?:a?b?)*$/'                         ) <>  []  then pause,end
if regexp('a'+lf+'b'+lf                             ,'/(?m)^b/'                              ) <>  [3 4]   then pause,end
if regexp('a'+lf+'b'+lf+'c'+lf                      ,'/^b/'                                  ) <>  []  then pause,end
if regexp('a'+lf+'b'+lf+'c'+lf                      ,'/()^b/'                                ) <>  []  then pause,end
if regexp('a'                                       ,'/(x)?(?(1)a|b)/'                       ) <>  []  then pause,end
if regexp('a'                                       ,'/(x)?(?(1)b|a)/'                       ) <>  1   then pause,end
if regexp('a'                                       ,'/()?(?(1)b|a)/'                        ) <>  1   then pause,end
if regexp('a'                                       ,'/()(?(1)b|a)/'                         ) <>  []  then pause,end
if regexp('a'                                       ,'/()?(?(1)a|b)/'                        ) <>  1   then pause,end
if regexp('blah)'                                   ,'/^(\()?blah(?(1)(\)))$/'               ) <>  []  then pause,end
if regexp('(blah'                                   ,'/^(\()?blah(?(1)(\)))$/'               ) <>  []  then pause,end
if regexp('blah)'                                   ,'/^(\(+)?blah(?(1)(\)))$/'              ) <>  []  then pause,end
if regexp('(blah'                                   ,'/^(\(+)?blah(?(1)(\)))$/'              ) <>  []  then pause,end
if regexp('a'                                       ,'/(?(?!a)a|b)/'                         ) <>  []  then pause,end
if regexp('a'                                       ,'/(?(?!a)b|a)/'                         ) <>  1   then pause,end
if regexp('a'                                       ,'/(?(?=a)b|a)/'                         ) <>  []  then pause,end
if regexp('a'                                       ,'/(?(?=a)a|b)/'                         ) <>  1   then pause,end
if regexp('aaab'                                    ,'/^(?=(a+?))\1ab/'                      ) <>  []  then pause,end
if regexp('aaab'                                    ,'/^(?=(a+?))\1ab/'                      ) <>  []  then pause,end
if regexp('abcd:'                                   ,'/([\w:]+::)?(\w+)$/'                   ) <>  []  then pause,end
if regexp('abcd:'                                   ,'/([\w:]+::)?(\w+)$/'                   ) <>  []  then pause,end
if regexp('aaab'                                    ,'/(>a+)ab/'                             ) <>  []  then pause,end
if regexp('aaab'                                    ,'/(?>a+)b/'                             ) <>  1   then pause,end
if regexp('abc'                                     ,'/[a[:]b[:c]/'                          ) <>  1   then pause,end
if regexp('abc'                                     ,'/[a[:]b[:c]/'                          ) <>  1   then pause,end
if regexp('((abc(ade)ufh()()x'                      ,'/((?>[^()]+)|\([^()]*\))+/'            ) <>  3   then pause,end
//===================================================================
if regexp('a'+lf+'b'+lf                             ,'/a\Z/'                                 ) <>  []  then pause,end
if regexp('a'+lf+'b'+lf                             ,'/a\z/'                                 ) <>  []  then pause,end
if regexp('a'+lf+'b'+lf                             ,'/a$/'                                  ) <>  []  then pause,end
if regexp('b'+lf+'a'+lf                             ,'/a\z/'                                 ) <>  []  then pause,end
if regexp('a'+lf+'b'+lf                             ,'/a\Z/m'                                ) <>  []  then pause,end
if regexp('a'+lf+'b'+lf                             ,'/a\z/m'                                ) <>  []  then pause,end
if regexp('b'+lf+'a'+lf                             ,'/a\z/m'                                ) <>  []  then pause,end
if regexp('aa'+lf+'b'+lf                            ,'/aa\Z/'                                ) <>  []  then pause,end
if regexp('aa'+lf+'b'+lf                            ,'/aa\z/'                                ) <>  []  then pause,end
if regexp('aa'+lf+'b'+lf                            ,'/aa$/'                                 ) <>  []  then pause,end
if regexp('b'+lf+'aa'+lf                            ,'/aa\z/'                                ) <>  []  then pause,end
if regexp('aa'+lf+'b'+lf                            ,'/aa\Z/m'                               ) <>  []  then pause,end
if regexp('aa'+lf+'b'+lf                            ,'/aa\z/m'                               ) <>  []  then pause,end
if regexp('b'+lf+'aa'+lf                            ,'/aa\z/m'                               ) <>  []  then pause,end
if regexp('ac'+lf+'b'+lf                            ,'/aa\Z/'                                ) <>  []  then pause,end
if regexp('ac'+lf+'b'+lf                            ,'/aa\z/'                                ) <>  []  then pause,end
if regexp('ac'+lf+'b'+lf                            ,'/aa$/'                                 ) <>  []  then pause,end
if regexp('b'+lf+'ac'+lf                            ,'/aa\Z/'                                ) <>  []  then pause,end
if regexp('b'+lf+'ac'+lf                            ,'/aa\z/'                                ) <>  []  then pause,end
if regexp('b'+lf+'ac'+lf                            ,'/aa$/'                                 ) <>  []  then pause,end
if regexp('b'+lf+'ac'                               ,'/aa\Z/'                                ) <>  []  then pause,end
if regexp('b'+lf+'ac'                               ,'/aa\z/'                                ) <>  []  then pause,end
if regexp('b'+lf+'ac'                               ,'/aa$/'                                 ) <>  []  then pause,end
if regexp('ac'+lf+'b'+lf                            ,'/aa\Z/m'                               ) <>  []  then pause,end
if regexp('ac'+lf+'b'+lf                            ,'/aa\z/m'                               ) <>  []  then pause,end
if regexp('ac'+lf+'b'+lf                            ,'/aa$/m'                                ) <>  []  then pause,end
if regexp('b'+lf+'ac'+lf                            ,'/aa\Z/m'                               ) <>  []  then pause,end
if regexp('b'+lf+'ac'+lf                            ,'/aa\z/m'                               ) <>  []  then pause,end
if regexp('b'+lf+'ac'+lf                            ,'/aa$/m'                                ) <>  []  then pause,end
if regexp('b'+lf+'ac'                               ,'/aa\Z/m'                               ) <>  []  then pause,end
if regexp('b'+lf+'ac'                               ,'/aa\z/m'                               ) <>  []  then pause,end
if regexp('b'+lf+'ac'                               ,'/aa$/m'                                ) <>  []  then pause,end
if regexp('ca'+lf+'b'+lf                            ,'/aa\Z/'                                ) <>  []  then pause,end
if regexp('ca'+lf+'b'+lf                            ,'/aa\z/'                                ) <>  []  then pause,end
if regexp('ca'+lf+'b'+lf                            ,'/aa$/'                                 ) <>  []  then pause,end
if regexp('b'+lf+'ca'+lf                            ,'/aa\Z/'                                ) <>  []  then pause,end
if regexp('b'+lf+'ca'+lf                            ,'/aa\z/'                                ) <>  []  then pause,end
if regexp('b'+lf+'ca'+lf                            ,'/aa$/'                                 ) <>  []  then pause,end
if regexp('b'+lf+'ca'                               ,'/aa\Z/'                                ) <>  []  then pause,end
if regexp('b'+lf+'ca'                               ,'/aa\z/'                                ) <>  []  then pause,end
if regexp('b'+lf+'ca'                               ,'/aa$/'                                 ) <>  []  then pause,end
if regexp('ca'+lf+'b'+lf                            ,'/aa\Z/m'                               ) <>  []  then pause,end
if regexp('ca'+lf+'b'+lf                            ,'/aa\z/m'                               ) <>  []  then pause,end
if regexp('ca'+lf+'b'+lf                            ,'/aa$/m'                                ) <>  []  then pause,end
if regexp('b'+lf+'ca'+lf                            ,'/aa\Z/m'                               ) <>  []  then pause,end
if regexp('b'+lf+'ca'+lf                            ,'/aa\z/m'                               ) <>  []  then pause,end
if regexp('b'+lf+'ca'+lf                            ,'/aa$/m'                                ) <>  []  then pause,end
if regexp('b'+lf+'ca'                               ,'/aa\Z/m'                               ) <>  []  then pause,end
if regexp('b'+lf+'ca'                               ,'/aa\z/m'                               ) <>  []  then pause,end
if regexp('b'+lf+'ca'                               ,'/aa$/m'                                ) <>  []  then pause,end
if regexp('ab'+lf+'b'+lf                            ,'/ab\Z/'                                ) <>  []  then pause,end
if regexp('ab'+lf+'b'+lf                            ,'/ab\z/'                                ) <>  []  then pause,end
if regexp('ab'+lf+'b'+lf                            ,'/ab$/'                                 ) <>  []  then pause,end
if regexp('b'+lf+'ab'+lf                            ,'/ab\z/'                                ) <>  []  then pause,end
if regexp('ab'+lf+'b'+lf                            ,'/ab\Z/m'                               ) <>  []  then pause,end
if regexp('ab'+lf+'b'+lf                            ,'/ab\z/m'                               ) <>  []  then pause,end
if regexp('b'+lf+'ab'+lf                            ,'/ab\z/m'                               ) <>  []  then pause,end
if regexp('ac'+lf+'b'+lf                            ,'/ab\Z/'                                ) <>  []  then pause,end
if regexp('ac'+lf+'b'+lf                            ,'/ab\z/'                                ) <>  []  then pause,end
if regexp('ac'+lf+'b'+lf                            ,'/ab$/'                                 ) <>  []  then pause,end
if regexp('b'+lf+'ac'+lf                            ,'/ab\Z/'                                ) <>  []  then pause,end
if regexp('b'+lf+'ac'+lf                            ,'/ab\z/'                                ) <>  []  then pause,end
if regexp('b'+lf+'ac'+lf                            ,'/ab$/'                                 ) <>  []  then pause,end
if regexp('b'+lf+'ac'                               ,'/ab\Z/'                                ) <>  []  then pause,end
if regexp('b'+lf+'ac'                               ,'/ab\z/'                                ) <>  []  then pause,end
if regexp('b'+lf+'ac'                               ,'/ab$/'                                 ) <>  []  then pause,end
if regexp('ac'+lf+'b'+lf                            ,'/ab\Z/m'                               ) <>  []  then pause,end
if regexp('ac'+lf+'b'+lf                            ,'/ab\z/m'                               ) <>  []  then pause,end
if regexp('ac'+lf+'b'+lf                            ,'/ab$/m'                                ) <>  []  then pause,end
if regexp('b'+lf+'ac'+lf                            ,'/ab\Z/m'                               ) <>  []  then pause,end
if regexp('b'+lf+'ac'+lf                            ,'/ab\z/m'                               ) <>  []  then pause,end
if regexp('b'+lf+'ac'+lf                            ,'/ab$/m'                                ) <>  []  then pause,end
if regexp('b'+lf+'ac'                               ,'/ab\Z/m'                               ) <>  []  then pause,end
if regexp('b'+lf+'ac'                               ,'/ab\z/m'                               ) <>  []  then pause,end
if regexp('b'+lf+'ac'                               ,'/ab$/m'                                ) <>  []  then pause,end
if regexp('ca'+lf+'b'+lf                            ,'/ab\Z/'                                ) <>  []  then pause,end
if regexp('ca'+lf+'b'+lf                            ,'/ab\z/'                                ) <>  []  then pause,end
if regexp('ca'+lf+'b'+lf                            ,'/ab$/'                                 ) <>  []  then pause,end
if regexp('b'+lf+'ca'+lf                            ,'/ab\Z/'                                ) <>  []  then pause,end
if regexp('b'+lf+'ca'+lf                            ,'/ab\z/'                                ) <>  []  then pause,end
if regexp('b'+lf+'ca'+lf                            ,'/ab$/'                                 ) <>  []  then pause,end
if regexp('b'+lf+'ca'                               ,'/ab\Z/'                                ) <>  []  then pause,end
if regexp('b'+lf+'ca'                               ,'/ab\z/'                                ) <>  []  then pause,end
if regexp('b'+lf+'ca'                               ,'/ab$/'                                 ) <>  []  then pause,end
if regexp('ca'+lf+'b'+lf                            ,'/ab\Z/m'                               ) <>  []  then pause,end
if regexp('ca'+lf+'b'+lf                            ,'/ab\z/m'                               ) <>  []  then pause,end
if regexp('ca'+lf+'b'+lf                            ,'/ab$/m'                                ) <>  []  then pause,end
if regexp('b'+lf+'ca'+lf                            ,'/ab\Z/m'                               ) <>  []  then pause,end
if regexp('b'+lf+'ca'+lf                            ,'/ab\z/m'                               ) <>  []  then pause,end
if regexp('b'+lf+'ca'+lf                            ,'/ab$/m'                                ) <>  []  then pause,end
if regexp('b'+lf+'ca'                               ,'/ab\Z/m'                               ) <>  []  then pause,end
if regexp('b'+lf+'ca'                               ,'/ab\z/m'                               ) <>  []  then pause,end
if regexp('b'+lf+'ca'                               ,'/ab$/m'                                ) <>  []  then pause,end
if regexp('abb'+lf+'b'+lf                           ,'/abb\Z/'                               ) <>  []  then pause,end
if regexp('abb'+lf+'b'+lf                           ,'/abb\z/'                               ) <>  []  then pause,end
if regexp('abb'+lf+'b'+lf                           ,'/abb$/'                                ) <>  []  then pause,end
if regexp('b'+lf+'abb'+lf                           ,'/abb\z/'                               ) <>  []  then pause,end
if regexp('abb'+lf+'b'+lf                           ,'/abb\Z/m'                              ) <>  []  then pause,end
if regexp('abb'+lf+'b'+lf                           ,'/abb\z/m'                              ) <>  []  then pause,end
if regexp('b'+lf+'abb'+lf                           ,'/abb\z/m'                              ) <>  []  then pause,end
if regexp('ac'+lf+'b'+lf                            ,'/abb\Z/'                               ) <>  []  then pause,end
if regexp('ac'+lf+'b'+lf                            ,'/abb\z/'                               ) <>  []  then pause,end
if regexp('ac'+lf+'b'+lf                            ,'/abb$/'                                ) <>  []  then pause,end
if regexp('b'+lf+'ac'+lf                            ,'/abb\Z/'                               ) <>  []  then pause,end
if regexp('b'+lf+'ac'+lf                            ,'/abb\z/'                               ) <>  []  then pause,end
if regexp('b'+lf+'ac'+lf                            ,'/abb$/'                                ) <>  []  then pause,end
if regexp('b'+lf+'ac'                               ,'/abb\Z/'                               ) <>  []  then pause,end
if regexp('b'+lf+'ac'                               ,'/abb\z/'                               ) <>  []  then pause,end
if regexp('b'+lf+'ac'                               ,'/abb$/'                                ) <>  []  then pause,end
if regexp('ac'+lf+'b'+lf                            ,'/abb\Z/m'                              ) <>  []  then pause,end
if regexp('ac'+lf+'b'+lf                            ,'/abb\z/m'                              ) <>  []  then pause,end
if regexp('ac'+lf+'b'+lf                            ,'/abb$/m'                               ) <>  []  then pause,end
if regexp('b'+lf+'ac'+lf                            ,'/abb\Z/m'                              ) <>  []  then pause,end
if regexp('b'+lf+'ac'+lf                            ,'/abb\z/m'                              ) <>  []  then pause,end
if regexp('b'+lf+'ac'+lf                            ,'/abb$/m'                               ) <>  []  then pause,end
if regexp('b'+lf+'ac'                               ,'/abb\Z/m'                              ) <>  []  then pause,end
if regexp('b'+lf+'ac'                               ,'/abb\z/m'                              ) <>  []  then pause,end
if regexp('b'+lf+'ac'                               ,'/abb$/m'                               ) <>  []  then pause,end
if regexp('ca'+lf+'b'+lf                            ,'/abb\Z/'                               ) <>  []  then pause,end
if regexp('ca'+lf+'b'+lf                            ,'/abb\z/'                               ) <>  []  then pause,end
if regexp('ca'+lf+'b'+lf                            ,'/abb$/'                                ) <>  []  then pause,end
if regexp('b'+lf+'ca'+lf                            ,'/abb\Z/'                               ) <>  []  then pause,end
if regexp('b'+lf+'ca'+lf                            ,'/abb\z/'                               ) <>  []  then pause,end
if regexp('b'+lf+'ca'+lf                            ,'/abb$/'                                ) <>  []  then pause,end
if regexp('b'+lf+'ca'                               ,'/abb\Z/'                               ) <>  []  then pause,end
if regexp('b'+lf+'ca'                               ,'/abb\z/'                               ) <>  []  then pause,end
if regexp('b'+lf+'ca'                               ,'/abb$/'                                ) <>  []  then pause,end
if regexp('ca'+lf+'b'+lf                            ,'/abb\Z/m'                              ) <>  []  then pause,end
if regexp('ca'+lf+'b'+lf                            ,'/abb\z/m'                              ) <>  []  then pause,end
if regexp('ca'+lf+'b'+lf                            ,'/abb$/m'                               ) <>  []  then pause,end
if regexp('b'+lf+'ca'+lf                            ,'/abb\Z/m'                              ) <>  []  then pause,end
if regexp('b'+lf+'ca'+lf                            ,'/abb\z/m'                              ) <>  []  then pause,end
if regexp('b'+lf+'ca'+lf                            ,'/abb$/m'                               ) <>  []  then pause,end
if regexp('b'+lf+'ca'                               ,'/abb\Z/m'                              ) <>  []  then pause,end
if regexp('b'+lf+'ca'                               ,'/abb\z/m'                              ) <>  []  then pause,end
if regexp('b'+lf+'ca'                               ,'/abb$/m'                               ) <>  []  then pause,end
if regexp('x'                                       ,'/a*abc?xyz+pqr{3}ab{2,}xy{4,5}pq{0,6}AB{0,}zz/') <>  []  then pause,end
if regexp('foo.bart'                                ,'/foo.bart/'                            ) <>  1   then pause,end
if regexp('abcd'+lf+'dxxx'                          ,'/^d[x][x][x]/m'                        ) <>  6   then pause,end
if regexp('xxxtt'                                   ,'/tt+$/'                                ) <>  4   then pause,end
if regexp('aaaXbX'                                  ,'/\GX.*X/'                              ) <>  []  then pause,end
if regexp('Changes'                                 ,'/\.c(pp|xx|c)?$/i'                     ) <>  []  then pause,end
if regexp('IO.c'                                    ,'/\.c(pp|xx|c)?$/i'                     ) <>  3   then pause,end
if regexp('C:/'                                     ,'/^([a-z]:)/'                           ) <>  []  then pause,end
if regexp(lf+'x aa'                                 ,'/^\S\s+aa$/m'                          ) <>  2   then pause,end
[start_pos, end_pos, match_str] = regexp(lf+'x aa'                                 ,'/^\S\s+aa$/m'                          );
if (start_pos <> 2) then pause,end
if (end_pos <> 5) then pause,end
if (match_str <> 'x aa') then pause,end
if regexp('ab'                                      ,'/(^|a)b/'                              ) <>  1   then pause,end
if regexp('abcab'                                   ,'/(\w)?(abc)\1b/'                       ) <>  []  then pause,end
if regexp('a,b,c'                                   ,'/^(?:.,){2}c/'                         ) <>  1   then pause,end
if regexp('a,b,c'                                   ,'/^(?:[^,]*,){2}c/'                     ) <>  1   then pause,end
if regexp(''                                        ,'/(?i)/'                                ) <>  []  then pause,end
if regexp('a'+lf+'xb'+lf                            ,'/(?!\A)x/m'                            ) <>  3   then pause,end
if regexp('123'+lf+'abcabcabcabc'+lf                ,'/^.{9}abc.*\n/m'                       ) <>  5   then pause,end
if regexp('a'                                       ,'/^(a)?(?(1)a|b)+$/'                    ) <>  []  then pause,end
if regexp('x1'                                      ,'/^(0+)?(?:x(1))?/'                     ) <>  1   then pause,end
if regexp('012cxx0190'                              ,'/^([0-9a-fA-F]+)(?:x([0-9a-fA-F]+)?)(?:x([0-9a-fA-F]+))?/') <>  1   then pause,end
if regexp('aaaacccc'                                ,'/((?:aaaa|bbbb)cccc)?/'                ) <>  1   then pause,end
if regexp('bbbbcccc'                                ,'/((?:aaaa|bbbb)cccc)?/'                ) <>  1   then pause,end
if regexp('a'+lf+'b'+lf                             ,'/b\s^/m'                               ) <>  []  then pause,end
if regexp('a'                                       ,'/\ba/'                                 ) <>  1   then pause,end
if regexp('AbCd'                                    ,'/ab(?i)cd/'                            ) <>  []  then pause,end
if regexp('abCd'                                    ,'/ab(?i)cd/'                            ) <>  1   then pause,end
if regexp('Oo'                                      ,'/^(o)(?!.*\1)/i'                       ) <>  []  then pause,end
if regexp('2'                                       ,'/2(]*)?$\1/'                           ) <>  1   then pause,end
if regexp('......abef'                              ,'/.*a(?!(b|cd)*e).*f/'                  ) <>  []  then pause,end
if regexp('fools'                                   ,'/(foo|fool|x.|money|parted)$/'         ) <>  []  then pause,end
if regexp('fools'                                   ,'/(x.|foo|fool|x.|money|parted|y.)$/'   ) <>  []  then pause,end
if regexp('fools'                                   ,'/(foo|fool|money|parted)$/'            ) <>  []  then pause,end
//===================================================================
if regexp('scilab-5.0'            ,'/^scilab-[5-9].[0-9](.[0-9])?(-(alpha-|beta-|rc)([0-9])?)?$/') <> 1  then pause,end
if regexp('scilab-5.0.1'          ,'/^scilab-[5-9].[0-9](.[0-9])?(-(alpha-|beta-|rc)([0-9])?)?$/') <> 1  then pause,end
if regexp('scilab-5.0-alpha-1'    ,'/^scilab-[5-9].[0-9](.[0-9])?(-(alpha-|beta-|rc)([0-9])?)?$/') <> 1  then pause,end
if regexp('scilab-5.0-alpha1'     ,'/^scilab-[5-9].[0-9](.[0-9])?(-(alpha-|beta-|rc)([0-9])?)?$/') <> [] then pause,end
if regexp('scilab-5.0-rc1'        ,'/^scilab-[5-9].[0-9](.[0-9])?(-(alpha-|beta-|rc)([0-9])?)?$/') <> 1  then pause,end
if regexp('scilab-5.0-rc-1'       ,'/^scilab-[5-9].[0-9](.[0-9])?(-(alpha-|beta-|rc)([0-9])?)?$/') <> [] then pause,end
if regexp('scilab-SE-trunk-27490' ,'/^scilab-[5-9].[0-9](.[0-9])?(-(alpha-|beta-|rc)([0-9])?)?$/') <> [] then pause,end
//===================================================================
// Chinese
str = '世界您好';
[s,e,m] = regexp(str,'/您好$/');
if part(str,s:e) <> m then pause,end

[s,e,m] = regexp(str,'/^世界/');
if part(str,s:e) <> m then pause,end

[s,e,m] = regexp(str,'/世界$/');
if s <> [] then pause,end
if e <> [] then pause,end
if m <> '' then pause,end

[s,e,m] = regexp(str,'/您好$/');
if part(str,s:e) <> m then pause,end

[s,e,m] = regexp(str,'/^您好/');
if s <> [] then pause,end
if e <> [] then pause,end
if m <> '' then pause,end

[s,e,m] = regexp(str,'/界您/');
if part(str,s:e) <> m then pause,end

[s,e,m] = regexp(str,'/界_您/');
if s <> [] then pause,end
if e <> [] then pause,end
if m <> '' then pause,end
//===================================================================
// Russian
str = 'привет мир';
[s,e,m] = regexp(str,'/^привет/');
if part(str,s:e) <> m then pause,end

[s,e,m] = regexp(str,'/привет$/');
if s <> [] then pause,end
if e <> [] then pause,end
if m <> '' then pause,end

[s,e,m] = regexp(str,'/мир$/');
if part(str,s:e) <> m then pause,end

[s,e,m] = regexp(str,'/^мир/');
if s <> [] then pause,end
if e <> [] then pause,end
if m <> '' then pause,end

[s,e,m] = regexp(str,'/вет\sм/');
if part(str,s:e) <> m then pause,end

[s,e,m] = regexp(str,'/вет_м/');
if s <> [] then pause,end
if e <> [] then pause,end
if m <> '' then pause,end
//===================================================================
// Cyrilic
str = 'АБВГДЄЖЅЗИІКЛМНОПҀРСТѸФХѠЦЧШЩЪЪІЬѢЮѦѨѪѬѮѰѲѴѤ';

[s,e,m] = regexp(str,'/^АБВГДЄЖЅЗИІКЛМНОПҀР/');
if part(str,s:e) <> m then pause,end

[s,e,m] = regexp(str,'/АБВГДЄЖЅЗИІКЛМНОПҀР$/');
if s <> [] then pause,end
if e <> [] then pause,end
if m <> '' then pause,end

[s,e,m] = regexp(str,'/ЧШЩЪЪІЬѢЮѦѨѪѬѮѰѲѴѤ$/');
if part(str,s:e) <> m then pause,end

[s,e,m] = regexp(str,'/^ЧШЩЪЪІЬѢЮѦѨѪѬѮѰѲѴѤ/');
if s <> [] then pause,end
if e <> [] then pause,end
if m <> '' then pause,end

[s,e,m] = regexp(str,'/ИІКЛМНОПҀРСТѸФХѠЦЧШЩЪ/');
if part(str,s:e) <> m then pause,end

[s,e,m] = regexp(str,'/ИІКЛМНОПҀ_РСТѸФХѠЦЧШЩЪ/');
if s <> [] then pause,end
if e <> [] then pause,end
if m <> '' then pause,end
//===================================================================
// Japanese
str = '丑丞串乃之乎也云亘亙些亦亥亨亮仔伊伎伍伽佃佑伶侃侑俄侠俣俐侶倭俺倶倦倖偲僅傭儲允兎兜其冥冴冶凄凌凜凛凧凪凰凱函刹劉劫勁勃';

[s,e,m] = regexp(str,'/^丑丞串乃之乎也云亘亙些亦/');
if part(str,s:e) <> m then pause,end

[s,e,m] = regexp(str,'/丑丞串乃之乎也云亘亙些亦$/');
if s <> [] then pause,end
if e <> [] then pause,end
if m <> '' then pause,end

[s,e,m] = regexp(str,'/凰凱函刹劉劫勁勃$/');
if part(str,s:e) <> m then pause,end

[s,e,m] = regexp(str,'/^凰凱函刹劉劫勁勃/');
if s <> [] then pause,end
if e <> [] then pause,end
if m <> '' then pause,end

[s,e,m] = regexp(str,'/亨亮仔伊伎伍伽佃佑伶侃/');
if part(str,s:e) <> m then pause,end

[s,e,m] = regexp(str,'/亨亮仔伊_伎伍伽佃佑伶侃/');
if s <> [] then pause,end
if e <> [] then pause,end
if m <> '' then pause,end
//===================================================================
// Thaï
str = 'มกระดุกกระดิก';
[s,e,m] = regexp(str,'/^มกระดุกกร/');
if part(str,s:e) <> m then pause,end

[s,e,m] = regexp(str,'/มกระดุกกร$/');
if s <> [] then pause,end
if e <> [] then pause,end
if m <> '' then pause,end

[s,e,m] = regexp(str,'/กกระดิก$/');
if part(str,s:e) <> m then pause,end

[s,e,m] = regexp(str,'/^กกระดิก/');
if s <> [] then pause,end
if e <> [] then pause,end
if m <> '' then pause,end

[s,e,m] = regexp(str,'/ดุกก/');
if part(str,s:e) <> m then pause,end

[s,e,m] = regexp(str,'/ดุก_ก/');
if s <> [] then pause,end
if e <> [] then pause,end
if m <> '' then pause,end
//===================================================================

// Subpatterns

piString="3.14";
[a,b,c,piStringSplit]=regexp(piString,"/(\d+)\.(\d+)/");
assert_checkequal(piStringSplit(1),"3");
assert_checkequal(piStringSplit(2),"14");

[a,b,c,d]=regexp('xabyabbbz','/ab(.*)b(.*)/');
assert_checkequal(size(d), [1, 2]);



// get host name from URL
myURL="http://www.scilab.org/download/";
[a,b,c,d]=regexp(myURL,'@^(?:http://)?([^/]+)@i');
assert_checkequal(d,"www.scilab.org");

str='foobar: 2012';
// Using named subpatterns
[a,b,c,d]=regexp(str,'/(?P<name>\w+): (?P<digit>\d+)/');
assert_checkequal(d(1),"foobar");
assert_checkequal(d(2),"2012");

