//<-- CLI SHELL MODE -->
// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2016 - INRIA - Serge Steer
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
function y=csort(x)
    eps=1e-10
    y=gsort(real(x(abs(imag(x))<eps)),"g","i")
    y=[y;gsort(x(imag(x)<-eps),"g","i")]
    y=[y;gsort(x(imag(x)>eps),"g","i")]
endfunction


//siso case

S=zpk([2;1;0.5-%i;0.5+%i],[4;5;3-%i;3+%i],1,"c");
H=zpk2ss(S);
assert_checkalmostequal(spec(H.A), [5;4;3+%i;3-%i]);
assert_checkalmostequal(trzeros(H),[2;0.5+%i;0.5-%i;1]);

//mimo case
z11=[1 -0.5];p11=[-2 -3+2*%i -3-2*%i  ];k11=1;
S11=zpk(z11,p11,k11,"c");
z21=0.3;p21=[-3+2*%i -3-2*%i];k21=1.5;
S21=zpk(z21,p21,k21,"c");
S12=zpk([],0,1,"c");
S22=zpk(1,-3,1,"c");
S=zpk({z11 [];z21 1},{p11,0;p21 -3},[k11 1;k21 1],"c");
H=zpk2ss(S);
for i=1:2
    for j=1:2
        Hij=minss(H(i,j));
        assert_checkalmostequal(csort(spec(Hij.A)),csort(spec(zpk2ss(S(i,j)).A)),1e-10,1e-10);
    end
end
