//<-- CLI SHELL MODE -->
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - Digiteo - unknown author
// Copyright (C) 2009 - Digiteo - Vincent LIARD
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

a = [1;2;3];
n=size(a,'*');
r1 = norm(1/n*exp(2*%i*%pi*(0:n-1)'.*.(0:n-1)/n)*a -fft([1;2;3],1));
if r1 > 10 *%eps then pause,end
r2 = norm(exp(-2*%i*%pi*(0:n-1)'.*.(0:n-1)/n)*a -fft(a,-1));
if r2 > 100 *%eps then pause,end
sample_rate = 1000;
t = 0:1/sample_rate:0.6;
N = size(t,'*');
s = sin(2*%pi*50*t)+sin(2*%pi*70*t+%pi/4)+grand(1,N,'nor',0,1);
y = fft(s);
if size(y) <> [1 601] then pause,end

