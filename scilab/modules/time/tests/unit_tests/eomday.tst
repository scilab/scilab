// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2007-2008 - INRIA - Pierre MARECHAL <pierre.marechal@inria.fr>
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

// unit tests for eomday function
// =============================================================================

years    = 1900:1999;

[nr,nc]  = size(years);
months   = 2 * ones(nr,nc);

E        = eomday(years,months);
test     = years(find(E == 29));
test_ref = [ 1904 1908 1912 1916 1920 1924 1928 1932 1936 1940 1944 1948 1952 1956 1960 1964 1968 1972 1976 1980 1984 1988 1992 1996 ];

if or(test<>test_ref) then pause,end
