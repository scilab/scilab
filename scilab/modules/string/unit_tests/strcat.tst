
//strcat

if strcat(['abc','abd','aa','bxe'])<>'abcabdaabxe' then pause,end
if strcat(['abc','abd','aa','bxe'],',')<>'abc,abd,aa,bxe' then pause,end
if strcat('abc')<>'abc' then pause,end
if strcat('abc','sd')<>'abc' then pause,end
