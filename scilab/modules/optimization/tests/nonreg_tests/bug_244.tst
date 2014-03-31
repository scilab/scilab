// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Vincent COUVERT
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->

// <-- Non-regression test for bug 244 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=244
//
// <-- Short Description -->
//   recursion problems with fsolve


// Titration of a dibase with HCl (at the beginning, a little amount of NaOH was added)

// parameters
//C0=2.39e-4
C0=2e-4;
C02=0.02;
na0=2e-7;
v0=0.01;

pK1=6.9;
pK2=6.9;

// function to get the calculated pH
deff('[x]=f0(pH)','x=(10^(-pH-pK1)+2*10^(-2*pH))/(10^(-pK1-pK2)+10^(-pH-pK1)+10^(-2*pH))+((v0+v)*(10^(-pH)-10^(pH-14))+na0-C02*v)/(C0*v0)');
deff('[pH]=fpH(p)','v=p;pH=fsolve(7,f0)');

// data

X=[];Y=[];

X=[
//0	
0.00001	
0.00002	
0.00003	
0.00004	
0.00005	
0.000055	
0.00006	
0.000065	
0.00007	
0.000075	
0.00008	
0.000085	
0.00009	
0.000095	
0.0001	
0.000105	
0.00011	
0.000115	
0.00012	
0.000125	
0.00013	
0.000135	
0.00014	
0.000145	
0.00015	
0.000155	
0.00016	
0.000165	
0.00017	
0.000175	
0.00018	
0.000185	
0.00019	
0.000195	
0.0002	
0.000205	
0.00021	
0.000215	
0.00022	
0.000225	
0.00023	
0.000235	
0.00024	
0.000245	
0.00025	
0.000255	
0.00026	
0.000265	
0.00027	
0.000275	
0.00028	
0.000285	
0.000295	
0.000305	
0.00032	
0.000345	
0.000395	
0.000445	
];

Y=[
//8.21	
7.82	
7.64	
7.48	
7.37	
7.22	
7.20	
7.17	
7.13	
7.12	
7.12	
7.10	
7.07	
7.04	
7.07	
7.04	
7.01	
6.98	
6.94	
6.91	
6.87	
6.84	
6.80	
6.76	
6.73	
6.68	
6.61	
6.57	
6.51	
6.45	
6.36	
6.27	
6.14	
6.02	
5.84	
5.64	
5.34	
5.00	
4.80	
4.59	
4.46	
4.40	
4.33	
4.24	
4.18	
4.11	
4.07	
4.02	
3.99	
3.95	
3.91	
3.88	
3.86	
3.80	
3.76	
3.69	
3.60	
3.47	
3.36	
];

// fitting
Z=[Y;X];
deff('e=G(p,z)','pK1=p(1),pK2=p(2),v=z(2),pHexp=z(1),e=pHexp-fpH(v)');
[p,err]=datafit(G,Z,[6;7]);

// graphic part
clf()
//v=[0:1e-5:4.5e-4]
v=X;
fplot2d(v,fpH);
plot2d(X,Y,[-2]);
