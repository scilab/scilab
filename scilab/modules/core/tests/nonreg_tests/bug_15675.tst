// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2018 - ESI Group - Antoine ELIAS
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
// <-- INTERACTIVE TEST -->
//
// <-- Non-regression test for bug 15675 -->
//
// <-- Bugzilla URL -->
// https://bugzilla.scilab.org/show_bug.cgi?id=15675
//
// <-- Short Description -->
// Wrong output of whos ( size and bytes )

format("e")
a_=zeros(512, 512);
whos -name a_ 

//must display :
//Name                     Type           Size           Bytes        
//
//a_                       constant       512x512        2097360
