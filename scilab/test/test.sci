//Scalar
a = %s;
a = a + a*a;
b = a;
a = a + a*a;
a = a + a*a;
a2 = a + a*a;
//row
c1 = [b,b];
d1 = [a,a];
e1 = [b,a];
f1 = [a,b];
c1i = [b * %i,b];
d1i = [a,a * %i];
e1i = [b*%i + a,a*%i + b];
f1i = [a*%i + b,b*%i + a];


//col
c2 = [b;b];
d2 = [a;a];
e2 = [b;a];
f2 = [a;b];
c2i = [b * %i ; b];
d2i = [a ; a * %i];
e2i = [b*%i + a ; a*%i + b];
f2i = [a*%i + b ; b*%i + a];

//matrix

c3 = [a,b*%i;a*%i,a];
d3 = [a*%i,a;b,b*%i];
e3 = [a*%i,b*%i;b,a];
f3 = [a+%s,a+2*%s;a+3*%s,a+4*%s];
c3i = [a,b;a,a];
d3i = [a,a;b,b];
e3i = [a,b;b,a];
f3i = [a+%s,a+(1-2*%i)*%s;a+(-5+3*%i)*%s,a+(6-4*%i)*%s];

c1, d1, e1, f1, c1i, d1i, e1i, f1i, c2, d2, e2, f2, c2i, d2i, e2i, f2i, c3, d3, e3, f3, c3i, d3i, e3i, f3i