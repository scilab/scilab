// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
a=dir('SCI\bin');
f1=a(2);
r1=newest(f1);
clear a f1 r1
// =============================================================================
if newest([]) <> [] then pause,end
if newest() <> [] then pause,end
if newest('SCI/etc/scilab.start') <> 1 then pause,end
if newest('SCI/nofile.txt') <> 1 then pause,end
if newest('SCI/modules/graphics/macros/bode.sci','SCI/modules/graphics/macros/bode.bin') <> 2 then pause,end
if newest(['SCI/modules/graphics/macros/bode.sci','SCI/modules/graphics/macros/bode.bin'])<> 2  then pause,end
if newest('SCI/modules/graphics/macros/bode.'+['sci','bin']) <> 2 then pause,end
if newest(['SCI\etc\scilab.start';'SCI\etc\scilab.quit']) <> 2 then pause,end
if newest(['SCI\etc\scilab.quit';'SCI\etc\scilab.start']) <> 1 then pause,end
if newest(['SCI\etc\scilab.start','SCI\etc\scilab.quit','SCI/nofile.txt']) <> 2 then pause,end
if newest(['SCI\etc\scilab.start','SCI/nofile.txt','SCI\etc\scilab.quit']) <> 3 then pause,end
if newest('SCI/nofile.txt') <> 1 then pause,end
if newest(['SCI\etc\scilab.start','SCI\etc\scilab.quit','SCI/nofile.txt']) <> 2 then pause,end
if newest(['SCI\etc\scilab.start','SCI/nofile.txt','SCI\etc\scilab.quit']) <> 3 then pause,end
if newest('SCI/nofile.txt') <> 1 then pause,end
if newest(['SCI\etc\scilab.start','SCI\etc\scilab.quit','SCI/nofile.txt']) <> 2 then pause,end
if newest(['SCI\etc\scilab.start','SCI/nofile.txt','SCI\etc\scilab.quit']) <> 3 then pause,end
M = ['SCI\etc\scilab.start','SCI\etc\scilab.start';'SCI\etc\scilab.quit','SCI\etc\scilab.quit'];
if newest(M) <> 2 then pause,end
if execstr('newest(''rrr'',''SCI\bin\libscilab.dll'',3)','errcatch') <> 999 then pause,end
// =============================================================================
