//scalar
a = %s - 5 + %i;
b = 3 - %i;
c = a + b;

//scalar + matrix
d = [1 + %i,2 - %i;-3 - %i,-4 + %i];
e = %s - 5 + %i;
f = d + e;

//matrix + scalar
g = 3 + %i;
h = [%s + 2 + %i, %s + 3 - %i, %s - 4 + %i, %s - 5 - %i; - %s + 6 + %i, -%s + 7 - %i, - %s - 8 + %i, - %s - 9 - %i];
i = h + g;

//matrix + matrix
j = [+1+%i,+2-%i,-3+%i,+4+%i;+5+%i,+6-%i,-7+%i,-8-%i];
k = [%s + 2 + %i, %s + 3 - %i, %s - 4 + %i, %s - 5 - %i; - %s + 6 + %i, -%s + 7 - %i, - %s - 8 + %i, - %s - 9 - %i];
l = k + j

c,f,i,l
