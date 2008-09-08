// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Pierre MARECHAL <pierre.marechal@inria.fr>
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// unit tests for sinh() function (element wise cosine inverse)
// =============================================================================

if execstr("sinh()"   ,"errcatch") == 0 then pause, end
if execstr("sinh(1,2)","errcatch") == 0 then pause, end

x = [0, %pi/2*%i , %pi*%i , 3*%pi/2*%i ];
v = [0, %i       , 0      , -%i        ];

if or(abs(sinh(x)-v) > sqrt (%eps)) then pause, end

if ~isnan(sinh(%nan))        then pause, end
if ~isnan(sinh(-%nan))       then pause, end

if real(sinh(%inf)) <> %inf  then pause, end
if imag(sinh(%inf)) <> 0     then pause, end

if real(sinh(-%inf)) <> - %inf then pause, end
if imag(sinh(-%inf)) <> 0      then pause, end
