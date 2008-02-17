// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2007-2008 - INRIA
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

//===============================
a = '  123   ';
r = stripblanks(a);
if ( r <> '123' ) then pause,end
//===============================
b = '!'+ a +'!';
r = stripblanks(b);
if ( r <> b ) then pause,end
//===============================
b = [];
r = stripblanks(b);
if ( r <> b ) then pause,end
//===============================
a=['  123   ',' xyz'];
r = stripblanks(a);
if ( r <> ['123','xyz'] ) then pause,end
//===============================
a=['',''];
r = stripblanks(a);
if ( r <> ['',''] ) then pause,end
//===============================
a=' S C I L A B ';
r = stripblanks(a);
if ( r <> 'S C I L A B' ) then pause,end
//===============================
TAB_CHAR = ascii(9);
//===============================
a = TAB_CHAR + '  123   ' + TAB_CHAR;
r = stripblanks(a,%T);
if ( r <> '123' ) then pause,end
//===============================
r = stripblanks(a,%F);
if ( r <> a ) then pause,end
//===============================
a=[TAB_CHAR + '  123   '+TAB_CHAR,' xyz'];
r = stripblanks(a);
if ( r <> [TAB_CHAR + '  123   '+TAB_CHAR,'xyz'] ) then pause,end
//===============================
a=[TAB_CHAR + '  123   '+TAB_CHAR,' xyz'];
r = stripblanks(a,%F);
if ( r <> [TAB_CHAR + '  123   '+TAB_CHAR,'xyz'] ) then pause,end
//===============================
a=[TAB_CHAR + '  123   '+TAB_CHAR,' xyz'];
r = stripblanks(a,%T);
if ( r <> [TAB_CHAR + '  123   '+TAB_CHAR,'xyz'] ) then pause,end
//===============================
STRING = ' '+ascii(9)+' '+'S'+' '+ascii(9)+'C'+' '+ascii(9)+'I'+' '+ascii(9)+'L'+' '+ascii(9)+'A'+' '+ascii(9)+'B'+' '+ascii(9)+' ';
//===============================
R1 = str2code('!'+stripblanks(STRING,%T)+'!');
W1 = [  38.;  - 28.;    40.;  - 40.;  - 12.;    40.;  - 40.;  - 18.;    40.;  - 40.;  - 21.;    40.;  - 40.;  - 10.; 40.;  - 40.;  - 11.;    38]; 
if (R1 <> W1) then pause,end
//===============================
R2 = str2code('!'+stripblanks(STRING,%F)+'!');
W2 = [38.;  - 40.;   40.;  - 28.;    40.;  - 40.;  - 12.;    40.;  - 40.;  - 18.;    40.;  - 40.;  - 21.;    40.;  - 40.;  - 10.;    40.;  - 40.;  - 11.;    40.;  - 40.;    38.];
if (R2 <> W2) then pause,end

// = strings with only blank character =========================================

if(stripblanks("")<>"")              then pause,end
if(stripblanks(" ")<>"")             then pause,end
if(stripblanks("      ")<>"")        then pause,end

if(stripblanks("",%T)<>"")           then pause,end
if(stripblanks(" ",%T)<>"")          then pause,end
if(stripblanks("      ",%T)<>"")     then pause,end

if(stripblanks(ascii(9))<>ascii(9))  then pause,end
if(stripblanks(ascii(9),%T)<>"")     then pause,end
