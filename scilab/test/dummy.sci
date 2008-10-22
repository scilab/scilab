/*
a = 1
b = %i
c = a + b
cc = a - b
d = 4*a
e = 4*b
f = 2*a + 3*b
ff = 2*a - 3*b

l = "re" + "toto"
m = ["je", "tu", "il"; "nous", "vous", "ils"]
n = ["e", "es", "e"; "eons", "ez", "ent"]
o = m + " mang" + n
a = [1 2;2 1]
b = [1 2;1 2]

*/
/* Error cases
j = [1 + "toto",4]
k = [1 ; 2  3]

if "toto" then
disp true
else
disp false
end

*/
/*
a = [1 2;3 4]
b = [2 1;4 3]
c = [2 3;4 5]

if a <> b then
	if c < a then
		d = "true true"
	else
		d = "true false"
	end
else
	if a < c then
		d = "true true"
	else
		d = "true false"
	end
end

a = 1;
b = 2;
*/

a = 3
for i=1:10 
	a = i
end

b = 1 + %i : 4 + 10*%i