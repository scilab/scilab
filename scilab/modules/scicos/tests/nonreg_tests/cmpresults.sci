// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Laurent VAYLET
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function cmpresults(testName,nbCols,idxCols,plotTitle)

disp(" -= " + testName + " =-")
disp("Reading results from Scilab 4...")
A  = mgetl(testName + ".out.ref");
AA = zeros(size(A,1),nbCols);
for k = 1:size(A,1)
  AA(k,:) = msscanf(A(k),mtlb_repmat('%f ',[1 nbCols]));
end

disp("Reading results from Scilab 5...")
B  = mgetl(testName + ".out");
BB = zeros(size(B,1),nbCols);
for k = 1:size(B,1)
  BB(k,:) = msscanf(B(k),mtlb_repmat('%f ',[1 nbCols]));
end

disp("Plotting diff = f(t)...")
t = AA(:,1);
plot(t,BB(:,idxCols)-AA(:,idxCols))
title(plotTitle)
