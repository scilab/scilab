// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2007-2008 - INRIA
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

lf = ascii(10);

//===============================
// unit tests grep
//===============================
txt=["find matches of a string in a vector of strings"
"search position of a character string in an other string"
"Compare Strings"];
//===============================
if grep(txt,"vide")<>[] then pause,end
if grep([],"vide")<>[] then pause,end
if grep("","vide")<>[] then pause,end
e="";if execstr("grep(txt,e)","errcatch")==0 then pause,end
//===============================
if grep(txt,"strings")<>1 then pause,end
if or(grep(txt,["strings" "Strings"])<>[1 3]) then pause,end
//===============================
[r,w]=grep(txt,["strings" "Strings"]);
if or(w<>[1 2])|or(r<>[1 3]) then pause,end
//===============================
e=["strings" ""]; if execstr("[r,w]=grep(txt,e)","errcatch")==0 then pause,end;
//===============================
if grep([]," ") <> [] then pause,end
//===============================
if grep("AB",["A","B"]) <> 1 then pause,end
//===============================
if grep("AB",["A";"B"]) <> 1 then pause,end
//===============================
if grep(["AB","B"],["A";"B"]) <> 1 then pause,end
//===============================
if grep(["AB","B"],["A";"B";"C"]) <> 1 then pause,end
//===============================
fd = mopen(SCI+"/modules/string/tests/unit_tests/text.txt","r");
txt = mgetl( fd );
mclose( fd );
//===============================
if grep(txt,"scilab") <> [] then pause,end
//===============================
ref = [ 1., 3., 5., 32., 48., 83., 110., 114., 162., 175., 177., 272., 365., 390., 402., 404., 408.];
if grep(txt,"Scilab") <> ref then pause,end
//===============================
if grep("abc"                                     ,"/abc/"                                 ,"r") <>  1   then pause,end
if grep("xbc"                                     ,"/abc/"                                 ,"r") <>  []  then pause,end
if grep("axc"                                     ,"/abc/"                                 ,"r") <>  []  then pause,end
if grep("abx"                                     ,"/abc/"                                 ,"r") <>  []  then pause,end
if grep("xabcy"                                   ,"/abc/"                                 ,"r") <>  1   then pause,end
if grep("ababc"                                   ,"/abc/"                                 ,"r") <>  1   then pause,end
if grep("abc"                                     ,"/ab*c/"                                ,"r") <>  1   then pause,end
if grep("abc"                                     ,"/ab*bc/"                               ,"r") <>  1   then pause,end
if grep("abbc"                                    ,"/ab*bc/"                               ,"r") <>  1   then pause,end
if grep("abbbbc"                                  ,"/ab*bc/"                               ,"r") <>  1   then pause,end
if grep("abbbbc"                                  ,"/.{1}/"                                ,"r") <>  1   then pause,end
if grep("abbbbc"                                  ,"/.{3,4}/"                              ,"r") <>  1   then pause,end
if grep("abbbbc"                                  ,"/ab{0,}bc/"                            ,"r") <>  1   then pause,end
if grep("abbc"                                    ,"/ab+bc/"                               ,"r") <>  1   then pause,end
if grep("abc"                                     ,"/ab+bc/"                               ,"r") <>  []  then pause,end
if grep("abq"                                     ,"/ab+bc/"                               ,"r") <>  []  then pause,end
if grep("abq"                                     ,"/ab{1,}bc/"                            ,"r") <>  []  then pause,end
if grep("abbbbc"                                  ,"/ab+bc/"                               ,"r") <>  1   then pause,end
if grep("abbbbc"                                  ,"/ab{1,}bc/"                            ,"r") <>  1   then pause,end
if grep("abbbbc"                                  ,"/ab{1,3}bc/"                           ,"r") <>  1   then pause,end
if grep("abbbbc"                                  ,"/ab{3,4}bc/"                           ,"r") <>  1   then pause,end
if grep("abbbbc"                                  ,"/ab{4,5}bc/"                           ,"r") <>  []  then pause,end
if grep("abbc"                                    ,"/ab?bc/"                               ,"r") <>  1   then pause,end
if grep("abc"                                     ,"/ab?bc/"                               ,"r") <>  1   then pause,end
if grep("abc"                                     ,"/ab{0,1}bc/"                           ,"r") <>  1   then pause,end
if grep("abbbbc"                                  ,"/ab?bc/"                               ,"r") <>  []  then pause,end
if grep("abc"                                     ,"/ab?c/"                                ,"r") <>  1   then pause,end
if grep("abc"                                     ,"/ab{0,1}c/"                            ,"r") <>  1   then pause,end
if grep("abc"                                     ,"/^abc$/"                               ,"r") <>  1   then pause,end
if grep("abcc"                                    ,"/^abc$/"                               ,"r") <>  []  then pause,end
if grep("abcc"                                    ,"/^abc/"                                ,"r") <>  1   then pause,end
if grep("aabc"                                    ,"/^abc$/"                               ,"r") <>  []  then pause,end
if grep("aabc"                                    ,"/abc$/"                                ,"r") <>  1   then pause,end
if grep("aabcd"                                   ,"/abc$/"                                ,"r") <>  []  then pause,end
if grep("abc"                                     ,"/^/"                                   ,"r") <>  1   then pause,end
if grep("abc"                                     ,"/$/"                                   ,"r") <>  1   then pause,end
if grep("abc"                                     ,"/a.c/"                                 ,"r") <>  1   then pause,end
if grep("axc"                                     ,"/a.c/"                                 ,"r") <>  1   then pause,end
if grep("axyzc"                                   ,"/a.*c/"                                ,"r") <>  1   then pause,end
if grep("axyzd"                                   ,"/a.*c/"                                ,"r") <>  []  then pause,end
if grep("abc"                                     ,"/a[bc]d/"                              ,"r") <>  []  then pause,end
if grep("abd"                                     ,"/a[bc]d/"                              ,"r") <>  1   then pause,end
if grep("abd"                                     ,"/a[b-d]e/"                             ,"r") <>  []  then pause,end
if grep("ace"                                     ,"/a[b-d]e/"                             ,"r") <>  1   then pause,end
if grep("aac"                                     ,"/a[b-d]/"                              ,"r") <>  1   then pause,end
if grep("a-"                                      ,"/a[-b]/"                               ,"r") <>  1   then pause,end
if grep("a-"                                      ,"/a[b-]/"                               ,"r") <>  1   then pause,end
if grep("a]"                                      ,"/a]/"                                  ,"r") <>  1   then pause,end
if grep("a]b"                                     ,"/a[]]b/"                               ,"r") <>  1   then pause,end
if grep("aed"                                     ,"/a[^bc]d/"                             ,"r") <>  1   then pause,end
if grep("abd"                                     ,"/a[^bc]d/"                             ,"r") <>  []  then pause,end
if grep("adc"                                     ,"/a[^-b]c/"                             ,"r") <>  1   then pause,end
if grep("a-c"                                     ,"/a[^-b]c/"                             ,"r") <>  []  then pause,end
if grep("a]c"                                     ,"/a[^]b]c/"                             ,"r") <>  []  then pause,end
if grep("adc"                                     ,"/a[^]b]c/"                             ,"r") <>  1   then pause,end
if grep("a-"                                      ,"/\ba\b/"                               ,"r") <>  1   then pause,end
if grep("-a"                                      ,"/\ba\b/"                               ,"r") <>  1   then pause,end
if grep("-a-"                                     ,"/\ba\b/"                               ,"r") <>  1   then pause,end
if grep("xy"                                      ,"/\by\b/"                               ,"r") <>  []  then pause,end
if grep("yz"                                      ,"/\by\b/"                               ,"r") <>  []  then pause,end
if grep("xyz"                                     ,"/\by\b/"                               ,"r") <>  []  then pause,end
if grep("a-"                                      ,"/\Ba\B/"                               ,"r") <>  []  then pause,end
if grep("-a"                                      ,"/\Ba\B/"                               ,"r") <>  []  then pause,end
if grep("-a-"                                     ,"/\Ba\B/"                               ,"r") <>  []  then pause,end
if grep("xy"                                      ,"/\By\b/"                               ,"r") <>  1   then pause,end
if grep("xy"                                      ,"/\By\b/"                               ,"r") <>  1   then pause,end
if grep("yz"                                      ,"/\by\B/"                               ,"r") <>  1   then pause,end
if grep("xyz"                                     ,"/\By\B/"                               ,"r") <>  1   then pause,end
if grep("a"                                       ,"/\w/"                                  ,"r") <>  1   then pause,end
if grep("-"                                       ,"/\w/"                                  ,"r") <>  []  then pause,end
if grep("a"                                       ,"/\W/"                                  ,"r") <>  []  then pause,end
if grep("-"                                       ,"/\W/"                                  ,"r") <>  1   then pause,end
if grep("a b"                                     ,"/a\sb/"                                ,"r") <>  1   then pause,end
if grep("a-b"                                     ,"/a\sb/"                                ,"r") <>  []  then pause,end
if grep("a b"                                     ,"/a\Sb/"                                ,"r") <>  []  then pause,end
if grep("a-b"                                     ,"/a\Sb/"                                ,"r") <>  1   then pause,end
if grep("1"                                       ,"/\d/"                                  ,"r") <>  1   then pause,end
if grep("-"                                       ,"/\d/"                                  ,"r") <>  []  then pause,end
if grep("1"                                       ,"/\D/"                                  ,"r") <>  []  then pause,end
if grep("-"                                       ,"/\D/"                                  ,"r") <>  1   then pause,end
if grep("a"                                       ,"/[\w]/"                                ,"r") <>  1   then pause,end
if grep("-"                                       ,"/[\w]/"                                ,"r") <>  []  then pause,end
if grep("a"                                       ,"/[\W]/"                                ,"r") <>  []  then pause,end
if grep("-"                                       ,"/[\W]/"                                ,"r") <>  1   then pause,end
if grep("a b"                                     ,"/a[\s]b/"                              ,"r") <>  1   then pause,end
if grep("a-b"                                     ,"/a[\s]b/"                              ,"r") <>  []  then pause,end
if grep("a b"                                     ,"/a[\S]b/"                              ,"r") <>  []  then pause,end
if grep("a-b"                                     ,"/a[\S]b/"                              ,"r") <>  1   then pause,end
if grep("1"                                       ,"/[\d]/"                                ,"r") <>  1   then pause,end
if grep("-"                                       ,"/[\d]/"                                ,"r") <>  []  then pause,end
if grep("1"                                       ,"/[\D]/"                                ,"r") <>  []  then pause,end
if grep("-"                                       ,"/[\D]/"                                ,"r") <>  1   then pause,end
if grep("abc"                                     ,"/ab|cd/"                               ,"r") <>  1   then pause,end
if grep("abcd"                                    ,"/ab|cd/"                               ,"r") <>  1   then pause,end
if grep("b"                                       ,"/$b/"                                  ,"r") <>  []  then pause,end
if grep("ab"                                      ,"/a\(*b/"                               ,"r") <>  1   then pause,end
if grep("a((b"                                    ,"/a\(*b/"                               ,"r") <>  1   then pause,end
if grep("aabbabc"                                 ,"/a+b+c/"                               ,"r") <>  1   then pause,end
if grep("aabbabc"                                 ,"/a{1,}b{1,}c/"                         ,"r") <>  1   then pause,end
if grep("abcabc"                                  ,"/a.+?c/"                               ,"r") <>  1   then pause,end
if grep("cde"                                     ,"/[^ab]*/"                              ,"r") <>  1   then pause,end
if grep(""                                        ,"/abc/"                                 ,"r") <>  []  then pause,end
if grep(""                                        ,"/a+/"                                  ,"r") <>  []  then pause,end
if grep("e"                                       ,"/a|b|c|d|e/"                           ,"r") <>  1   then pause,end
if grep("abcdefg"                                 ,"/abcd*efg/"                            ,"r") <>  1   then pause,end
if grep("xabyabbbz"                               ,"/ab*/"                                 ,"r") <>  1   then pause,end
if grep("xayabbbz"                                ,"/ab*/"                                 ,"r") <>  1   then pause,end
if grep("hij"                                     ,"/[abhgefdc]ij/"                        ,"r") <>  1   then pause,end
if grep("abcde"                                   ,"/^(ab|cd)e/"                           ,"r") <>  []  then pause,end
if grep("adcdcde"                                 ,"/a[bcd]*dcdcde/"                       ,"r") <>  1   then pause,end
if grep("adcdcde"                                 ,"/a[bcd]+dcdcde/"                       ,"r") <>  []  then pause,end
if grep("alpha"                                   ,"/[a-zA-Z_][a-zA-Z0-9_]*/"              ,"r") <>  1   then pause,end
if grep("effg"                                    ,"/(bc+d$|ef*g.|h?i(j|k))/"              ,"r") <>  []  then pause,end
if grep("bcdd"                                    ,"/(bc+d$|ef*g.|h?i(j|k))/"              ,"r") <>  []  then pause,end
if grep("aa"                                      ,"/((((((((((a))))))))))\10/"            ,"r") <>  1   then pause,end
if grep("aa"                                      ,"/((((((((((a))))))))))\041/"           ,"r") <>  []  then pause,end
if grep("a!"                                      ,"/((((((((((a))))))))))\041/"           ,"r") <>  1   then pause,end
if grep("a"                                       ,"/(((((((((a)))))))))/"                 ,"r") <>  1   then pause,end
if grep("uh-uh"                                   ,"/multiple words of text/"              ,"r") <>  []  then pause,end
if grep("multiple words, yeah"                    ,"/multiple words/"                      ,"r") <>  1   then pause,end
if grep("ab"                                      ,"/[k]/"                                 ,"r") <>  []  then pause,end
if grep("ac"                                      ,"/a[-]?c/"                              ,"r") <>  1   then pause,end
if grep("a"                                       ,"/(a)|\1/"                              ,"r") <>  1   then pause,end
if grep("x"                                       ,"/(a)|\1/"                              ,"r") <>  []  then pause,end
if grep("aaxabxbaxbbx"                            ,"/((\3|b)\2(a)x)+/"                     ,"r") <>  []  then pause,end
if grep("ABC"                                     ,"/abc/i"                                ,"r") <>  1   then pause,end
if grep("XBC"                                     ,"/abc/i"                                ,"r") <>  []  then pause,end
if grep("AXC"                                     ,"/abc/i"                                ,"r") <>  []  then pause,end
if grep("ABX"                                     ,"/abc/i"                                ,"r") <>  []  then pause,end
if grep("XABCY"                                   ,"/abc/i"                                ,"r") <>  1   then pause,end
if grep("ABABC"                                   ,"/abc/i"                                ,"r") <>  1   then pause,end
if grep("ABC"                                     ,"/ab*c/i"                               ,"r") <>  1   then pause,end
if grep("ABC"                                     ,"/ab*bc/i"                              ,"r") <>  1   then pause,end
if grep("ABBC"                                    ,"/ab*bc/i"                              ,"r") <>  1   then pause,end
if grep("ABBBBC"                                  ,"/ab*?bc/i"                             ,"r") <>  1   then pause,end
if grep("ABBBBC"                                  ,"/ab{0,}?bc/i"                          ,"r") <>  1   then pause,end
if grep("ABBC"                                    ,"/ab+?bc/i"                             ,"r") <>  1   then pause,end
if grep("ABC"                                     ,"/ab+bc/i"                              ,"r") <>  []  then pause,end
if grep("ABQ"                                     ,"/ab+bc/i"                              ,"r") <>  []  then pause,end
if grep("ABQ"                                     ,"/ab{1,}bc/i"                           ,"r") <>  []  then pause,end
if grep("ABBBBC"                                  ,"/ab+bc/i"                              ,"r") <>  1   then pause,end
if grep("ABBBBC"                                  ,"/ab{1,}?bc/i"                          ,"r") <>  1   then pause,end
if grep("ABBBBC"                                  ,"/ab{1,3}?bc/i"                         ,"r") <>  1   then pause,end
if grep("ABBBBC"                                  ,"/ab{3,4}?bc/i"                         ,"r") <>  1   then pause,end
if grep("ABBBBC"                                  ,"/ab{4,5}?bc/i"                         ,"r") <>  []  then pause,end
if grep("ABBC"                                    ,"/ab??bc/i"                             ,"r") <>  1   then pause,end
if grep("ABC"                                     ,"/ab??bc/i"                             ,"r") <>  1   then pause,end
if grep("ABC"                                     ,"/ab{0,1}?bc/i"                         ,"r") <>  1   then pause,end
if grep("ABBBBC"                                  ,"/ab??bc/i"                             ,"r") <>  []  then pause,end
if grep("ABC"                                     ,"/ab??c/i"                              ,"r") <>  1   then pause,end
if grep("ABC"                                     ,"/ab{0,1}?c/i"                          ,"r") <>  1   then pause,end
if grep("ABC"                                     ,"/^abc$/i"                              ,"r") <>  1   then pause,end
if grep("ABCC"                                    ,"/^abc$/i"                              ,"r") <>  []  then pause,end
if grep("ABCC"                                    ,"/^abc/i"                               ,"r") <>  1   then pause,end
if grep("AABC"                                    ,"/^abc$/i"                              ,"r") <>  []  then pause,end
if grep("AABC"                                    ,"/abc$/i"                               ,"r") <>  1   then pause,end
if grep("ABC"                                     ,"/^/i"                                  ,"r") <>  1   then pause,end
if grep("ABC"                                     ,"/$/i"                                  ,"r") <>  1   then pause,end
if grep("ABC"                                     ,"/a.c/i"                                ,"r") <>  1   then pause,end
if grep("AXC"                                     ,"/a.c/i"                                ,"r") <>  1   then pause,end
if grep("AXYZC"                                   ,"/a.*?c/i"                              ,"r") <>  1   then pause,end
if grep("AXYZD"                                   ,"/a.*c/i"                               ,"r") <>  []  then pause,end
if grep("ABC"                                     ,"/a[bc]d/i"                             ,"r") <>  []  then pause,end
if grep("ABD"                                     ,"/a[bc]d/i"                             ,"r") <>  1   then pause,end
if grep("ABD"                                     ,"/a[b-d]e/i"                            ,"r") <>  []  then pause,end
if grep("ACE"                                     ,"/a[b-d]e/i"                            ,"r") <>  1   then pause,end
if grep("AAC"                                     ,"/a[b-d]/i"                             ,"r") <>  1   then pause,end
if grep("A-"                                      ,"/a[-b]/i"                              ,"r") <>  1   then pause,end
if grep("A-"                                      ,"/a[b-]/i"                              ,"r") <>  1   then pause,end
if grep("A]"                                      ,"/a]/i"                                 ,"r") <>  1   then pause,end
if grep("A]B"                                     ,"/a[]]b/i"                              ,"r") <>  1   then pause,end
if grep("AED"                                     ,"/a[^bc]d/i"                            ,"r") <>  1   then pause,end
if grep("ABD"                                     ,"/a[^bc]d/i"                            ,"r") <>  []  then pause,end
if grep("ADC"                                     ,"/a[^-b]c/i"                            ,"r") <>  1   then pause,end
if grep("A-C"                                     ,"/a[^-b]c/i"                            ,"r") <>  []  then pause,end
if grep("A]C"                                     ,"/a[^]b]c/i"                            ,"r") <>  []  then pause,end
if grep("ADC"                                     ,"/a[^]b]c/i"                            ,"r") <>  1   then pause,end
if grep("ABC"                                     ,"/ab|cd/i"                              ,"r") <>  1   then pause,end
if grep("ABCD"                                    ,"/ab|cd/i"                              ,"r") <>  1   then pause,end
if grep("B"                                       ,"/$b/i"                                 ,"r") <>  []  then pause,end
if grep("AB"                                      ,"/a\(*b/i"                              ,"r") <>  1   then pause,end
if grep("A((B"                                    ,"/a\(*b/i"                              ,"r") <>  1   then pause,end
if grep("AABBABC"                                 ,"/a+b+c/i"                              ,"r") <>  1   then pause,end
if grep("AABBABC"                                 ,"/a{1,}b{1,}c/i"                        ,"r") <>  1   then pause,end
if grep("ABCABC"                                  ,"/a.+?c/i"                              ,"r") <>  1   then pause,end
if grep("ABCABC"                                  ,"/a.*?c/i"                              ,"r") <>  1   then pause,end
if grep("ABCABC"                                  ,"/a.{0,5}?c/i"                          ,"r") <>  1   then pause,end
if grep("CDE"                                     ,"/[^ab]*/i"                             ,"r") <>  1   then pause,end
if grep(""                                        ,"/abc/i"                                ,"r") <>  []  then pause,end
if grep(""                                        ,"/a+/i"                                 ,"r") <>  []  then pause,end
if grep("E"                                       ,"/a|b|c|d|e/i"                          ,"r") <>  1   then pause,end
if grep("ABCDEFG"                                 ,"/abcd*efg/i"                           ,"r") <>  1   then pause,end
if grep("XABYABBBZ"                               ,"/ab*/i"                                ,"r") <>  1   then pause,end
if grep("XAYABBBZ"                                ,"/ab*/i"                                ,"r") <>  1   then pause,end
if grep("HIJ"                                     ,"/[abhgefdc]ij/i"                       ,"r") <>  1   then pause,end
if grep("ABCDE"                                   ,"/^(ab|cd)e/i"                          ,"r") <>  []  then pause,end
if grep("ADCDCDE"                                 ,"/a[bcd]*dcdcde/i"                      ,"r") <>  1   then pause,end
if grep("ADCDCDE"                                 ,"/a[bcd]+dcdcde/i"                      ,"r") <>  []  then pause,end
if grep("ALPHA"                                   ,"/[a-zA-Z_][a-zA-Z0-9_]*/i"             ,"r") <>  1   then pause,end
if grep("EFFG"                                    ,"/(bc+d$|ef*g.|h?i(j|k))/i"             ,"r") <>  []  then pause,end
if grep("BCDD"                                    ,"/(bc+d$|ef*g.|h?i(j|k))/i"             ,"r") <>  []  then pause,end
if grep("AA"                                      ,"/((((((((((a))))))))))\10/i"           ,"r") <>  1   then pause,end
if grep("AA"                                      ,"/((((((((((a))))))))))\041/i"          ,"r") <>  []  then pause,end
if grep("A!"                                      ,"/((((((((((a))))))))))\041/i"          ,"r") <>  1   then pause,end
if grep("A"                                       ,"/(((((((((a)))))))))/i"                ,"r") <>  1   then pause,end
if grep("UH-UH"                                   ,"/multiple words of text/i"             ,"r") <>  []  then pause,end
if grep("MULTIPLE WORDS, YEAH"                    ,"/multiple words/i"                     ,"r") <>  1   then pause,end
if grep("AB"                                      ,"/[k]/i"                                ,"r") <>  []  then pause,end
if grep("AC"                                      ,"/a[-]?c/i"                             ,"r") <>  1   then pause,end
if grep("abad"                                    ,"/a(?!b)./"                             ,"r") <>  1   then pause,end
if grep("abad"                                    ,"/a(?=d)./"                             ,"r") <>  1   then pause,end
if grep("abad"                                    ,"/a(?=c|d)./"                           ,"r") <>  1   then pause,end
if grep("<&OUT"                                   ,"/^[<>]&/"                              ,"r") <>  1   then pause,end
if grep("aaaaaaaaa"                               ,"/^(a\1?){4}$/"                         ,"r") <>  []  then pause,end
if grep("aaaaaaaaaaa"                             ,"/^(a\1?){4}$/"                         ,"r") <>  []  then pause,end
if grep("aaaaaaaaa"                               ,"/^(a(?(1)\1)){4}$/"                    ,"r") <>  []  then pause,end
if grep("aaaaaaaaaaa"                             ,"/^(a(?(1)\1)){4}$/"                    ,"r") <>  []  then pause,end
if grep("ab"                                      ,"/(?<=a)b/"                             ,"r") <>  1   then pause,end
if grep("cb"                                      ,"/(?<=a)b/"                             ,"r") <>  []  then pause,end
if grep("b"                                       ,"/(?<=a)b/"                             ,"r") <>  []  then pause,end
if grep("ab"                                      ,"/(?<!c)b/"                             ,"r") <>  1   then pause,end
if grep("cb"                                      ,"/(?<!c)b/"                             ,"r") <>  []  then pause,end
if grep("b"                                       ,"/(?<!c)b/"                             ,"r") <>  1   then pause,end
if grep("b"                                       ,"/(?<!c)b/"                             ,"r") <>  1   then pause,end
if grep("aba"                                     ,"/(?:..)*a/"                            ,"r") <>  1   then pause,end
if grep("aba"                                     ,"/(?:..)*?a/"                           ,"r") <>  1   then pause,end
if grep("abc"                                     ,"/^(?:b|a(?=(.)))*\1/"                  ,"r") <>  1   then pause,end
if grep("ab"                                      ,"/(?:(?i)a)b/"                          ,"r") <>  1   then pause,end
if grep("Ab"                                      ,"/(?:(?i)a)b/"                          ,"r") <>  1   then pause,end
if grep("aB"                                      ,"/(?:(?i)a)b/"                          ,"r") <>  []  then pause,end
if grep("aB"                                      ,"/((?i)a)b/"                            ,"r") <>  []  then pause,end
if grep("ab"                                      ,"/(?i:a)b/"                             ,"r") <>  1   then pause,end
if grep("Ab"                                      ,"/(?i:a)b/"                             ,"r") <>  1   then pause,end
if grep("aB"                                      ,"/(?i:a)b/"                             ,"r") <>  []  then pause,end
if grep("aB"                                      ,"/((?i:a))b/"                           ,"r") <>  []  then pause,end
if grep("ab"                                      ,"/(?:(?-i)a)b/i"                        ,"r") <>  1   then pause,end
if grep("aB"                                      ,"/(?:(?-i)a)b/i"                        ,"r") <>  1   then pause,end
if grep("Ab"                                      ,"/(?:(?-i)a)b/i"                        ,"r") <>  []  then pause,end
if grep("Ab"                                      ,"/((?-i)a)b/i"                          ,"r") <>  []  then pause,end
if grep("aB"                                      ,"/(?:(?-i)a)b/i"                        ,"r") <>  1   then pause,end
if grep("AB"                                      ,"/(?:(?-i)a)b/i"                        ,"r") <>  []  then pause,end
if grep("AB"                                      ,"/((?-i)a)b/i"                          ,"r") <>  []  then pause,end
if grep("ab"                                      ,"/(?-i:a)b/i"                           ,"r") <>  1   then pause,end
if grep("aB"                                      ,"/(?-i:a)b/i"                           ,"r") <>  1   then pause,end
if grep("Ab"                                      ,"/(?-i:a)b/i"                           ,"r") <>  []  then pause,end
if grep("Ab"                                      ,"/((?-i:a))b/i"                         ,"r") <>  []  then pause,end
if grep("aB"                                      ,"/(?-i:a)b/i"                           ,"r") <>  1   then pause,end
if grep("AB"                                      ,"/(?-i:a)b/i"                           ,"r") <>  []  then pause,end
if grep("AB"                                      ,"/((?-i:a))b/i"                         ,"r") <>  []  then pause,end
if grep("a"+lf+"B"                                ,"/((?-i:a.))b/i"                        ,"r") <>  []  then pause,end
if grep("B"+lf+"B"                                ,"/((?s-i:a.))b/i"                       ,"r") <>  []  then pause,end
if grep("cabbbb"                                  ,"/(?:c|d)(?:)(?:a(?:)(?:b)(?:b(?:))(?:b(?:)(?:b)))/","r") <>  1   then pause,end
if grep("caaaaaaaabbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb","/(?:c|d)(?:)(?:aaaaaaaa(?:)(?:bbbbbbbb)(?:bbbbbbbb(?:))(?:bbbbbbbb(?:)(?:bbbbbbbb)))/","r") <>  1   then pause,end
if grep("foobar1234baz"                           ,"/foo\w*\d{4}baz/"                      ,"r") <>  1   then pause,end
if grep("x~~"                                     ,"/x(~~)*(?:(?:F)?)?/"                   ,"r") <>  1   then pause,end
if grep("aaac"                                    ,"/^a(?#xxx){3}c/"                       ,"r") <>  1   then pause,end
if grep("aaac"                                    ,"/^a (?#xxx) (?#yyy) {3}c/x"            ,"r") <>  1   then pause,end
if grep("dbcb"                                    ,"/(?<![cd])b/"                          ,"r") <>  []  then pause,end
if grep("dbaacb"                                  ,"/(?<![cd])[ab]/"                       ,"r") <>  1   then pause,end
if grep("dbcb"                                    ,"/(?<!(c|d))b/"                         ,"r") <>  []  then pause,end
if grep("dbaacb"                                  ,"/(?<!(c|d))[ab]/"                      ,"r") <>  1   then pause,end
if grep("cdaccb"                                  ,"/(?<!cd)[ab]/"                         ,"r") <>  1   then pause,end
if grep("a--"                                     ,"/^(?:a?b?)*$/"                         ,"r") <>  []  then pause,end
if grep("a"+lf+"b"+lf                             ,"/(?m)^b/"                              ,"r") <>  1   then pause,end
if grep("a"+lf+"b"+lf+"c"+lf                      ,"/^b/"                                  ,"r") <>  []  then pause,end
if grep("a"+lf+"b"+lf+"c"+lf                      ,"/()^b/"                                ,"r") <>  []  then pause,end
if grep("a"                                       ,"/(x)?(?(1)a|b)/"                       ,"r") <>  []  then pause,end
if grep("a"                                       ,"/(x)?(?(1)b|a)/"                       ,"r") <>  1   then pause,end
if grep("a"                                       ,"/()?(?(1)b|a)/"                        ,"r") <>  1   then pause,end
if grep("a"                                       ,"/()(?(1)b|a)/"                         ,"r") <>  []  then pause,end
if grep("a"                                       ,"/()?(?(1)a|b)/"                        ,"r") <>  1   then pause,end
if grep("blah)"                                   ,"/^(\()?blah(?(1)(\)))$/"               ,"r") <>  []  then pause,end
if grep("(blah"                                   ,"/^(\()?blah(?(1)(\)))$/"               ,"r") <>  []  then pause,end
if grep("blah)"                                   ,"/^(\(+)?blah(?(1)(\)))$/"              ,"r") <>  []  then pause,end
if grep("(blah"                                   ,"/^(\(+)?blah(?(1)(\)))$/"              ,"r") <>  []  then pause,end
if grep("a"                                       ,"/(?(?!a)a|b)/"                         ,"r") <>  []  then pause,end
if grep("a"                                       ,"/(?(?!a)b|a)/"                         ,"r") <>  1   then pause,end
if grep("a"                                       ,"/(?(?=a)b|a)/"                         ,"r") <>  []  then pause,end
if grep("a"                                       ,"/(?(?=a)a|b)/"                         ,"r") <>  1   then pause,end
if grep("aaab"                                    ,"/^(?=(a+?))\1ab/"                      ,"r") <>  []  then pause,end
if grep("aaab"                                    ,"/^(?=(a+?))\1ab/"                      ,"r") <>  []  then pause,end
if grep("abcd:"                                   ,"/([\w:]+::)?(\w+)$/"                   ,"r") <>  []  then pause,end
if grep("abcd:"                                   ,"/([\w:]+::)?(\w+)$/"                   ,"r") <>  []  then pause,end
if grep("aaab"                                    ,"/(>a+)ab/"                             ,"r") <>  []  then pause,end
if grep("aaab"                                    ,"/(?>a+)b/"                             ,"r") <>  1   then pause,end
if grep("abc"                                     ,"/[a[:]b[:c]/"                          ,"r") <>  1   then pause,end
if grep("abc"                                     ,"/[a[:]b[:c]/"                          ,"r") <>  1   then pause,end
if grep("((abc(ade)ufh()()x"                      ,"/((?>[^()]+)|\([^()]*\))+/"            ,"r") <>  1   then pause,end
if grep("a"+lf+"b"+lf                             ,"/a\Z/"                                 ,"r") <>  []  then pause,end
if grep("a"+lf+"b"+lf                             ,"/a\z/"                                 ,"r") <>  []  then pause,end
if grep("a"+lf+"b"+lf                             ,"/a$/"                                  ,"r") <>  []  then pause,end
if grep("b"+lf+"a"+lf                             ,"/a\z/"                                 ,"r") <>  []  then pause,end
if grep("a"+lf+"b"+lf                             ,"/a\Z/m"                                ,"r") <>  []  then pause,end
if grep("a"+lf+"b"+lf                             ,"/a\z/m"                                ,"r") <>  []  then pause,end
if grep("b"+lf+"a"+lf                             ,"/a\z/m"                                ,"r") <>  []  then pause,end
if grep("aa"+lf+"b"+lf                            ,"/aa\Z/"                                ,"r") <>  []  then pause,end
if grep("aa"+lf+"b"+lf                            ,"/aa\z/"                                ,"r") <>  []  then pause,end
if grep("aa"+lf+"b"+lf                            ,"/aa$/"                                 ,"r") <>  []  then pause,end
if grep("b"+lf+"aa"+lf                            ,"/aa\z/"                                ,"r") <>  []  then pause,end
if grep("aa"+lf+"b"+lf                            ,"/aa\Z/m"                               ,"r") <>  []  then pause,end
if grep("aa"+lf+"b"+lf                            ,"/aa\z/m"                               ,"r") <>  []  then pause,end
if grep("b"+lf+"aa"+lf                            ,"/aa\z/m"                               ,"r") <>  []  then pause,end
if grep("ac"+lf+"b"+lf                            ,"/aa\Z/"                                ,"r") <>  []  then pause,end
if grep("ac"+lf+"b"+lf                            ,"/aa\z/"                                ,"r") <>  []  then pause,end
if grep("ac"+lf+"b"+lf                            ,"/aa$/"                                 ,"r") <>  []  then pause,end
if grep("b"+lf+"ac"+lf                            ,"/aa\Z/"                                ,"r") <>  []  then pause,end
if grep("b"+lf+"ac"+lf                            ,"/aa\z/"                                ,"r") <>  []  then pause,end
if grep("b"+lf+"ac"+lf                            ,"/aa$/"                                 ,"r") <>  []  then pause,end
if grep("b"+lf+"ac"                               ,"/aa\Z/"                                ,"r") <>  []  then pause,end
if grep("b"+lf+"ac"                               ,"/aa\z/"                                ,"r") <>  []  then pause,end
if grep("b"+lf+"ac"                               ,"/aa$/"                                 ,"r") <>  []  then pause,end
if grep("ac"+lf+"b"+lf                            ,"/aa\Z/m"                               ,"r") <>  []  then pause,end
if grep("ac"+lf+"b"+lf                            ,"/aa\z/m"                               ,"r") <>  []  then pause,end
if grep("ac"+lf+"b"+lf                            ,"/aa$/m"                                ,"r") <>  []  then pause,end
if grep("b"+lf+"ac"+lf                            ,"/aa\Z/m"                               ,"r") <>  []  then pause,end
if grep("b"+lf+"ac"+lf                            ,"/aa\z/m"                               ,"r") <>  []  then pause,end
if grep("b"+lf+"ac"+lf                            ,"/aa$/m"                                ,"r") <>  []  then pause,end
if grep("b"+lf+"ac"                               ,"/aa\Z/m"                               ,"r") <>  []  then pause,end
if grep("b"+lf+"ac"                               ,"/aa\z/m"                               ,"r") <>  []  then pause,end
if grep("b"+lf+"ac"                               ,"/aa$/m"                                ,"r") <>  []  then pause,end
if grep("ca"+lf+"b"+lf                            ,"/aa\Z/"                                ,"r") <>  []  then pause,end
if grep("ca"+lf+"b"+lf                            ,"/aa\z/"                                ,"r") <>  []  then pause,end
if grep("ca"+lf+"b"+lf                            ,"/aa$/"                                 ,"r") <>  []  then pause,end
if grep("b"+lf+"ca"+lf                            ,"/aa\Z/"                                ,"r") <>  []  then pause,end
if grep("b"+lf+"ca"+lf                            ,"/aa\z/"                                ,"r") <>  []  then pause,end
if grep("b"+lf+"ca"+lf                            ,"/aa$/"                                 ,"r") <>  []  then pause,end
if grep("b"+lf+"ca"                               ,"/aa\Z/"                                ,"r") <>  []  then pause,end
if grep("b"+lf+"ca"                               ,"/aa\z/"                                ,"r") <>  []  then pause,end
if grep("b"+lf+"ca"                               ,"/aa$/"                                 ,"r") <>  []  then pause,end
if grep("ca"+lf+"b"+lf                            ,"/aa\Z/m"                               ,"r") <>  []  then pause,end
if grep("ca"+lf+"b"+lf                            ,"/aa\z/m"                               ,"r") <>  []  then pause,end
if grep("ca"+lf+"b"+lf                            ,"/aa$/m"                                ,"r") <>  []  then pause,end
if grep("b"+lf+"ca"+lf                            ,"/aa\Z/m"                               ,"r") <>  []  then pause,end
if grep("b"+lf+"ca"+lf                            ,"/aa\z/m"                               ,"r") <>  []  then pause,end
if grep("b"+lf+"ca"+lf                            ,"/aa$/m"                                ,"r") <>  []  then pause,end
if grep("b"+lf+"ca"                               ,"/aa\Z/m"                               ,"r") <>  []  then pause,end
if grep("b"+lf+"ca"                               ,"/aa\z/m"                               ,"r") <>  []  then pause,end
if grep("b"+lf+"ca"                               ,"/aa$/m"                                ,"r") <>  []  then pause,end
if grep("ab"+lf+"b"+lf                            ,"/ab\Z/"                                ,"r") <>  []  then pause,end
if grep("ab"+lf+"b"+lf                            ,"/ab\z/"                                ,"r") <>  []  then pause,end
if grep("ab"+lf+"b"+lf                            ,"/ab$/"                                 ,"r") <>  []  then pause,end
if grep("b"+lf+"ab"+lf                            ,"/ab\z/"                                ,"r") <>  []  then pause,end
if grep("ab"+lf+"b"+lf                            ,"/ab\Z/m"                               ,"r") <>  []  then pause,end
if grep("ab"+lf+"b"+lf                            ,"/ab\z/m"                               ,"r") <>  []  then pause,end
if grep("b"+lf+"ab"+lf                            ,"/ab\z/m"                               ,"r") <>  []  then pause,end
if grep("ac"+lf+"b"+lf                            ,"/ab\Z/"                                ,"r") <>  []  then pause,end
if grep("ac"+lf+"b"+lf                            ,"/ab\z/"                                ,"r") <>  []  then pause,end
if grep("ac"+lf+"b"+lf                            ,"/ab$/"                                 ,"r") <>  []  then pause,end
if grep("b"+lf+"ac"+lf                            ,"/ab\Z/"                                ,"r") <>  []  then pause,end
if grep("b"+lf+"ac"+lf                            ,"/ab\z/"                                ,"r") <>  []  then pause,end
if grep("b"+lf+"ac"+lf                            ,"/ab$/"                                 ,"r") <>  []  then pause,end
if grep("b"+lf+"ac"                               ,"/ab\Z/"                                ,"r") <>  []  then pause,end
if grep("b"+lf+"ac"                               ,"/ab\z/"                                ,"r") <>  []  then pause,end
if grep("b"+lf+"ac"                               ,"/ab$/"                                 ,"r") <>  []  then pause,end
if grep("ac"+lf+"b"+lf                            ,"/ab\Z/m"                               ,"r") <>  []  then pause,end
if grep("ac"+lf+"b"+lf                            ,"/ab\z/m"                               ,"r") <>  []  then pause,end
if grep("ac"+lf+"b"+lf                            ,"/ab$/m"                                ,"r") <>  []  then pause,end
if grep("b"+lf+"ac"+lf                            ,"/ab\Z/m"                               ,"r") <>  []  then pause,end
if grep("b"+lf+"ac"+lf                            ,"/ab\z/m"                               ,"r") <>  []  then pause,end
if grep("b"+lf+"ac"+lf                            ,"/ab$/m"                                ,"r") <>  []  then pause,end
if grep("b"+lf+"ac"                               ,"/ab\Z/m"                               ,"r") <>  []  then pause,end
if grep("b"+lf+"ac"                               ,"/ab\z/m"                               ,"r") <>  []  then pause,end
if grep("b"+lf+"ac"                               ,"/ab$/m"                                ,"r") <>  []  then pause,end
if grep("ca"+lf+"b"+lf                            ,"/ab\Z/"                                ,"r") <>  []  then pause,end
if grep("ca"+lf+"b"+lf                            ,"/ab\z/"                                ,"r") <>  []  then pause,end
if grep("ca"+lf+"b"+lf                            ,"/ab$/"                                 ,"r") <>  []  then pause,end
if grep("b"+lf+"ca"+lf                            ,"/ab\Z/"                                ,"r") <>  []  then pause,end
if grep("b"+lf+"ca"+lf                            ,"/ab\z/"                                ,"r") <>  []  then pause,end
if grep("b"+lf+"ca"+lf                            ,"/ab$/"                                 ,"r") <>  []  then pause,end
if grep("b"+lf+"ca"                               ,"/ab\Z/"                                ,"r") <>  []  then pause,end
if grep("b"+lf+"ca"                               ,"/ab\z/"                                ,"r") <>  []  then pause,end
if grep("b"+lf+"ca"                               ,"/ab$/"                                 ,"r") <>  []  then pause,end
if grep("ca"+lf+"b"+lf                            ,"/ab\Z/m"                               ,"r") <>  []  then pause,end
if grep("ca"+lf+"b"+lf                            ,"/ab\z/m"                               ,"r") <>  []  then pause,end
if grep("ca"+lf+"b"+lf                            ,"/ab$/m"                                ,"r") <>  []  then pause,end
if grep("b"+lf+"ca"+lf                            ,"/ab\Z/m"                               ,"r") <>  []  then pause,end
if grep("b"+lf+"ca"+lf                            ,"/ab\z/m"                               ,"r") <>  []  then pause,end
if grep("b"+lf+"ca"+lf                            ,"/ab$/m"                                ,"r") <>  []  then pause,end
if grep("b"+lf+"ca"                               ,"/ab\Z/m"                               ,"r") <>  []  then pause,end
if grep("b"+lf+"ca"                               ,"/ab\z/m"                               ,"r") <>  []  then pause,end
if grep("b"+lf+"ca"                               ,"/ab$/m"                                ,"r") <>  []  then pause,end
if grep("abb"+lf+"b"+lf                           ,"/abb\Z/"                               ,"r") <>  []  then pause,end
if grep("abb"+lf+"b"+lf                           ,"/abb\z/"                               ,"r") <>  []  then pause,end
if grep("abb"+lf+"b"+lf                           ,"/abb$/"                                ,"r") <>  []  then pause,end
if grep("b"+lf+"abb"+lf                           ,"/abb\z/"                               ,"r") <>  []  then pause,end
if grep("abb"+lf+"b"+lf                           ,"/abb\Z/m"                              ,"r") <>  []  then pause,end
if grep("abb"+lf+"b"+lf                           ,"/abb\z/m"                              ,"r") <>  []  then pause,end
if grep("b"+lf+"abb"+lf                           ,"/abb\z/m"                              ,"r") <>  []  then pause,end
if grep("ac"+lf+"b"+lf                            ,"/abb\Z/"                               ,"r") <>  []  then pause,end
if grep("ac"+lf+"b"+lf                            ,"/abb\z/"                               ,"r") <>  []  then pause,end
if grep("ac"+lf+"b"+lf                            ,"/abb$/"                                ,"r") <>  []  then pause,end
if grep("b"+lf+"ac"+lf                            ,"/abb\Z/"                               ,"r") <>  []  then pause,end
if grep("b"+lf+"ac"+lf                            ,"/abb\z/"                               ,"r") <>  []  then pause,end
if grep("b"+lf+"ac"+lf                            ,"/abb$/"                                ,"r") <>  []  then pause,end
if grep("b"+lf+"ac"                               ,"/abb\Z/"                               ,"r") <>  []  then pause,end
if grep("b"+lf+"ac"                               ,"/abb\z/"                               ,"r") <>  []  then pause,end
if grep("b"+lf+"ac"                               ,"/abb$/"                                ,"r") <>  []  then pause,end
if grep("ac"+lf+"b"+lf                            ,"/abb\Z/m"                              ,"r") <>  []  then pause,end
if grep("ac"+lf+"b"+lf                            ,"/abb\z/m"                              ,"r") <>  []  then pause,end
if grep("ac"+lf+"b"+lf                            ,"/abb$/m"                               ,"r") <>  []  then pause,end
if grep("b"+lf+"ac"+lf                            ,"/abb\Z/m"                              ,"r") <>  []  then pause,end
if grep("b"+lf+"ac"+lf                            ,"/abb\z/m"                              ,"r") <>  []  then pause,end
if grep("b"+lf+"ac"+lf                            ,"/abb$/m"                               ,"r") <>  []  then pause,end
if grep("b"+lf+"ac"                               ,"/abb\Z/m"                              ,"r") <>  []  then pause,end
if grep("b"+lf+"ac"                               ,"/abb\z/m"                              ,"r") <>  []  then pause,end
if grep("b"+lf+"ac"                               ,"/abb$/m"                               ,"r") <>  []  then pause,end
if grep("ca"+lf+"b"+lf                            ,"/abb\Z/"                               ,"r") <>  []  then pause,end
if grep("ca"+lf+"b"+lf                            ,"/abb\z/"                               ,"r") <>  []  then pause,end
if grep("ca"+lf+"b"+lf                            ,"/abb$/"                                ,"r") <>  []  then pause,end
if grep("b"+lf+"ca"+lf                            ,"/abb\Z/"                               ,"r") <>  []  then pause,end
if grep("b"+lf+"ca"+lf                            ,"/abb\z/"                               ,"r") <>  []  then pause,end
if grep("b"+lf+"ca"+lf                            ,"/abb$/"                                ,"r") <>  []  then pause,end
if grep("b"+lf+"ca"                               ,"/abb\Z/"                               ,"r") <>  []  then pause,end
if grep("b"+lf+"ca"                               ,"/abb\z/"                               ,"r") <>  []  then pause,end
if grep("b"+lf+"ca"                               ,"/abb$/"                                ,"r") <>  []  then pause,end
if grep("ca"+lf+"b"+lf                            ,"/abb\Z/m"                              ,"r") <>  []  then pause,end
if grep("ca"+lf+"b"+lf                            ,"/abb\z/m"                              ,"r") <>  []  then pause,end
if grep("ca"+lf+"b"+lf                            ,"/abb$/m"                               ,"r") <>  []  then pause,end
if grep("b"+lf+"ca"+lf                            ,"/abb\Z/m"                              ,"r") <>  []  then pause,end
if grep("b"+lf+"ca"+lf                            ,"/abb\z/m"                              ,"r") <>  []  then pause,end
if grep("b"+lf+"ca"+lf                            ,"/abb$/m"                               ,"r") <>  []  then pause,end
if grep("b"+lf+"ca"                               ,"/abb\Z/m"                              ,"r") <>  []  then pause,end
if grep("b"+lf+"ca"                               ,"/abb\z/m"                              ,"r") <>  []  then pause,end
if grep("b"+lf+"ca"                               ,"/abb$/m"                               ,"r") <>  []  then pause,end
if grep("x"                                       ,"/a*abc?xyz+pqr{3}ab{2,}xy{4,5}pq{0,6}AB{0,}zz/","r") <>  []  then pause,end
if grep("foo.bart"                                ,"/foo.bart/"                            ,"r") <>  1   then pause,end
if grep("abcd"+lf+"dxxx"                          ,"/^d[x][x][x]/m"                        ,"r") <>  1   then pause,end
if grep("xxxtt"                                   ,"/tt+$/"                                ,"r") <>  1   then pause,end
if grep("aaaXbX"                                  ,"/\GX.*X/"                              ,"r") <>  []  then pause,end
if grep("Changes"                                 ,"/\.c(pp|xx|c)?$/i"                     ,"r") <>  []  then pause,end
if grep("IO.c"                                    ,"/\.c(pp|xx|c)?$/i"                     ,"r") <>  1   then pause,end
if grep("C:/"                                     ,"/^([a-z]:)/"                           ,"r") <>  []  then pause,end
if grep(lf+"x aa"                                 ,"/^\S\s+aa$/m"                          ,"r") <>  1   then pause,end
if grep("ab"                                      ,"/(^|a)b/"                              ,"r") <>  1   then pause,end
if grep("abcab"                                   ,"/(\w)?(abc)\1b/"                       ,"r") <>  []  then pause,end
if grep("a,b,c"                                   ,"/^(?:.,){2}c/"                         ,"r") <>  1   then pause,end
if grep("a,b,c"                                   ,"/^(?:[^,]*,){2}c/"                     ,"r") <>  1   then pause,end
if grep(""                                        ,"/(?i)/"                                ,"r") <>  1  then pause,end
if grep("a"+lf+"xb"+lf                            ,"/(?!\A)x/m"                            ,"r") <>  1   then pause,end
if grep("123"+lf+"abcabcabcabc"+lf                ,"/^.{9}abc.*\n/m"                       ,"r") <>  1   then pause,end
if grep("a"                                       ,"/^(a)?(?(1)a|b)+$/"                    ,"r") <>  []  then pause,end
if grep("x1"                                      ,"/^(0+)?(?:x(1))?/"                     ,"r") <>  1   then pause,end
if grep("012cxx0190"                              ,"/^([0-9a-fA-F]+)(?:x([0-9a-fA-F]+)?)(?:x([0-9a-fA-F]+))?/","r") <>  1   then pause,end
if grep("aaaacccc"                                ,"/((?:aaaa|bbbb)cccc)?/"                ,"r") <>  1   then pause,end
if grep("bbbbcccc"                                ,"/((?:aaaa|bbbb)cccc)?/"                ,"r") <>  1   then pause,end
if grep("a"+lf+"b"+lf                             ,"/b\s^/m"                               ,"r") <>  []  then pause,end
if grep("a"                                       ,"/\ba/"                                 ,"r") <>  1   then pause,end
if grep("AbCd"                                    ,"/ab(?i)cd/"                            ,"r") <>  []  then pause,end
if grep("abCd"                                    ,"/ab(?i)cd/"                            ,"r") <>  1   then pause,end
if grep("Oo"                                      ,"/^(o)(?!.*\1)/i"                       ,"r") <>  []  then pause,end
if grep("2"                                       ,"/2(]*)?$\1/"                           ,"r") <>  1   then pause,end
if grep("......abef"                              ,"/.*a(?!(b|cd)*e).*f/"                  ,"r") <>  []  then pause,end
if grep("fools"                                   ,"/(foo|fool|x.|money|parted)$/"         ,"r") <>  []  then pause,end
if grep("fools"                                   ,"/(x.|foo|fool|x.|money|parted|y.)$/"   ,"r") <>  []  then pause,end
if grep("fools"                                   ,"/(foo|fool|money|parted)$/"            ,"r") <>  []  then pause,end
//========================================================================================
// depend of PCRE library options
//if grep('a\b'                                     ,'/a\\b/'                                ,'r') <>  1   then pause,end
//if grep('bbbbXcXaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa'  ,'/.X(.+)+X/'                            ,'r') <>  1   then pause,end
//if grep('bbbbXcXXaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa' ,'/.X(.+)+XX/'                           ,'r') <>  1   then pause,end
//if grep('bbbbXXcXaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa' ,'/.XX(.+)+X/'                           ,'r') <>  1   then pause,end
//if grep('bbbbXXaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa'  ,'/.X(.+)+X/'                            ,'r') <>  []  then pause,end
//if grep('bbbbXXXaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa' ,'/.X(.+)+XX/'                           ,'r') <>  []  then pause,end
//if grep('bbbbXXXaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa' ,'/.XX(.+)+X/'                           ,'r') <>  []  then pause,end
//if grep('bbbbXcXaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa'  ,'/.X(.+)+[X]/'                          ,'r') <>  1   then pause,end
//if grep('bbbbXcXXaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa' ,'/.X(.+)+[X][X]/'                       ,'r') <>  1   then pause,end
//if grep('bbbbXXcXaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa' ,'/.XX(.+)+[X]/'                         ,'r') <>  1   then pause,end
//if grep('bbbbXXaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa'  ,'/.X(.+)+[X]/'                          ,'r') <>  []  then pause,end
//if grep('bbbbXXXaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa' ,'/.X(.+)+[X][X]/'                       ,'r') <>  []  then pause,end
//if grep('bbbbXXXaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa' ,'/.XX(.+)+[X]/'                         ,'r') <>  []  then pause,end
//if grep('bbbbXcXaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa'  ,'/.[X](.+)+[X]/'                        ,'r') <>  1   then pause,end
//if grep('bbbbXcXXaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa' ,'/.[X](.+)+[X][X]/'                     ,'r') <>  1   then pause,end
//if grep('bbbbXXcXaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa' ,'/.[X][X](.+)+[X]/'                     ,'r') <>  1   then pause,end
//if grep('bbbbXXaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa'  ,'/.[X](.+)+[X]/'                        ,'r') <>  []  then pause,end
//if grep('bbbbXXXaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa' ,'/.[X](.+)+[X][X]/'                     ,'r') <>  []  then pause,end
//if grep('bbbbXXXaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa' ,'/.[X][X](.+)+[X]/'                     ,'r') <>  []  then pause,end
//if grep('cabd'                                    ,'/a(?{})b/'                             ,'r') <>  1   then pause,end
//if grep('a'                                       ,'/(?(?{0})b|a)/'                        ,'r') <>  1   then pause,end
//if grep('a'                                       ,'/(?(?{1})a|b)/'                        ,'r') <>  1   then pause,end
//if grep('x'                                       ,'/(??{})/'                              ,'r') <>  1   then pause,end
//if grep('cabd'                                    ,'/a(?{"\{"})b/'                         ,'r') <>  1   then pause,end
//if grep('A\B'                                     ,'/a\\b/i'                               ,'r') <>  1   then pause,end
//if grep('aaaaaaaaaaaaaaab'                        ,'/(a|aa|aaa|aaaa|aaaaa|aaaaaa)(??{$1&&"foo"})(b|c)/','r') <>  []  then pause,end
//========================================================================================
str = "世界您好";
needle = "界";
[r,w] = grep(str,needle);
if r <> 1 then pause,end
if w <> 1 then pause,end

str = "азеазея";
needle = "з";
[r,w] = grep(str,needle);
if r <> 1 then pause,end
if w <> 1 then pause,end

str = "ハロー・ワールド";
needle = "ワ";
[r,w] = grep(str,needle);
if r <> 1 then pause,end
if w <> 1 then pause,end

str = "เฮลโลเวิลด์";
needle = "วิ";
[r,w] = grep(str,needle);
if r <> 1 then pause,end
if w <> 1 then pause,end

str = "حريات وحقوق";
needle = "و";
[r,w] = grep(str,needle);
if r <> 1 then pause,end
if w <> 1 then pause,end

str = "תוכנית";
needle = "ו";
grep(str,needle);
if r <> 1 then pause,end
if w <> 1 then pause,end

//========================================================================================
// Chinese
str = "世界您好";
[r,w] = grep(str,"/^世界/","r");
if r <> 1 then pause,end
if w <> 1 then pause,end

[r,w] = grep(str,"/世界$/","r");
if r <> [] then pause,end
if w <> [] then pause,end

[r,w] = grep(str,"/您好$/","r");
if r <> 1 then pause,end
if w <> 1 then pause,end

[r,w] = grep(str,"/^您好/","r");
if r <> [] then pause,end
if w <> [] then pause,end

[r,w] = grep(str,"/界您/","r");
if r <> 1 then pause,end
if w <> 1 then pause,end

[r,w] = grep(str,"界您");
if r <> 1 then pause,end
if w <> 1 then pause,end

[r,w] = grep(str,"/界_您/","r");
if r <> [] then pause,end
if w <> [] then pause,end
//========================================================================================
// Russian
str = "привет мир";
[r,w] = grep(str,"/^привет/","r");
if r <> 1 then pause,end
if w <> 1 then pause,end

[r,w] = grep(str,"/привет$/","r");
if r <> [] then pause,end
if w <> [] then pause,end

[r,w] = grep(str,"/мир$/","r");
if r <> 1 then pause,end
if w <> 1 then pause,end

[r,w] = grep(str,"/^мир/","r");
if r <> [] then pause,end
if w <> [] then pause,end

[r,w] = grep(str,"/вет\sм/","r");
if r <> 1 then pause,end
if w <> 1 then pause,end

[r,w] = grep(str,"/вет_м/","r");
if r <> [] then pause,end
if w <> [] then pause,end
//========================================================================================
// Cyrilic
str = "АБВГДЄЖЅЗИІКЛМНОПҀРСТѸФХѠЦЧШЩЪЪІЬѢЮѦѨѪѬѮѰѲѴѤ";

[r,w] = grep(str,"/^АБВГДЄЖЅЗИІКЛМНОПҀР/","r");
if r <> 1 then pause,end
if w <> 1 then pause,end

[r,w] = grep(str,"/АБВГДЄЖЅЗИІКЛМНОПҀР$/","r");
if r <> [] then pause,end
if w <> [] then pause,end

[r,w] = grep(str,"/ЧШЩЪЪІЬѢЮѦѨѪѬѮѰѲѴѤ$/","r");
if r <> 1 then pause,end
if w <> 1 then pause,end

[r,w] = grep(str,"/^ЧШЩЪЪІЬѢЮѦѨѪѬѮѰѲѴѤ/","r");
if r <> [] then pause,end
if w <> [] then pause,end

[r,w] = grep(str,"/ИІКЛМНОПҀРСТѸФХѠЦЧШЩЪ/","r");
if r <> 1 then pause,end
if w <> 1 then pause,end

[r,w] = grep(str,"/ИІКЛМНОПҀ_РСТѸФХѠЦЧШЩЪ/","r");
if r <> [] then pause,end
if w <> [] then pause,end
//========================================================================================
// Japanese
str = "丑丞串乃之乎也云亘亙些亦亥亨亮仔伊伎伍伽佃佑伶侃侑俄侠俣俐侶倭俺倶倦倖偲僅傭儲允兎兜其冥冴冶凄凌凜凛凧凪凰凱函刹劉劫勁勃";

[r,w] = grep(str,"/^丑丞串乃之乎也云亘亙些亦/","r");
if r <> 1 then pause,end
if w <> 1 then pause,end

[r,w] = grep(str,"/丑丞串乃之乎也云亘亙些亦$/","r");
if r <> [] then pause,end
if w <> [] then pause,end

[r,w] = grep(str,"/凰凱函刹劉劫勁勃$/","r");
if r <> 1 then pause,end
if w <> 1 then pause,end

[r,w] = grep(str,"/^凰凱函刹劉劫勁勃/","r");
if r <> [] then pause,end
if w <> [] then pause,end

[r,w] = grep(str,"/亨亮仔伊伎伍伽佃佑伶侃/","r");
if r <> 1 then pause,end
if w <> 1 then pause,end

[r,w] = grep(str,"/亨亮仔伊_伎伍伽佃佑伶侃/","r");
if r <> [] then pause,end
if w <> [] then pause,end
//========================================================================================
// Thaï
str = "มกระดุกกระดิก";
[r,w] = grep(str,"/^มกระดุกกร/","r");
if r <> 1 then pause,end
if w <> 1 then pause,end

[r,w] = grep(str,"/มกระดุกกร$/","r");
if r <> [] then pause,end
if w <> [] then pause,end

[r,w] = grep(str,"/กกระดิก$/","r");
if r <> 1 then pause,end
if w <> 1 then pause,end

[r,w] = grep(str,"/^กกระดิก/","r");
if r <> [] then pause,end
if w <> [] then pause,end

[r,w] = grep(str,"/ดุกก/","r");
if r <> 1 then pause,end
if w <> 1 then pause,end

[r,w] = grep(str,"/ดุก_ก/","r");
if r <> [] then pause,end
if w <> [] then pause,end
//========================================================================================
