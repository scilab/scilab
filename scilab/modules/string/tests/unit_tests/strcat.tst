// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2007-2008 - INRIA
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

//===============================
// unit tests strcat
//===============================
if strcat(['abc','abd','aa','bxe'])<>'abcabdaabxe' then pause,end
//===============================
if strcat(['abc','abd';'aa','bxe'])<>'abcaaabdbxe' then pause,end
//===============================
if strcat(['abc';'abd';'aa';'bxe'])<>'abcabdaabxe' then pause,end
//===============================
if strcat(['abc','abd','aa','bxe'],',')<>'abc,abd,aa,bxe' then pause,end
//===============================
if strcat('abc')<>'abc' then pause,end
//===============================
if strcat('abc','sd')<>'abc' then pause,end
//===============================
if strcat('') <> '' then pause,end
//===============================
if strcat(['','']) <> '' then pause,end
//===============================
if strcat('','')<>'' then pause,end
//===============================
if strcat([])<>[] then pause,end
//===============================
if strcat([],'')<>[] then pause,end
//===============================
if execstr('strcat([],3)','errcatch') == 0  then pause,end
//===============================
if strcat([],'3') <> [] then pause,end
//===============================
if strcat(['abc','abd','aa','bxe']) <> 'abc'+'abd'+'aa'+'bxe' then pause,end
//===============================
if strcat(['abc','abd','aa','bxe'],'')<>'abcabdaabxe' then pause,end
//===============================
fd = mopen(SCI+'/modules/string/tests/unit_tests/text.txt','r');
txt = mgetl( fd );
mclose( fd );
NEW_TXT_A = strcat(txt);
NEW_TXT = strcat(NEW_TXT_A,'END OF FILE');
if ( length(NEW_TXT) <>length(NEW_TXT_A) ) then pause,end
//===============================
if strcat(['abc','abd','aa','bxe'],'| , |')<>'abc| , |abd| , |aa| , |bxe' then pause,end
if length(strcat(['abc','abd','aa','bxe'],'| , |')) <> 26 then pause,end
//===============================
