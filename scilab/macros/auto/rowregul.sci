function [Stmp,Ws]=rowregul(Sl,alfa,beta);
//[Stmp,Ws]=regul(Sl) computes a polynomial-state-space postfilter 
//Ws such that Stmp=Ws*Sl is proper and has D full rank
//Poles at infinity of Sl are moved to -alfa;
//Zeros at infinity of Sl are moved to -beta;
//Sl is asummed right invertible i.e. ss2tf(Sl) full row rank
//!
// Copyright INRIA
[Stmp,Ws]=colregul(Sl',alfa,beta);
Stmp=Stmp';Ws=Ws';

