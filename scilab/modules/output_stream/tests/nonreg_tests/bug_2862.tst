// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - allan.cornet@inria.fr
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

printf("%x",2^31-1);

printf("%x",2^31);

printf("%x",2^31+1);

if sprintf("%x",2^31-1) <> '7fffffff' then pause,end

if sprintf("%x",2^31) <> '80000000' then pause,end

if sprintf("%x",2^31+1) <> '80000001' then pause,end

if sprintf("%x",2^31+2) <> '80000002' then pause,end

