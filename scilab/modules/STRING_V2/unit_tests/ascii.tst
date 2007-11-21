//===============================
// unit tests ascii
// INRIA 2007
//===============================
if or(ascii('abcdefghijklmnopqrstuvwxyz')<>(97:122)) then pause,end
if or(ascii(97:122)<>'abcdefghijklmnopqrstuvwxyz') then pause,end
if ascii('')<>[] then pause,end
if ascii([])<>'' then pause,end
//===============================
CODE_ASCII_SCILAB = [115 99 105 108 97 98];
str_scilab = 'scilab';
if ascii(str_scilab) <> CODE_ASCII_SCILAB then pause,end
if (ascii(CODE_ASCII_SCILAB) <> str_scilab) then pause,end
//===============================
if ascii([str_scilab 'a';'b' 'c'])  <> [115 99 105 108 97 98 98 97 99] then pause,end
//===============================
fd = mopen(SCI+'/modules/string/unit_tests/text.txt','r');
txt = mgetl( fd );
mclose( fd );
ASCII_TXT = ascii(txt);
TXT_FROM_ASCII_CODE = ascii(ASCII_TXT);
if (length(TXT_FROM_ASCII_CODE) <> 27455) then pause,end
//===============================