// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Michael Baudin
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
//
// introduction.sce --
//   The scripts used in the introduction chapter of "Scilab and Statistics".
//
//
// Empirical Cumulated Density Function of Normal Law with 200 samples
//
N=200;
x = rand(1,N,"normal");
Xsorted =gsort(x,"g","i"); 
Ydata = (1:N)/N;
plot(Xsorted,Ydata);
f=gcf();
f.children.title.text="Empirical Cumulated Density Function of Normal Law";
compound=f.children.children(1);
h1=captions(compound.children,"rand() with Polar method");
h1.legend_location="in_upper_left";
nbplots = 1;
for i = 1:nbplots
  mypoly = f.children.children(2).children(i);
  mypoly.thickness=2;
  mypoly.mark_size=2;
  mypoly.mark_size_unit="tabulated";
end
filename = "introduction_ecdfnormal.png";
xs2png(0,filename);

//
// Empirical Cumulated Density Function of Normal Law with 200 samples
// Compared with "exact" Cumulated Density Function.
//
N=200;
x = rand(1,N,"normal");
Xsorted =gsort(x,"g","i"); 
Ydata = (1:N)/N;
x=linspace(-3,3,100);
P=cdfnor("PQ",x,zeros(x),ones(x));
plot(Xsorted,Ydata,x,P);
f=gcf();
f.children.title.text="Empirical Cumulated Density Function of Normal Law";
compound=f.children.children(1);
h1=captions(compound.children,["cdfnor() with Rational Functions","rand() with Polar method"]);
h1.legend_location="in_upper_left";
nbplots = 2;
for i = 1:nbplots
  mypoly = f.children.children(2).children(i);
  mypoly.thickness=2;
  mypoly.mark_size=2;
  mypoly.mark_size_unit="tabulated";
end
filename = "introduction_ecdfcomparison.png";
xs2png(0,filename);

//
// Observe convergence of mean and variance as N > infinity
//
nbpoints = 5;
means=zeros(nbpoints,1);
vars=zeros(nbpoints,1);
stdevs=zeros(nbpoints,1);
nlist = 1:nbpoints;
for i = nlist
  N=10^i;
  x = rand(1,N,"normal");
  means(i) = mean(x);
  vars(i) = variance(x);
  stdevs(i) = stdev(x);
end
plot(nlist,[means,vars,stdevs]);
f=gcf();
f.children.x_label.text="Log - Number of Samples";
f.children.title.text="Convergence of moments of Normal Law";
compound=f.children.children(1);
h1=captions(compound.children,["Standard Deviation","Variance","Mean"]);
h1.legend_location="in_upper_right";
nbplots = 3;
for i = 1:nbplots
  mypoly = f.children.children(2).children(i);
  mypoly.thickness=2;
  mypoly.mark_size=2;
  mypoly.mark_size_unit="tabulated";
end
filename = "introduction_convergencemoments.png";
xs2png(0,filename);

