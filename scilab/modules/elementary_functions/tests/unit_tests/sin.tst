// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Pierre MARECHAL <pierre.marechal@inria.fr>
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// unit tests for sin() function (element wise cosine inverse)
// =============================================================================

if execstr("sin()"   ,"errcatch") == 0 then pause, end
if execstr("sin(1,2)","errcatch") == 0 then pause, end

rt2 = sqrt (2);
rt3 = sqrt (3);
x = [0, %pi/6 , %pi/4 , %pi/3 , %pi/2 , 2*%pi/3 , 3*%pi/4 , 5*%pi/6 , %pi];
v = [0, 1/2   , rt2/2 , rt3/2 , 1     , rt3/2   , rt2/2   , 1/2     , 0  ];

if or(abs(sin(x)-v) > sqrt(%eps)) then pause, end

if ~isnan(sin(%nan)) then pause, end
if ~isnan(sin(-%nan)) then pause, end

if ~isnan(real(sin(%inf)))    then pause, end
if imag(sin(%inf)) <> 0       then pause, end

if ~isnan(real(sin(-%inf)))   then pause, end
if imag(sin(-%inf)) <> 0      then pause, end
