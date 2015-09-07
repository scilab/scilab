//<-- CLI SHELL MODE -->
// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA - Serge Steer
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
h=syslin('c',1/%s);

if norm(freq(h.num,h.den,1:3)-[1 1/2 1/3])>1d-15 then pause,end
if norm(freq(h.num,h.den,%i*(1:3))+%i*[1 1/2 1/3])>1d-15 then pause,end


sl=tf2ss(h);
if norm(freq(sl.A,sl.B,sl.C,1:3)-[1 1/2 1/3])>1d-15 then pause,end
if norm(freq(sl.A,sl.B,sl.C,%i*(1:3))+%i*[1 1/2 1/3])>1d-15 then pause,end
if norm(freq(sl.A,sl.B,sl.C,0,1:3)-[1 1/2 1/3])>1d-15 then pause,end
if norm(freq(sl.A,sl.B,sl.C,0,%i*(1:3))+%i*[1 1/2 1/3])>1d-15 then pause,end


A=[1 2;3 4];H=A/%s;
if norm(freq(H.num,H.den,1:3)-[1 1/2 1/3].*.A,1)>1d-15 then pause,end
if norm(freq(H.num,H.den,%i*(1:3))+%i*[1 1/2 1/3].*.A,1)>1d-15 then pause,end

A=[1 2;3 4];H=A/%s;sl=tf2ss(H);
if norm(freq(sl.A,sl.B,sl.C,1:3)-[1 1/2 1/3].*.A,1)>3d-15 then pause,end
if norm(freq(sl.A,sl.B,sl.C,%i*(1:3))+%i*[1 1/2 1/3].*.A,1)>3d-15 then pause,end

if norm(freq(sl.A,sl.B,sl.C,sl.D,1:3)-[1 1/2 1/3].*.A,1)>3d-15 then pause,end
if norm(freq(sl.A,sl.B,sl.C,sl.D,%i*(1:3))+%i*[1 1/2 1/3].*.A,1)>3d-15 then pause,end

