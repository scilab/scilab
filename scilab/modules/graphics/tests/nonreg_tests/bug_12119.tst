// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2012 - Michael Baudin
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->
H = [
    0.    0.1007
    1.    0.0965
    2.    0.0996
    3.    0.1019
    4.    0.0974
    5.    0.1019
    6.    0.0977
    7.    0.1055
    8.    0.1006
    9.    0.0982
]
scf();
bar(H(:,1),H(:,2));

scf();
bar(H(:,1),H(:,2),"grouped");

scf();
bar(H(:,1),H(:,2),"stacked");

scf();
barh(H(:,1),H(:,2),"stacked");

scf();
barh(H(:,1),H(:,2),"grouped");
