//===============================
str = 'Madam,in Eden I''m Adam ';
str_clean = strsubst(str,' ','');
str_clean = strsubst(str_clean,',','');
str_clean = strsubst(str_clean,'''','');
str_clean = convstr(str_clean,'l');
r = strrev(str_clean);
if r<>str_clean then pause,end
//===============================
str = 'la mere gide digere mal';
str_without_blanks = strsubst(str,' ','');
r = strrev(str_without_blanks);
if r<>str_without_blanks then pause,end
//===============================
str = 'a man, a plan, a canal : panama';
str_clean = strsubst(str,' ','');
str_clean = strsubst(str_clean,',','');
str_clean = strsubst(str_clean,':','');
r = strrev(str_clean);
if r<>str_clean then pause,end
//===============================
s = strrev('');
if s <> '' then pause,end
//===============================
