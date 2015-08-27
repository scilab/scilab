//<-- CLI SHELL MODE -->
// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- Non-regression test for bug 498 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=498
//
// <-- Short Description -->
//    Bug Report Id: 12361200372510504
//     I got another one!
//
//    On Scilab CVS with " min(), uint() " function
//    The Error Messages are:
//      none but wrong result
//    Commands:  --min(2,uint16(1))  >//WRONG
//     ans  =
//
//        2.  
//
//    -->min(2,int16(1)) //OK
//     ans  =
//
//        1.  
//
//    -->min(2,int8(1))   //OK
//     ans  =
//
//        1.  
// ...

if min(2,uint16(1))   <>    1.  then pause,end
if min(2,int16(1))    <>    1.  then pause,end
if min(2,int8(1))     <>    1.  then pause,end
if min(2,int8(10))    <>    2.  then pause,end
if min(2,uint8(1))    <>    1.  then pause,end
if max(uint8(10),2)   <>    10. then pause,end
if max(int8(10),2)    <>    10. then pause,end
if max(int8(10),-2)   <>    10. then pause,end
if max(int8(10),-2.6) <>    10. then pause,end
if min(int8(10),-2.6) <>  - 2.6 then pause,end
