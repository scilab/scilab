function [Stmp,Ws]=rowregul(Sl,Alfa,Beta);
//[Stmp,Ws]=regul(Sl) computes a polynomial-state-space postfilter 
//Ws such that Stmp=Ws*Sl is proper and has D full rank
//Poles at infinity of Sl are moved to -Alfa;
//Zeros at infinity of Sl are moved to -Beta;
//Sl is asummed right invertible i.e. ss2tf(Sl) full row rank
//!
// Copyright INRIA
[Stmp,Ws]=colregul(Sl',Alfa,Beta);
Stmp=Stmp';Ws=Ws';
endfunction
