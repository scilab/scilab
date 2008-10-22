// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- INTERACTIVE TEST -->

a=1;
deff('[x]=tata(y)','x=-y,write(%io(2),''enter quit''),pause,x=+20')
xx=tata(10);
quit;
if 10+xx<>0 then pause,end
b=2;
deff('[x]=tata(y)',['x=-y'
                 'write(6,''enter return'')'
                 'pause'
                 'x=+20'])
xx=tata(0);
return
if 20-xx<>0 then pause,end
deff('[x]=tata(y)','x=-y,write(6,''type x=resume(0)''),pause')
xx=tata(40);
x=resume(0)
if xx<>0 then pause,end
clear x
text=['after quit '
      'undefined variable x']
deff('[x]=toto(n)',['v=100'
                    'write(%io(2),text)'
                    'pause'
                     'x=n'])
write(%io(2),'enter abort (after receiving this message:)')
toto(0);
abort

