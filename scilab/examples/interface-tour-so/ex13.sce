A=[1,2,3,4];
B=[10,20,30;40,50,60];

//Just display A and B
//C interface 
ex13c_1(A,B)

//Just display A and B
//Fortran Interface 
ex13f_1(A,B)

// C 
// executes myfct with input variables x=5 and y=6
//----------------------------

deff('[u,v,w]=myfct(x,y)','u=7+x,v=8+y,w=9+y')
x=5;y=6;[u,v,w]=myfct(x,y);
[p,q,r]=ex13c_2(1,2,3,4,5,6);
if p<>u | q <> v | r <> w then pause,end

// Fortran 
//  executes myfct with input variables x=5 and y=6
//----------------------------

deff('[u,v,w]=myfct(x,y)','u=7+x,v=8+y,w=9+y')
x=5;y=6;[u,v,w]=myfct(x,y);
[p,q,r]=ex13f_2(1,2,3,4,5,6);
if p<>u | q <> v | r <> w then pause,end

// C 
//----------------------------
deff('p=mypoly(x)','p=poly(x,''z'',''coeffs'')')
c=[1,2,3];
P1=ex13c_3(2,c);
if P1<>poly(c*2,'z','coeffs') then pause,end

// Fortran 
//----------------------------
deff('p=mypoly(x)','p=poly(x,''z'',''coeffs'')')
c=[1,2,3];
P1=ex13f_3(2,c);
if P1<>poly(c*2,'z','coeffs') then pause,end



