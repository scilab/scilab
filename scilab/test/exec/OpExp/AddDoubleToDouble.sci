//scalar
a = 1;
b = %i;
c = a + b;
d = b + a;

//scalar + matrix
e = [1,2;3,4];
f = %i;
g = e + f;
h = f + e;

//matrix + matrix
i = [1,2;3,4];
j = [%i,-%i;%i,-%i];
k = i + j;
l = j + i;


c,d,g,h,k,l
