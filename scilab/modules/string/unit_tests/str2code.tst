
//str2code
if or(str2code('abcdefghijklmnopqrstuvwxyz')<>(10:35)') then pause,end
if or(str2code('ABCDEFGHIJKLMNOPQRSTUVWXYZ')<>-(10:35)') then pause,end
if or(str2code('0123456789')<>(0:9)') then pause,end
if str2code(emptystr())<>[] then pause,end
