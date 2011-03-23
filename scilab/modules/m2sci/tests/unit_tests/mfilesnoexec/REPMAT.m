% Test file for function repmat()
% Copyright INRIA
% Matlab version: 7.0.0.19901 (R14)
% V.C.

A=repmat(2,2);
B=repmat([2,2],2);
C=repmat(1==1,2);
D=repmat([1==1 1==1],2);
E=repmat('str1',2);
F=repmat(['str1';'str2'],2);

G=repmat(2,[2,2]);
H=repmat([2,2],[2,2]);
I=repmat(1==1,[2,2]);
J=repmat([1==1 1==1],[2,2]);
K=repmat('str1',[2,2]);
L=repmat(['str1';'str2'],[2,2]);

%m2scideclare m|Unknown Unknown|Double|Real
N=repmat(2,m);
O=repmat(1==1,m);
P=repmat('str1',m);

%m2scideclare n|1 1|Double|Unknown
Q=repmat(2,n);
R=repmat(1==1,n);
S=repmat('str1',n);

A1=repmat(2,2,3);
B1=repmat([2,2],2,3);
C1=repmat(1==1,2,3);
D1=repmat([1==1 1==1],2,3);
E1=repmat('str1',2,3);
F1=repmat(['str1';'str2'],2,3);

G1=repmat(2,2,3);
H1=repmat([2,2],2,3);
I1=repmat(1==1,2,3);
J1=repmat([1==1 1==1],2,3);
K1=repmat('str1',2,3);
L1=repmat(['str1';'str2'],2,3);

%m2scideclare m2|Unknown Unknown|Double|Real
%m2scideclare n2|Unknown Unknown|Double|Real
N1=repmat(2,m2,n2);
O1=repmat(1==1,m2,n2);
P1=repmat('str1',m2,n2);

%m2scideclare m1|1 1|Double|Unknown
%m2scideclare n1|1 1|Double|Unknown
Q1=repmat(2,m1,n1);
R1=repmat(1==1,m1,n1);
S1=repmat('str1',m1,n1);

T=repmat(ones(2,2,2),2)
T=repmat(ones(2,2,2),[2,2])