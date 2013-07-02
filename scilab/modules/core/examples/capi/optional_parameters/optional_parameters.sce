// Example with optional argument specified with the 'arg=value syntax'
// [a,b,c] = ex12c(x1, [v1 = arg1, v2 = arg2]), arg1 default value 99
//					        arg2 default value 3
// only v1 and v2 are recognized as optional argument names
// the return value are a = x1, b = 2*v2, c = 3*v2

[a,b,c] = optional_parameters("test");
disp("a = " + a + " b = " + string(b) + " c = " + string(c));

[a,b,c] = optional_parameters("test",v1=[10,20]);
disp("a = " + a + " b = " + string(b) + " c = " + string(c));

[a,b,c] = optional_parameters("test",v1=[10,20],v2=8);
disp("a = " + a + " b = " + string(b) + " c = " + string(c));

[a,b,c] = optional_parameters("test",v2=8,v1=[10]);
disp("a = " + a + " b = " + string(b) + " c = " + string(c));
