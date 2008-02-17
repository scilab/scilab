// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

newest('SCI/etc/scilab.start')
// ans  =
//
//    1.


newest('SCI/nofile.txt')
// ans  =
//
//    1.

a=dir('C:\WINDOWS\system32');
f1=a(2);
r1=newest('C:\WINDOWS\system32\'+f1 )
//r1  =
//
//   22.

'C:\WINDOWS\system32\'+f1(r1)
// ans  =
//
// C:\WINDOWS\system32\Drivers

clear a f1 r1


a=dir('SCI\bin');
f1=a(2);
r1=newest(f1)
// r1  =
//
//    80.
f1(r1)
clear a f1 r1


// modified scilab.start before this test
newest('SCI\etc\scilab.start','SCI\etc\scilab.quit')
// ans  =
//
//    1.

newest(['SCI\etc\scilab.start','SCI\etc\scilab.quit'])
// ans  =
//
//    1.

newest(['SCI\etc\scilab.start';'SCI\etc\scilab.quit'])
// ans  =
//
//    1.

newest(['SCI\etc\scilab.quit';'SCI\etc\scilab.start'])
// ans  =
//
//    2.

newest(['SCI\etc\scilab.start','SCI\etc\scilab.start';'SCI\etc\scilab.quit','SCI\etc\scilab.quit'])
//                                                                                       !--error 999
//not a vector of filenames.


newest(['SCI\etc\scilab.start','SCI\etc\scilab.quit','SCI/nofile.txt'])
// ans  =
//
//    1.

newest(['SCI\etc\scilab.start','SCI/nofile.txt','SCI\etc\scilab.quit'])
// ans  =
//
//    1.

newest('SCI/nofile.txt')
// ans  =
//
//    1.

newest(['SCI\etc\scilab.start','SCI\etc\scilab.quit','SCI/nofile.txt'])
// ans  =
//
//    1.

newest(['SCI\etc\scilab.start','SCI/nofile.txt','SCI\etc\scilab.quit'])
// ans  =
//
//    1.

newest('SCI/nofile.txt')
// ans  =
//
//    1.

newest(['SCI\etc\scilab.start','SCI\etc\scilab.quit','SCI/nofile.txt'])
// ans  =
//
//    1.

newest(['SCI\etc\scilab.start','SCI/nofile.txt','SCI\etc\scilab.quit'])
// ans  =
//
//    1.

newest('rrr','SCI\bin\libscilab.dll',3)
//                                        !--error 999
//newest: incorrect 3th parameter(s). 

newest([])
// ans  =
//
//     []

newest()
// ans  =
//
//     []
  