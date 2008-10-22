// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Pierre MARECHAL <pierre.marechal@inria.fr>
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// unit tests for cos() function (element wise cosine)
// =============================================================================


// 1. Interface
// ============

if execstr("cos()"   ,"errcatch")           == 0 then pause, end
if execstr("cos(1,2)","errcatch")           == 0 then pause, end
if execstr("cos(''my string'')","errcatch") == 0 then pause, end

// 2. Singular Values
// ==================

rt2 = sqrt(2);
rt3 = sqrt(3);

x = [0, %pi/6 , %pi/4 , %pi/3 , %pi/2 , 2*%pi/3 , 3*%pi/4   , 5*%pi/6  , %pi ];
v = [1, rt3/2 , rt2/2 , 1/2   , 0     , - 1/2   , - rt2/2   , -rt3/2   , -1  ];

if or(abs(cos(x)-v) > sqrt(%eps)) then pause, end


// 3. Not A Number
// ===============

if ~isnan(cos(%nan)) then pause, end
if ~isnan(cos(-%nan)) then pause, end


// 4. Limit values
// ===============

if ~isnan(real(cos(%inf)))    then pause, end
if imag(cos(%inf)) <> 0       then pause, end

if ~isnan(real(cos(-%inf)))   then pause, end
if imag(cos(-%inf)) <> 0      then pause, end



// 5. Properties
// =============

A = rand(100,100);
B = rand(100,100);

// cos(-x) = cos(x)
if or( cos(-A) - cos(A) > %eps) then pause, end

// cos(%pi/2 - x) = sin(x)
if or( cos(%pi/2 - A) - sin(A) > %eps) then pause, end

// cos(%pi - x) = - cos(x)
if or( cos(%pi - A) + cos(A) > %eps) then pause, end

// cos(%pi + x) = - cos(x)
if or( cos(%pi + A) + cos(A) > %eps) then pause, end

// cos(%pi/2 + x) = - sin(x)
if or( cos(%pi/2 + A) + sin(A) > %eps) then pause, end

// cos^2(a) + sin^2(a) = 1
if or( (cos(A)).^2 + (sin(A)).^2 - 1 > %eps) then pause, end

// cos(a + b) = cos(a) cos(b) - sin(a) sin(b)
if or( cos(A + B) - cos(A).*cos(B) + sin(A).*sin(B) > 2 * %eps) then pause, end

// cos(a -b) = cos(a) cos(b) + sin(a) sin(b)
if or( cos(A - B) - cos(A).*cos(B) - sin(A).*sin(B) > 2 * %eps) then pause, end

// sin(a + b) = sin(a) cos(b) + sin(b) cos(a)
if or( sin(A + B) - sin(A).*cos(B) - sin(B).*cos(A) > 2 * %eps) then pause, end

// sin(a -b) = sin(a) cos(b) - sin(b) cos(a)
if or( sin(A - B) - sin(A).*cos(B) + sin(B).*cos(A) > 2 * %eps) then pause, end

// cos(2a) = cos^2(a) - sin^2(a) = 2 cos^2(a) - 1 = 1 - 2 sin^2(a)
if or( cos(2*A) - (cos(A)).^2 + (sin(A)).^2 > 2 * %eps ) then pause, end
if or( cos(2*A) - 2 * (cos(A)).^2 + 1 > 2 * %eps ) then pause, end
if or( cos(2*A) - 1 + 2 * (sin(A)).^2 > 2 * %eps ) then pause, end
