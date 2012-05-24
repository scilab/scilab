// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2007-2008 - INRIA
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

//==============================================================================

str       = "Madam,in Eden I''m Adam ";
str_clean = strsubst(str,' ','');
str_clean = strsubst(str_clean,',','');
str_clean = strsubst(str_clean,'''','');
str_clean = convstr(str_clean,'l');
r         = strrev(str_clean);
if r<>str_clean then pause,end

//==============================================================================

str                = "la mere gide digere mal";
str_without_blanks = strsubst(str,' ','');
r                  = strrev(str_without_blanks);
if r<>str_without_blanks then pause,end

//==============================================================================

str       = "a man, a plan, a canal : panama";
str_clean = strsubst(str,' ','');
str_clean = strsubst(str_clean,',','');
str_clean = strsubst(str_clean,':','');
r         = strrev(str_clean);
if r<>str_clean then pause,end

//==============================================================================

s = strrev('');
if s <> '' then pause,end

//==============================================================================

str1          = "Madam,in Eden I''m Adam";
str2          = "la mere gide digere mal";
str3          = "a man, a plan, a canal : panama";

str1_rev      = "madA m''I nedE ni,madaM";
str2_rev      = "lam eregid edig erem al";
str3_rev      = "amanap : lanac a ,nalp a ,nam a";


if strrev(str1) <> str1_rev then pause,end
if strrev(str2) <> str2_rev then pause,end
if strrev(str3) <> str3_rev then pause,end

if strrev(str1_rev) <> str1 then pause,end
if strrev(str2_rev) <> str2 then pause,end
if strrev(str3_rev) <> str3 then pause,end

//=================

str_test      = [ str1     str2     str3     ];
str_test_rev  = [ str1_rev str2_rev str3_rev ];

if strrev(str_test) <>  str_test_rev                               then pause,end
if strrev(str_test) <> [ str1_rev      str2_rev     str3_rev     ] then pause,end
if strrev(str_test) <> [ strrev(str1)  strrev(str2) strrev(str3) ] then pause,end

//=================

str_test      = [ str1     ; str2     ; str3     ];
str_test_rev  = [ str1_rev ; str2_rev ; str3_rev ];

if strrev(str_test) <>  str_test_rev                                  then pause,end
if strrev(str_test) <> [ str1_rev     ; str2_rev     ; str3_rev     ] then pause,end
if strrev(str_test) <> [ strrev(str1) ; strrev(str2) ; strrev(str3) ] then pause,end

//=================

str_test      = [ str1     str2     ; str3     str1     ];
str_test_rev  = [ str1_rev str2_rev ; str3_rev str1_rev ];

if strrev(str_test) <>  str_test_rev                                             then pause,end
if strrev(str_test) <> [ str1_rev     str2_rev     ; str3_rev     str1_rev     ] then pause,end
if strrev(str_test) <> [ strrev(str1) strrev(str2) ; strrev(str3) strrev(str1) ] then pause,end
