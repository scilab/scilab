// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

a0 = readgateway('core');
[a1,b1] = readgateway('core');
[a2,b2,c2] = readgateway('core');

if a0 <> a1 then pause,end
if a1 <> a2 then pause,end

if b1 <> b2 then pause,end
if size(a0) <> size(c2) then pause,end

if type(a2) <> 10 then pause,end
if type(b2) <> 1 then pause,end
if type(c2) <> 1 then pause,end

if and(a2 <> 'debug') then pause,end

m = getmodules();
for i=1:size(m,'*')
 [a2,b2,c2] = readgateway(m(i));
end
