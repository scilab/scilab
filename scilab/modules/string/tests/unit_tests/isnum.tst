// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2007-2008 - INRIA - Pierre MARECHAL <pierre .marechal@inria.fr>
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

if ~csv_isnum('%pi')       then pause, end

if ~csv_isnum('1')         then pause, end
if ~csv_isnum('1.2')       then pause, end
if ~csv_isnum('+1.2')      then pause, end
if ~csv_isnum('-1.2')      then pause, end

if ~csv_isnum('1e2')       then pause, end
if ~csv_isnum('1d2')       then pause, end
if ~csv_isnum('1E2')       then pause, end
if ~csv_isnum('1D2')       then pause, end

if ~csv_isnum('1e+2')      then pause, end
if ~csv_isnum('1e-2')      then pause, end
if ~csv_isnum('+1e+2')     then pause, end
if ~csv_isnum('-1e+2')     then pause, end

if ~csv_isnum('-12e+3')    then pause, end

if ~csv_isnum('-1.2e+3')   then pause, end
if ~csv_isnum('-1.2e+345') then pause, end

if csv_isnum('a')          then pause, end
if csv_isnum('e')          then pause, end
if csv_isnum('d')          then pause, end
if csv_isnum('E')          then pause, end
if csv_isnum('e')          then pause, end

if csv_isnum('-1.2a+345')  then pause, end
if ~csv_isnum('-1.2e+3+45') then pause, end
if ~csv_isnum('-1.2e+3-45') then pause, end

if execstr("csv_isnum(1)","errcatch") == 0 then pause, end
