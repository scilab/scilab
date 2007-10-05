// Copyright INRIA
//poly (coeff)

v=[1 2 3];nam='s';job='c';p=1+2*%s+3*%s^2;
if poly(v,'s','c')<>p then pause,end
if poly(v+0,'s','c')<>p then pause,end
if poly(v,nam,'c')<>p then pause,end
if poly(v+0,nam,'c')<>p then pause,end
if poly(v,'s',job)<>p then pause,end
if poly(v+0,'s',job)<>p then pause,end
if poly(v,nam,job)<>p then pause,end
if poly(v+0,nam,job)<>p then pause,end
v=[1+%i 2 3];p=1+%i+2*%s+3*%s^2;
if poly(v,'s','c')<>p then pause,end
if poly(v+0,'s','c')<>p then pause,end
if poly(v,nam,'c')<>p then pause,end
if poly(v+0,nam,'c')<>p then pause,end
if poly(v,'s',job)<>p then pause,end
if poly(v+0,'s',job)<>p then pause,end
if poly(v,nam,job)<>p then pause,end
if poly(v+0,nam,job)<>p then pause,end

//poly (roots)
v=[1 2 3];nam='s';job='r';p=-6+11*%s-6*%s^2+%s^3;
if poly(v,'s','r')<>p then pause,end
if poly(v+0,'s','r')<>p then pause,end
if poly(v,nam,'r')<>p then pause,end
if poly(v+0,nam,'r')<>p then pause,end
if poly(v,'s',job)<>p then pause,end
if poly(v+0,'s',job)<>p then pause,end
if poly(v,nam,job)<>p then pause,end
if poly(v+0,nam,job)<>p then pause,end
v=[1+%i 2 3];p=-6-%i*6+(11+%i*5)*%s+(-6-%i)*%s^2+%s^3;
if poly(v,'s','r')<>p then pause,end
if poly(v+0,'s','r')<>p then pause,end
if poly(v,nam,'r')<>p then pause,end
if poly(v+0,nam,'r')<>p then pause,end
if poly(v,'s',job)<>p then pause,end
if poly(v+0,'s',job)<>p then pause,end
if poly(v,nam,job)<>p then pause,end
if poly(v+0,nam,job)<>p then pause,end

v=[];p=poly(1,'s','c');
if poly(v,'s','r')<>p then pause,end
if poly([],'s','r')<>p then pause,end
if poly(v,nam,'r')<>p then pause,end
if poly([],nam,'r')<>p then pause,end
if poly(v,'s',job)<>p then pause,end
if poly([],'s',job)<>p then pause,end
if poly(v,nam,job)<>p then pause,end
if poly([],nam,job)<>p then pause,end

//poly (caracteristic)
nam='s';a=[1 2;3 4];p=-2-5*%s+%s^2;
if norm(coeff(poly(a,'s')-p))>100*%eps then pause,end
if norm(coeff(poly(a+0,'s')-p))>100*%eps then pause,end
if norm(coeff(poly(a,nam)-p))>100*%eps then pause,end
if norm(coeff(poly(a+0,nam)-p))>100*%eps then pause,end
a=[1+%i 2;3 4];p=-2+%i*4+(-5-%i)*%s+%s^2 ;
if norm(coeff(poly(a,'s')-p))>100*%eps then pause,end
if norm(coeff(poly(a+0,'s')-p))>100*%eps then pause,end
if norm(coeff(poly(a,nam)-p))>100*%eps then pause,end
if norm(coeff(poly(a+0,nam)-p))>100*%eps then pause,end
a=[];p=poly(1,'s','c');
if poly(a,'s')<>p then pause,end
if poly([],'s')<>p then pause,end
if poly(a,nam)<>p then pause,end
if poly([],nam)<>p then pause,end

//roots
p=-6+11*%s-6*%s^2+%s^3;
if norm(coeff(poly(roots(p),'s','r')-p))>400*%eps then pause,end
if norm(coeff(poly(roots(p+0),'s','r')-p))>400*%eps then pause,end
p=-6-%i*6+(11+%i*5)*%s+(-6-%i)*%s^2+%s^3;
if norm(coeff(poly(roots(p),'s','r')-p))>400*%eps then pause,end
if norm(coeff(poly(roots(p+0),'s','r')-p))>400*%eps then pause,end
p=1;v=[];
if or(roots(p)<>v) then pause,end
if or(roots(p+0)<>v) then pause,end

//degree
v=[1 2 3];d=[0 0 0];
if or(degree(v)<>d) then pause,end
if or(degree(v+0)<>d) then pause,end
v=[-2-5*%s+%s^2;-6+11*%s-6*%s^2+%s^3];d=[2;3];
if or(degree(v)<>d) then pause,end
if or(degree(v+0)<>d) then pause,end
v=[];d=[];
if or(degree(v)<>d) then pause,end
if or(degree([])<>d) then pause,end

//coeff
p=[1+2*%s;%s^3];c=[1 2 0 0;0 0 0 1];sel=[0 1 1];

if or(coeff(p)<>c) then pause,end
if or(coeff(p+0)<>c) then pause,end
if or(coeff(p,sel)<>c(:,sel+1)) then pause,end
if or(coeff(p+0,sel)<>c(:,sel+1)) then pause,end
if or(coeff(p,sel+0)<>c(:,sel+1)) then pause,end
if or(coeff(p+0,sel+0)<>c(:,sel+1)) then pause,end
sel=5;
if or(coeff(p,sel+0)<>zeros(size(p,1),1)) then pause,end
p=[1+2*%s, %s^3];c=[1 0 2 0 0 0 0 1];
if or(coeff(p)<>c) then pause,end
if or(coeff(p+0)<>c) then pause,end
sel=[0 0];c=[1 0 1 0];
if or(coeff(p,sel)<>c) then pause,end
if or(coeff(p+0,sel)<>c) then pause,end
if or(coeff(p,sel+0)<>c) then pause,end
if or(coeff(p+0,sel+0)<>c) then pause,end

p=[1+%i+2*%s;%s^3];c=[1+%i 2 0 0;0 0 0 1];sel=[0 1 1];

if or(coeff(p)<>c) then pause,end
if or(coeff(p+0)<>c) then pause,end
if or(coeff(p,sel)<>c(:,sel+1)) then pause,end
if or(coeff(p+0,sel)<>c(:,sel+1)) then pause,end
if or(coeff(p,sel+0)<>c(:,sel+1)) then pause,end
if or(coeff(p+0,sel+0)<>c(:,sel+1)) then pause,end
sel=5;
if or(coeff(p,sel+0)<>zeros(size(p,1),1)) then pause,end
p=[1+2*%s, %s^3];c=[1 0 2 0 0 0 0 1];
if or(coeff(p)<>c) then pause,end
if or(coeff(p+0)<>c) then pause,end
sel=[0 0];c=[1 0 1 0];
if or(coeff(p,sel)<>c) then pause,end
if or(coeff(p+0,sel)<>c) then pause,end
if or(coeff(p,sel+0)<>c) then pause,end
if or(coeff(p+0,sel+0)<>c) then pause,end

p=[1 2;3 4];sel=0;c=p;
if or(coeff(p)<>c) then pause,end
if or(coeff(p+0)<>c) then pause,end
if or(coeff(p,sel)<>c) then pause,end
if or(coeff(p+0,sel)<>c) then pause,end
if or(coeff(p,sel+0)<>c) then pause,end
if or(coeff(p+0,sel+0)<>c) then pause,end
p=[1 2;3 4];sel=[0 0];c=[p p];
if or(coeff(p,sel)<>c) then pause,end
if or(coeff(p+0,sel)<>c) then pause,end
if or(coeff(p,sel+0)<>c) then pause,end
if or(coeff(p+0,sel+0)<>c) then pause,end
p=[1 2;3 4];sel=[0 1];c=[p 0*p];
if or(coeff(p,sel)<>c) then pause,end
if or(coeff(p+0,sel)<>c) then pause,end
if or(coeff(p,sel+0)<>c) then pause,end
if or(coeff(p+0,sel+0)<>c) then pause,end

p=[1 2;3 4];sel=[];c=[];
if or(coeff(p,sel)<>c) then pause,end
if or(coeff(p+0,sel)<>c) then pause,end
if or(coeff(p,[])<>c) then pause,end
if or(coeff(p+0,[])<>c) then pause,end

p=[],sel=0,c=[];
if or(coeff(p)<>c) then pause,end
if or(coeff([])<>c) then pause,end
if or(coeff(p,sel)<>c) then pause,end
if or(coeff([],sel)<>c) then pause,end

//sum 
a=[1+%s 2;-3 4;5 -6];
if sum(a)<>3+%s then pause,end
if sum(a+0)<>3+%s then pause,end
if or(sum(a,1)<>[3+%s 0]) then pause,end
if or(sum(a+0,1)<>[3+%s 0]) then pause,end
n=1;
if or(sum(a,n)<>[3+%s 0]) then pause,end
if or(sum(a+0,n)<>[3+%s 0]) then pause,end
if or(sum(a,'r')<>[3+%s 0]) then pause,end
if or(sum(a+0,'r')<>[3+%s 0]) then pause,end
n='r';
if or(sum(a,n)<>[3+%s 0]) then pause,end
if or(sum(a+0,n)<>[3+%s 0]) then pause,end

if or(sum(a,2)<>[3+%s;1;-1]) then pause,end
if or(sum(a+0,2)<>[3+%s;1;-1]) then pause,end
n=2;
if or(sum(a,n)<>[3+%s;1;-1]) then pause,end
if or(sum(a+0,n)<>[3+%s;1;-1]) then pause,end
if or(sum(a,'c')<>[3+%s;1;-1]) then pause,end
if or(sum(a+0,'c')<>[3+%s;1;-1]) then pause,end
n='c';
if or(sum(a,n)<>[3+%s;1;-1]) then pause,end
if or(sum(a+0,n)<>[3+%s;1;-1]) then pause,end

a=[1+%s 2;-3 4;5 -6]+0*%i;
if or(sum(a)<>3+%s+0*%i) then pause,end
if or(sum(a+0)<>3+%s+0*%i) then pause,end
if or(sum(a,1)<>[3+%s 0]+0*%i) then pause,end
if or(sum(a+0,1)<>[3+%s 0]+0*%i) then pause,end
n=1;
if or(sum(a,n)<>[3+%s 0]+0*%i) then pause,end
if or(sum(a+0,n)<>[3+%s 0]+0*%i) then pause,end
if or(sum(a,'r')<>[3+%s 0]+0*%i) then pause,end
if or(sum(a+0,'r')<>[3+%s 0]+0*%i) then pause,end
n='r';
if or(sum(a,n)<>[3+%s 0]+0*%i) then pause,end
if or(sum(a+0,n)<>[3+%s 0]+0*%i) then pause,end

if or(sum(a,2)<>[3+%s;1;-1]+0*%i) then pause,end
if or(sum(a+0,2)<>[3+%s;1;-1]+0*%i) then pause,end
n=2;
if or(sum(a,n)<>[3+%s;1;-1]+0*%i) then pause,end
if or(sum(a+0,n)<>[3+%s;1;-1]+0*%i) then pause,end
if or(sum(a,'c')<>[3+%s;1;-1]+0*%i) then pause,end
if or(sum(a+0,'c')<>[3+%s;1;-1]+0*%i) then pause,end
n='c';
if or(sum(a,n)<>[3+%s;1;-1]+0*%i) then pause,end
if or(sum(a+0,n)<>[3+%s;1;-1]+0*%i) then pause,end

//prod
a=[1+%s 2;-3 4;5 -6];
if prod(a)<>720+720*%s then pause,end
if prod(a+0)<>720+720*%s then pause,end
if or(prod(a,1)<>[-15-15*%s,-48]) then pause,end
if or(prod(a+0,1)<>[-15-15*%s,-48]) then pause,end
n=1;
if or(prod(a,n)<>[-15-15*%s,-48]) then pause,end
if or(prod(a+0,n)<>[-15-15*%s,-48]) then pause,end
if or(prod(a,'r')<>[-15-15*%s,-48]) then pause,end
if or(prod(a+0,'r')<>[-15-15*%s,-48]) then pause,end
n='r';
if or(prod(a,n)<>[-15-15*%s,-48]) then pause,end
if or(prod(a+0,n)<>[-15-15*%s,-48]) then pause,end

if or(prod(a,2)<>[2+2*%s;-12;-30]) then pause,end
if or(prod(a+0,2)<>[2+2*%s;-12;-30]) then pause,end
n=2;
if or(prod(a,n)<>[2+2*%s;-12;-30]) then pause,end
if or(prod(a+0,n)<>[2+2*%s;-12;-30]) then pause,end
if or(prod(a,'c')<>[2+2*%s;-12;-30]) then pause,end
if or(prod(a+0,'c')<>[2+2*%s;-12;-30]) then pause,end
n='c';
if or(prod(a,n)<>[2+2*%s;-12;-30]) then pause,end
if or(prod(a+0,n)<>[2+2*%s;-12;-30]) then pause,end

a=[1+%s 2;-3 4;5 -6]+0*%i;
if or(prod(a)<>720+720*%s+0*%i) then pause,end
if or(prod(a+0)<>720+720*%s+0*%i) then pause,end
if or(prod(a,1)<>[-15-15*%s,-48]+0*%i) then pause,end
if or(prod(a+0,1)<>[-15-15*%s,-48]+0*%i) then pause,end
n=1;
if or(prod(a,n)<>[-15-15*%s,-48]+0*%i) then pause,end
if or(prod(a+0,n)<>[-15-15*%s,-48]+0*%i) then pause,end
if or(prod(a,'r')<>[-15-15*%s,-48]+0*%i) then pause,end
if or(prod(a+0,'r')<>[-15-15*%s,-48]+0*%i) then pause,end
n='r';
if or(prod(a,n)<>[-15-15*%s,-48]+0*%i) then pause,end
if or(prod(a+0,n)<>[-15-15*%s,-48]+0*%i) then pause,end

if or(prod(a,2)<>[2+2*%s;-12;-30]+0*%i) then pause,end
if or(prod(a+0,2)<>[2+2*%s;-12;-30]+0*%i) then pause,end
n=2;
if or(prod(a,n)<>[2+2*%s;-12;-30]+0*%i) then pause,end
if or(prod(a+0,n)<>[2+2*%s;-12;-30]+0*%i) then pause,end
if or(prod(a,'c')<>[2+2*%s;-12;-30]+0*%i) then pause,end
if or(prod(a+0,'c')<>[2+2*%s;-12;-30]+0*%i) then pause,end
n='c';
if or(prod(a,n)<>[2+2*%s;-12;-30]+0*%i) then pause,end
if or(prod(a+0,n)<>[2+2*%s;-12;-30]+0*%i) then pause,end

//diag
a=[1+%s 2 3];
if or(diag(a+0)<>[1+%s 0 0;0 2 0;0 0 3]) then pause,end
if or(diag(a)<>[1+%s 0 0;0 2 0;0 0 3]) then pause,end

if or(diag([1+%s %i 2])<>[1+%s 0 0;0 %i 0;0 0 2]) then pause,end
a=[1+%s %i 2 ];
if or(diag(a)<>[1+%s 0 0;0 %i 0;0 0 2]) then pause,end
a=[1+%s 2 3 4;5 6 7 8];un=poly(1,'s','c');
if or(diag(a)<>[1+%s;6]) then pause,end
if or(diag(a+0)<>[1+%s;6]) then pause,end
if or(diag(a,1)<>[2*un;7]) then pause,end
if or(diag(a+0,1)<>[2*un;7]) then pause,end
if or(diag(a,-1)<>[5*un]) then pause,end
if or(diag(a+0,-1)<>[5*un]) then pause,end
if diag(a,4)<>[] then pause,end
a(1,1)=%i+%s;
if or(diag(a)<>[%i+%s;6]) then pause,end
if or(diag(a+0)<>[%i+%s;6]) then pause,end
if or(diag(a,1)<>[2*un;7]) then pause,end
if or(diag(a+0,1)<>[2*un;7]) then pause,end
if or(diag(a,-1)<>[5*un]) then pause,end
if or(diag(a+0,-1)<>[5*un]) then pause,end
if diag(a,4)<>[] then pause,end

//triu
a=[1+%s 2 3 4;5 6 7 8];un=poly(1,'s','c');
if or(triu(a)<>[1+%s 2 3 4;0 6 7 8]) then pause,end
if or(triu(a+0)<>[1+%s 2 3 4;0 6 7 8]) then pause,end
if or(triu(a,1)<>[0 2*un 3 4;0 0 7 8]) then pause,end
if or(triu(a+0,1)<>[0 2*un 3 4;0 0 7 8]) then pause,end
if or(triu(a,-1)<>a) then pause,end
if or(triu(a+0,-1)<>a) then pause,end
if or(triu(a,4)<>0*a) then pause,end
a(1,1)=%i+%s;
if or(triu(a)<>[%i+%s 2 3 4;0 6 7 8]) then pause,end
if or(triu(a+0)<>[%i+%s 2 3 4;0 6 7 8]) then pause,end
if or(triu(a,1)<>[0 2*un 3 4;0 0 7 8]) then pause,end
if or(triu(a+0,1)<>[0 2*un 3 4;0 0 7 8]) then pause,end
if or(triu(a,-1)<>a) then pause,end
if or(triu(a+0,-1)<>a) then pause,end
if or(triu(a,4)<>0*a) then pause,end

//tril
a=[1+%s 2 3 4;5 6 7 8];
if or(tril(a)<>[1+%s 0 0 0;5 6 0 0]) then pause,end
if or(tril(a+0)<>[1+%s 0 0 0;5 6 0 0]) then pause,end
if or(tril(a,1)<>[1+%s 2 0 0;5 6  7 0]) then pause,end
if or(tril(a+0,1)<>[1+%s 2 0 0;5 6  7 0]) then pause,end
if or(tril(a,4)<>a) then pause,end
if or(tril(a+0,4)<>a) then pause,end
if or(tril(a,-3)<>0*a) then pause,end
a(1,1)=%i+%s;
if or(tril(a)<>[%i+%s 0 0 0;5 6 0 0]) then pause,end
if or(tril(a+0)<>[%i+%s 0 0 0;5 6 0 0]) then pause,end
if or(tril(a,1)<>[%i+%s 2 0 0;5 6  7 0]) then pause,end
if or(tril(a+0,1)<>[%i+%s 2 0 0;5 6  7 0]) then pause,end
if or(tril(a,4)<>a) then pause,end
if or(tril(a+0,4)<>a) then pause,end
if or(tril(a,-3)<>0*a) then pause,end


//simp_mode
a=%t
simp_mode(%t);
if simp_mode()<>%t then pause,end
simp_mode(a);
if simp_mode()<>%t then pause,end
[n,d]=simp(%s,%s);if n<>poly(1,'s','c')|d<>poly(1,'s','c') then pause,end
a=%f
simp_mode(%f);
if simp_mode()<>%f then pause,end
simp_mode(a);
if simp_mode()<>%f then pause,end
[n,d]=simp(%s,%s);if n<>%s|d<>%s then pause,end

//varn
a=1+%s;
if varn(a)<>'s' then pause,end
if varn(a+0)<>'s' then pause,end
if varn(a,'z')<>1+%z then pause,end
if varn(a+0,'z')<>1+%z then pause,end
nam='z';
if varn(a,nam)<>1+%z then pause,end
if varn(a+0,nam)<>1+%z then pause,end
a=[];
if varn(a,nam)<>[] then pause,end
if varn([],nam)<>[] then pause,end
//clean
a=[1 1.d-12 1.d-5 2d8+%s];un=poly(1,'s','c')
b=[1 0 1.d-5 2d8+%s];
if or(clean(a)<>b) then pause,end
if or(clean(a+0)<>b) then pause,end
epsa=1.d-10;
if or(clean(a,epsa)<>b) then pause,end
if or(clean(a+0,epsa)<>b) then pause,end
if or(clean(a,epsa+0)<>b) then pause,end
if or(clean(a+0,epsa+0)<>b) then pause,end
epsr=1.d-4;b=[1 0 1.d-5 2d8*un];
if or(clean(a,epsa,epsr)<>b) then pause,end
if or(clean(a+0,epsa,epsr)<>b) then pause,end
if or(clean(a,epsa+0,epsr)<>b) then pause,end
if or(clean(a+0,epsa,epsr+0)<>b) then pause,end
if or(clean(a,epsa+0,epsr)<>b) then pause,end
if or(clean(a+0,epsa,epsr+0)<>b) then pause,end
if or(clean(a,epsa+0,epsr)<>b) then pause,end
if or(clean(a+0,epsa+0,epsr+0)<>b) then pause,end

a=[1+%i 1.d-12 1.d-5 2d8+%s];
b=[1+%i 0 1.d-5 2d8+%s];
if or(clean(a)<>b) then pause,end
if or(clean(a+0)<>b) then pause,end
epsa=1.d-10;
if or(clean(a,epsa)<>b) then pause,end
if or(clean(a+0,epsa)<>b) then pause,end
if or(clean(a,epsa+0)<>b) then pause,end
if or(clean(a+0,epsa+0)<>b) then pause,end
epsr=1.d-5;b=[1+%i 0 1.d-5 poly([2d8,0],'s','c')];
if or(clean(a,epsa,epsr)<>b) then pause,end
if or(clean(a+0,epsa,epsr)<>b) then pause,end
if or(clean(a,epsa+0,epsr)<>b) then pause,end
if or(clean(a+0,epsa,epsr+0)<>b) then pause,end
if or(clean(a,epsa+0,epsr)<>b) then pause,end
if or(clean(a+0,epsa,epsr+0)<>b) then pause,end
if or(clean(a,epsa+0,epsr)<>b) then pause,end
if or(clean(a+0,epsa+0,epsr+0)<>b) then pause,end

//simp
simp_mode(%t);
p=[];
[n,d]=simp(p,p);if n<>[]|p<>[] then pause,end
[n,d]=simp([],[]);if n<>[]|p<>[] then pause,end

un=poly(1,'s','c');p=%s;
[n,d]=simp(p,p);if n<>un|d<>un then pause,end
[n,d]=simp(p+0,p);if n<>un|d<>un then pause,end
[n,d]=simp(p,p+0);if n<>un|d<>un then pause,end
[n,d]=simp(p+0,p+0);if n<>un|d<>un then pause,end
un=1;
[n,d]=simp(p,un);if n<>p|d<>1 then pause,end
[n,d]=simp(p,1);if n<>p|d<>1 then pause,end
[n,d]=simp(p+0,un);if n<>p|d<>1 then pause,end
[n,d]=simp(p+0,1);if n<>p|d<>1 then pause,end

[n,d]=simp(un,p);if n<>un|d<>p then pause,end
[n,d]=simp(1,p);if n<>un|d<>p then pause,end
[n,d]=simp(un,p+0);if n<>un|d<>p then pause,end
[n,d]=simp(1,p+0);if n<>un|d<>p then pause,end


p=1+%s^10;un=poly(1,'s','c')

[n,d]=simp(p,p);if n<>un|d<>un then pause,end
[n,d]=simp(p+0,p);if n<>un|d<>un then pause,end
[n,d]=simp(p,p+0);if n<>un|d<>un then pause,end
[n,d]=simp(p+0,p+0);if n<>un|d<>un then pause,end
un=1;
[n,d]=simp(p,un);if n<>p|d<>1 then pause,end
[n,d]=simp(p,1);if n<>p|d<>1 then pause,end
[n,d]=simp(p+0,un);if n<>p|d<>1 then pause,end
[n,d]=simp(p+0,1);if n<>p|d<>1 then pause,end

[n,d]=simp(un,p);if n<>un|d<>p then pause,end
[n,d]=simp(1,p);if n<>un|d<>p then pause,end
[n,d]=simp(un,p+0);if n<>un|d<>p then pause,end
[n,d]=simp(1,p+0);if n<>un|d<>p then pause,end

un=poly(1,'s','c');p=%s;
h=rlist(p,p,[]);un=rlist(poly(1,'s','c'),poly(1,'s','c'),[]);

if simp(h)<>un then pause,end
if simp(rlist(p,p,[]))<>un then pause,end

h=rlist(p,1,[]);
if simp(h)<>h then pause,end
if simp(rlist(p,1,[]))<>h then pause,end

h=rlist(1,p,[]);
if simp(h)<>h then pause,end
if simp(rlist(1,p,[]))<>h then pause,end

p=1+%s^10;
h=rlist(p,p,[]);un=rlist(poly(1,'s','c'),poly(1,'s','c'),[]);

if simp(h)<>un then pause,end
if simp(rlist(p,p,[]))<>un then pause,end

h=rlist(p,1,[]);
if simp(h)<>h then pause,end
if simp(rlist(p,1,[]))<>h then pause,end

h=rlist(1,p,[]);
if simp(h)<>h then pause,end
if simp(rlist(1,p,[]))<>h then pause,end

//pdiv
p=[];
[n,d]=pdiv(p,p);if n<>[]|p<>[] then pause,end
[n,d]=pdiv([],[]);if n<>[]|p<>[] then pause,end
if pdiv(p,p)<>[] then pause,end
if pdiv([],[])<>[] then pause,end

p1=%s;p2=p1;
[r,q]=pdiv(p1,p2);if p2*q+r<>p1 then pause,end
[r,q]=pdiv(p1+0,p2);if p2*q+r<>p1 then pause,end
[r,q]=pdiv(p1,p2+0);if p2*q+r<>p1 then pause,end
[r,q]=pdiv(p1+0,p2+0);if p2*q+r<>p1 then pause,end

p1=%s;p2=1;
[r,q]=pdiv(p1,p2);if p2*q+r<>p1 then pause,end
[r,q]=pdiv(p1+0,p2);if p2*q+r<>p1 then pause,end
[r,q]=pdiv(p1,p2+0);if p2*q+r<>p1 then pause,end
[r,q]=pdiv(p1+0,p2+0);if p2*q+r<>p1 then pause,end

p1=1;p2=%s;
[r,q]=pdiv(p1,p2);if p2*q+r<>p1 then pause,end
[r,q]=pdiv(p1+0,p2);if p2*q+r<>p1 then pause,end
[r,q]=pdiv(p1,p2+0);if p2*q+r<>p1 then pause,end
[r,q]=pdiv(p1+0,p2+0);if p2*q+r<>p1 then pause,end

p1=%s+%i;p2=p1;
[r,q]=pdiv(p1,p2);if p2*q+r<>p1 then pause,end
[r,q]=pdiv(p1+0,p2);if p2*q+r<>p1 then pause,end
[r,q]=pdiv(p1,p2+0);if p2*q+r<>p1 then pause,end
[r,q]=pdiv(p1+0,p2+0);if p2*q+r<>p1 then pause,end

p1=%s;p2=1+%i;
[r,q]=pdiv(p1,p2);if p2*q+r<>p1 then pause,end
[r,q]=pdiv(p1+0,p2);if p2*q+r<>p1 then pause,end
[r,q]=pdiv(p1,p2+0);if p2*q+r<>p1 then pause,end
[r,q]=pdiv(p1+0,p2+0);if p2*q+r<>p1 then pause,end

p1=1+%i;p2=%s;
[r,q]=pdiv(p1,p2);if p2*q+r<>p1 then pause,end
[r,q]=pdiv(p1+0,p2);if p2*q+r<>p1 then pause,end
[r,q]=pdiv(p1,p2+0);if p2*q+r<>p1 then pause,end
[r,q]=pdiv(p1+0,p2+0);if p2*q+r<>p1 then pause,end

//
p1=%s;p2=p1;
[r,q]=pdiv(p1,p2);if pdiv(p1,p2)<>q then pause,end
[r,q]=pdiv(p1+0,p2);if pdiv(p1,p2)<>q then pause,end
[r,q]=pdiv(p1,p2+0);if pdiv(p1,p2)<>q then pause,end
[r,q]=pdiv(p1+0,p2+0);if pdiv(p1,p2)<>q then pause,end

p1=%s;p2=1;
[r,q]=pdiv(p1,p2);if pdiv(p1,p2)<>q then pause,end
[r,q]=pdiv(p1+0,p2);if pdiv(p1,p2)<>q then pause,end
[r,q]=pdiv(p1,p2+0);if pdiv(p1,p2)<>q then pause,end
[r,q]=pdiv(p1+0,p2+0);if pdiv(p1,p2)<>q then pause,end

p1=1;p2=%s;
[r,q]=pdiv(p1,p2);if pdiv(p1,p2)<>q then pause,end
[r,q]=pdiv(p1+0,p2);if pdiv(p1,p2)<>q then pause,end
[r,q]=pdiv(p1,p2+0);if pdiv(p1,p2)<>q then pause,end
[r,q]=pdiv(p1+0,p2+0);if pdiv(p1,p2)<>q then pause,end

p1=%s+%i;p2=p1;
[r,q]=pdiv(p1,p2);if pdiv(p1,p2)<>q then pause,end
[r,q]=pdiv(p1+0,p2);if pdiv(p1,p2)<>q then pause,end
[r,q]=pdiv(p1,p2+0);if pdiv(p1,p2)<>q then pause,end
[r,q]=pdiv(p1+0,p2+0);if pdiv(p1,p2)<>q then pause,end

p1=%s;p2=1+%i;
[r,q]=pdiv(p1,p2);if pdiv(p1,p2)<>q then pause,end
[r,q]=pdiv(p1+0,p2);if pdiv(p1,p2)<>q then pause,end
[r,q]=pdiv(p1,p2+0);if pdiv(p1,p2)<>q then pause,end
[r,q]=pdiv(p1+0,p2+0);if pdiv(p1,p2)<>q then pause,end

p1=1+%i;p2=%s;
[r,q]=pdiv(p1,p2);if pdiv(p1,p2)<>q then pause,end
[r,q]=pdiv(p1+0,p2);if pdiv(p1,p2)<>q then pause,end
[r,q]=pdiv(p1,p2+0);if pdiv(p1,p2)<>q then pause,end
[r,q]=pdiv(p1+0,p2+0);if pdiv(p1,p2)<>q then pause,end




//bezout
p1=%s;p2=p1;
[p,U]=bezout(p1,p2);if norm(coeff([p1 p2]*U-[p 0]))>10*%eps  then pause,end
[p,U]=bezout(p1+0,p2);if norm(coeff([p1 p2]*U-[p 0]))>10*%eps  then pause,end
[p,U]=bezout(p1,p2+0);if norm(coeff([p1 p2]*U-[p 0]))>10*%eps  then pause,end
[p,U]=bezout(p1+0,p2+0);if norm(coeff([p1 p2]*U-[p 0]))>10*%eps  then pause,end

p1=%s;p2=1;
[p,U]=bezout(p1,p2);if norm(coeff([p1 p2]*U-[p 0]))>10*%eps  then pause,end
[p,U]=bezout(p1+0,p2);if norm(coeff([p1 p2]*U-[p 0]))>10*%eps  then pause,end
[p,U]=bezout(p1,p2+0);if norm(coeff([p1 p2]*U-[p 0]))>10*%eps  then pause,end
[p,U]=bezout(p1+0,p2+0);if norm(coeff([p1 p2]*U-[p 0]))>10*%eps  then pause,end

p1=1;p2=%s;
[p,U]=bezout(p1,p2);if norm(coeff([p1 p2]*U-[p 0]))>10*%eps  then pause,end
[p,U]=bezout(p1+0,p2);if norm(coeff([p1 p2]*U-[p 0]))>10*%eps  then pause,end
[p,U]=bezout(p1,p2+0);if norm(coeff([p1 p2]*U-[p 0]))>10*%eps  then pause,end
[p,U]=bezout(p1+0,p2+0);if norm(coeff([p1 p2]*U-[p 0]))>10*%eps  then pause,end

p1=1;p2=1;
[p,U]=bezout(p1,p2);if norm(coeff([p1 p2]*U-[p 0]))>10*%eps  then pause,end
[p,U]=bezout(p1+0,p2);if norm(coeff([p1 p2]*U-[p 0]))>10*%eps  then pause,end
[p,U]=bezout(p1,p2+0);if norm(coeff([p1 p2]*U-[p 0]))>10*%eps  then pause,end
[p,U]=bezout(p1+0,p2+0);if norm(coeff([p1 p2]*U-[p 0]))>10*%eps  then pause,end


//sfact
p=(%s-1/2)*(2-%s);
w=sfact(p); if norm(coeff(w*numer(horner(w,1/%s))-p))>10*%eps  then pause,end
w=sfact(p+0);if norm(coeff(w*numer(horner(w,1/%s))-p))>10*%eps  then pause,end


F1=[%z-1/2,%z+1/2,%z^2+2;1,%z,-%z;%z^3+2*%z,%z,1/2-%z];
P=F1*gtild(F1,'d');
F=sfact(P);if norm(coeff(P-F*gtild(F,'d')))>100*%eps  then pause,end
F=sfact(P+0);if norm(coeff(P-F*gtild(F,'d')))>100*%eps  then pause,end
