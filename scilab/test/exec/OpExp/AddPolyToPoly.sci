//scalar
a = %s + 3 - %i;
b = %s - 8 - %i;
c = a + b;
d = b + a;

//scalar + matrix
e = [%s - 1 + %i, -%s + 2 - %i; %s + 3 + %i, -%s - 4 - %i];
f = %s + 8 - %i;
g = e + f;
h = f + e;

//matrix + matrix
i = [%s - 1 + %i, -%s + 2 - %i; %s + 3 + %i, -%s - 4 - %i];
j = [%s - 1 + %i, -%s + 2 + %i; %s + 3 + %i, -%s - 4 + %i];
k = i + j;

c,d,g,h,k
