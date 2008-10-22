// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2007-2008 - INRIA
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

//==============================================================================
// Benchmark for datenum function
//==============================================================================

// <-- BENCH NB RUN : 1000 -->

nb_field = 5000;

years  = int( rand(1:nb_field) * 2500 );
months = int( rand(1:nb_field) * 12   ) + 1;
days   = int( rand(1:nb_field) * 28   ) + 1;

// <-- BENCH START -->
datenum(years,months,days);
// <-- BENCH END -->
