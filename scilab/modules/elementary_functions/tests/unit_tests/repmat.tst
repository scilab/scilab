// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 - INRIA - Serge Steer
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

if or(repmat(1:3,2,2)<>[1,2,3,1,2,3;1,2,3,1,2,3]) then pause,end
if or(repmat(1+2*%i,2)<>[ 1+%i*2, 1+%i*2; 1+%i*2, 1+%i*2]) then pause,end


a=int8([1 0 1;0 1 0]);
if or(repmat(a,2,3,2)<>matrix(int32([1;0;1;0;0;1;0;1;1;0;1;1;1;0;1;0;0;1;
                    0;1;1;0;1;0;1;0;1;0;0;1;0;1;1;0;1;0;1;0;1;0;0;1;0;1; 
                    1;0;1;1;1;0;1;0;0;1;0;1;1;0;1;0;1;0;1;0;0;1;0;1;1;0; 
                    1;0]),4,9,2)) then pause,end 

  
if or(repmat("Scilab",3,2)<>["Scilab","Scilab";"Scilab","Scilab";
                    "Scilab","Scilab"]) then pause,end

H=repmat([1/%s;2/(%s+1)],1,3);
if or(H.num<>[1,1,1;2,2,2]) then pause,end
if or(H.den<>[%s,%s,%s;1+%s,1+%s,1+%s]) then pause,end
if repmat([],2,2)<>[] then pause,end

