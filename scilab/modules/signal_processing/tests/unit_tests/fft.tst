//<-- CLI SHELL MODE -->
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - Digiteo - unknown author
// Copyright (C) 2009 - Digiteo - Vincent LIARD
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

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

