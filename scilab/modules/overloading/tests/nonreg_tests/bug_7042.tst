// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - INRIA - Serge Steer <serge.steer@inria.fr>
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- Non-regression test for bug 7042 -->
//
// <-- CLI SHELL MODE -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id= 7042
//
// <-- Short Description -->
// insertion of an empty matrix into a struct (to remove a row or column)
// is buggy when multiple index is used
d11=struct('day',25,'month' ,'DEC','year',2006);
d12=struct('day',2,'month' ,'DEC','year',2006);
d21=struct('day',8,'month' ,'Feb','year',2010);
d22=struct('day',12,'month' ,'Feb','year',2010);

d=[d11 d12;d21 d22];

dd=d;
dd(1,:)=[];
if or(dd<>d(2,:)) then pause,end

dd=d;
dd(:,2)=[];
if or(dd<>d(:,1)) then pause,end

dd=d;
dd(:,:)=[];
if or(size(dd)<>0) then pause,end

dd=d;
dd(1)=[];
if or(dd<>d(2:4)) then pause,end

dd=d;
dd(1,1,3)=d11;
dd(:,:,3)=[];
if or(size(dd)<>[2 2 2]) then pause,end
if or(dd(:,:,1)<>d) then pause,end
