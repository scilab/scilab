// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
// <-- CLI SHELL MODE -->

// test des insertions dans les listes
a=1;b=3;c=2;d=[1 2 3];e=[3 4 5];f=[10;20];g=[44 55];
h=5;i=4;a0=10;c0=20;b0=30;g0=[44 55 66; 10 20 30];


l=list();l(0)=a;if l<>list(a) then pause,end
l=list();l(1)=a;if l<>list(a) then pause,end
l=list(a);l(2)=b;if l<>list(a,b) then pause,end
l=list(a);l(0)=b;if l<>list(b,a) then pause,end
l=list(a);l(1)=c;if l<>list(c)  then pause,end
l=list(a);l(1)=d;if l<>list(d)  then pause,end
l=list(d);l(1)=a;if l<>list(a) then pause,end
l=list(a);l(1)=null();if l<>list() then pause,end
l=list(a,c);l(1)=null();if l<>list(c) then pause,end
l=list(a,c);l(2)=null();if l<>list(a) then pause,end
l=list(a,c,b);l(2)=null();if l<>list(a,b) then pause,end
l=list(a,c,b);l(2)=e;if l<>list(a,e,b) then pause,end
l=list(a,e,b);l(2)=a;if l<>list(a,c,b) then pause,end

l=list();l(0)=null();if l<>list() then pause,end
l=list();l(1)=null();if l<>list() then pause,end
//
l=list(a);l(0)=list(c,b);if l<>list(list(c,b),a) then pause,end
l=list(a);l(2)=list(c,b);if l<>list(a,list(c,b)) then pause,end
l=list(a,list(c,b));l(3)=i;if l<>list(a,list(c,b),i) then pause,end
l=list(a,list(c,b),i);l(1)=null();if l<>list(list(c,b),i) then pause,end
l=list(a,list(c,b),i);l(2)=null();if l<>list(a,i) then pause,end
l=list(a,list(c,b),i);l(3)=null();if l<>list(a,list(c,b)) then pause,end
//
l=list(a,list(c,b,i),h);l(2)(2)=a0;if l<>list(a,list(c,a0,i),h) then pause,end
l=list(a,list(c,b,i),h);l(2)(2)=f;if l<>list(a,list(c,f,i),h) then pause,end
l=list(a,list(c,f,i),h);l(2)(2)=b;if l<>list(a,list(c,b,i),h) then pause,end

l=list(a,list(c,b,i),h);l(2)(3)=a0;if l<>list(a,list(c,b,a0),h) then pause,end
l=list(a,list(c,b,i),h);l(2)(3)=f;if l<>list(a,list(c,b,f),h) then pause,end
l=list(a,list(c,b,f),h);l(2)(3)=i;if l<>list(a,list(c,b,i),h) then pause,end

l=list(a,list(c,b),h);l(2)(3)=i;if l<>list(a,list(c,b,i),h) then pause,end
l=list(a,list(c,b),h);l(2)(0)=a;if l<>list(a,list(a,c,b),h) then pause,end
l=list(a,list(c,b,i),h);l(2)(2)=null();if l<>list(a,list(c,i),h) then pause,end
l=list(a,list(c,b,i),h);l(2)(1)=null();if l<>list(a,list(b,i),h) then pause,end
l=list(a,list(c,b,i),h);l(2)(3)=null();if l<>list(a,list(c,b),h) then pause,end
l=list(a,list(c,b),h);l(2)(3)=g;if l<>list(a,list(c,b,g),h) then pause,end
l=list(a,list(c,b),h);l(2)(0)=e;if l<>list(a,list(e,c,b),h) then pause,end

l=list(a,list(c,list(a0,c0,b0),b),h);
l(2)(2)(1)=0;if l<>list(a,list(c,list(0,c0,b0),b),h); then pause,end
l=list(a,list(c,list(a0,c0,b0),b),h);
l(2)(2)(1)=null();if l<>list(a,list(c,list(c0,b0),b),h); then pause,end
l=list(a,list(c,list(a0,c0,b0),b),h);
l(2)(2)(1)=g;if l<>list(a,list(c,list(g,c0,b0),b),h); then pause,end

l=list(a,list(c,list(a0,c0,b0),b),h);
l(2)(2)(0)=g;if l<>list(a,list(c,list(g,a0,c0,b0),b),h); then pause,end

l=list(a,list(c,list(a0,c0,b0),b),h);
l(2)(2)(3)=g;if l<>list(a,list(c,list(a0,c0,g),b),h); then pause,end
l=list(a,list(c,list(a0,c0,b0),b),h);
l(2)(2)(3)=null();if l<>list(a,list(c,list(a0,c0),b),h); then pause,end

l=list(a,list(c,list(a0,c0,b0),b),h);
l(2)(2)(4)=g;if l<>list(a,list(c,list(a0,c0,b0,g),b),h); then pause,end
l=list();l(5)=33;
ll=l;

l=list(a,1/%s);l(2)("num")=33;if l<>list(a,33/%s) then pause,end
l=list(a,1/%s);l(2)("num")=%s+1;if l<>list(a,((%s+1)/%s)) then pause,end
l=list(a,1/%s);l(2)("den")=%s^1+1;if l<>list(a,1/(%s^2+1)) then pause,end
l=list(a,1/%s);l(2)("num")(1)=33;if l<>list(a,33/%s) then pause,end
l=list(a,1/%s);l(2)("num")(1)=%s+1;if l<>list(a,((%s+1)/%s)) then pause,end
l=list(a,1/%s);l(2)("den")(1)=%s^1+1;if l<>list(a,1/(%s^2+1)) then pause,end
l=list(a,1/%s);l(2)("num")(1,1)=33;if l<>list(a,33/%s) then pause,end
l=list(a,1/%s);l(2)("num")(1,1)=%s+1;if l<>list(a,((%s+1)/%s)) then pause,end
l=list(a,1/%s);l(2)("den")(1,1)=%s^1+1;if l<>list(a,1/(%s^2+1)) then pause,end

l=list(a,1/%s,g);l(2)("num")=33;if l<>list(a,33/%s,g) then pause,end
l=list(a,1/%s,g);l(2)("num")=%s+1;if l<>list(a,(%s+1)/%s,g) then pause,end
l=list(a,1/%s,g);l(2)("den")=%s^1+1;if l<>list(a,1/(%s^2+1),g) then pause,end
l=list(a,1/%s,g);l(2)("num")(1)=33;if l<>list(a,33/%s,g) then pause,end
l=list(a,1/%s,g);l(2)("num")(1)=%s+1;if l<>list(a,(%s+1)/%s,g) then pause,end
l=list(a,1/%s,g);l(2)("den")(1)=%s^1+1;if l<>list(a,1/(%s^2+1),g) then pause,end
l=list(a,1/%s,g);l(2)("num")(1,1)=33;if l<>list(a,33/%s,g) then pause,end
l=list(a,1/%s,g);l(2)("num")(1,1)=%s+1;if l<>list(a,(%s+1)/%s,g) then pause,end
l=list(a,1/%s,g);l(2)("den")(1,1)=%s^1+1;if l<>list(a,1/(%s^2+1),g) then pause,end

l=list(a,1/%s,g);l(2)("num")(1,2)=33;l(2)("den")(1,2)=%s+1;
if l<>list(a,[1 33]./[%s %s+1],g) then pause,end


//a=1;b=3;c=2;d=[1 2 3];e=[3 4 5];f=[10;20];g=[44 55];
//h=5;i=4;a0=10;c0=20;b0=30;g0=[44 55 66; 10 20 30];
l=list(a);l(1)(1)=3;x=a;x(1)=3;if l<>list(x) then pause,end
l=list(e);l(1)(2)=[];x=e;x(2)=[];if l<>list(x) then pause,end
l=list(a);l(1)(1,1)=3;x=a;x(1,1)=3;if l<>list(x) then pause,end
l=list(e);l(1)(1,2)=[];x=e;x(1,2)=[];if l<>list(x) then pause,end
l=list(a);l(1)(5,5)=3;x=a;x(5,5)=3;if l<>list(x) then pause,end
l=list(g0);l(1)(2,3)=3;x=g0;x(2,3)=3;if l<>list(x) then pause,end

l=list(b,a);l(2)(1)=3;x=a;x(1)=3;if l<>list(b,x) then pause,end
l=list(b,e);l(2)(2)=[];x=e;x(2)=[];if l<>list(b,x) then pause,end
l=list(b,a);l(2)(1,1)=3;x=a;x(1,1)=3;if l<>list(b,x) then pause,end
l=list(b,e);l(2)(1,2)=[];x=e;x(1,2)=[];if l<>list(b,x) then pause,end
l=list(b,a);l(2)(5,5)=3;x=a;x(5,5)=3;if l<>list(b,x) then pause,end
l=list(b,g0);l(2)(2,3)=3;x=g0;x(2,3)=3;if l<>list(b,x) then pause,end
l=list(b,g0);l(2)(2,2:3)=[1 3];x=g0;x(2,2:3)=[1 3];if l<>list(b,x) then pause,end


a=1:10;b=3:2:33;c=(-10:0)';d=[1 2+%s 3];e=["1";"2";"3";"4"];
f=[%t %t %f];g=["12345","abcdefghijk"];
h=rand(3,3);i=eye(10,10);a0=10+%s;c0=20;b0=sparse(eye(30,30));

l=list();l(0)=a;if l<>list(a) then pause,end
l=list();l(1)=a;if l<>list(a) then pause,end
l=list(a);l(2)=b;if l<>list(a,b) then pause,end
l=list(a);l(0)=b;if l<>list(b,a) then pause,end
l=list(a);l(1)=c;if l<>list(c)  then pause,end
l=list(a);l(1)=d;if l<>list(d)  then pause,end
l=list(d);l(1)=a;if l<>list(a) then pause,end
l=list(a);l(1)=null();if l<>list() then pause,end
l=list(a,c);l(1)=null();if l<>list(c) then pause,end
l=list(a,c);l(2)=null();if l<>list(a) then pause,end
l=list(a,c,b);l(2)=null();if l<>list(a,b) then pause,end
l=list(a,c,b);l(2)=e;if l<>list(a,e,b) then pause,end
l=list(a,e,b);l(2)=a;if l<>list(a,c,b) then pause,end

l=list();l(0)=null();if l<>list() then pause,end
l=list();l(1)=null();if l<>list() then pause,end
//
l=list(a);l(0)=list(c,b);if l<>list(list(c,b),a) then pause,end
l=list(a);l(2)=list(c,b);if l<>list(a,list(c,b)) then pause,end
l=list(a,list(c,b));l(3)=i;if l<>list(a,list(c,b),i) then pause,end
l=list(a,list(c,b),i);l(1)=null();if l<>list(list(c,b),i) then pause,end
l=list(a,list(c,b),i);l(2)=null();if l<>list(a,i) then pause,end
l=list(a,list(c,b),i);l(3)=null();if l<>list(a,list(c,b)) then pause,end
//
l=list(a,list(c,b,i),h);l(2)(2)=a0;if l<>list(a,list(c,a0,i),h) then pause,end
l=list(a,list(c,b,i),h);l(2)(2)=f;if l<>list(a,list(c,f,i),h) then pause,end
l=list(a,list(c,f,i),h);l(2)(2)=b;if l<>list(a,list(c,b,i),h) then pause,end

l=list(a,list(c,b,i),h);l(2)(3)=a0;if l<>list(a,list(c,b,a0),h) then pause,end
l=list(a,list(c,b,i),h);l(2)(3)=f;if l<>list(a,list(c,b,f),h) then pause,end
l=list(a,list(c,b,f),h);l(2)(3)=i;if l<>list(a,list(c,b,i),h) then pause,end

l=list(a,list(c,b),h);l(2)(3)=i;if l<>list(a,list(c,b,i),h) then pause,end
l=list(a,list(c,b),h);l(2)(0)=a;if l<>list(a,list(a,c,b),h) then pause,end
l=list(a,list(c,b,i),h);l(2)(2)=null();if l<>list(a,list(c,i),h) then pause,end
l=list(a,list(c,b,i),h);l(2)(1)=null();if l<>list(a,list(b,i),h) then pause,end
l=list(a,list(c,b,i),h);l(2)(3)=null();if l<>list(a,list(c,b),h) then pause,end
l=list(a,list(c,b),h);l(2)(3)=g;if l<>list(a,list(c,b,g),h) then pause,end
l=list(a,list(c,b),h);l(2)(0)=e;if l<>list(a,list(e,c,b),h) then pause,end

l=list(a,list(c,list(a0,c0,b0),b),h);
l(2)(2)(1)=0;if l<>list(a,list(c,list(0,c0,b0),b),h); then pause,end
l=list(a,list(c,list(a0,c0,b0),b),h);
l(2)(2)(1)=null();if l<>list(a,list(c,list(c0,b0),b),h); then pause,end
l=list(a,list(c,list(a0,c0,b0),b),h);
l(2)(2)(1)=g;if l<>list(a,list(c,list(g,c0,b0),b),h); then pause,end

l=list(a,list(c,list(a0,c0,b0),b),h);
l(2)(2)(0)=g;if l<>list(a,list(c,list(g,a0,c0,b0),b),h); then pause,end

l=list(a,list(c,list(a0,c0,b0),b),h);
l(2)(2)(3)=g;if l<>list(a,list(c,list(a0,c0,g),b),h); then pause,end
l=list(a,list(c,list(a0,c0,b0),b),h);
l(2)(2)(3)=null();if l<>list(a,list(c,list(a0,c0),b),h); then pause,end

l=list(a,list(c,list(a0,c0,b0),b),h);
l(2)(2)(4)=g;if l<>list(a,list(c,list(a0,c0,b0,g),b),h); then pause,end

l=list(a,1/%s);l(2)("num")=33;if l<>list(a,33/%s) then pause,end
l=list(a,1/%s);l(2)("num")=%s+1;if l<>list(a,((%s+1)/%s)) then pause,end
l=list(a,1/%s);l(2)("den")=%s^1+1;if l<>list(a,1/(%s^2+1)) then pause,end
l=list(a,1/%s);l(2)("num")(1)=33;if l<>list(a,33/%s) then pause,end
l=list(a,1/%s);l(2)("num")(1)=%s+1;if l<>list(a,((%s+1)/%s)) then pause,end
l=list(a,1/%s);l(2)("den")(1)=%s^1+1;if l<>list(a,1/(%s^2+1)) then pause,end
l=list(a,1/%s);l(2)("num")(1,1)=33;if l<>list(a,33/%s) then pause,end
l=list(a,1/%s);l(2)("num")(1,1)=%s+1;if l<>list(a,((%s+1)/%s)) then pause,end
l=list(a,1/%s);l(2)("den")(1,1)=%s^1+1;if l<>list(a,1/(%s^2+1)) then pause,end

l=list(a,1/%s,g);l(2)("num")=33;if l<>list(a,33/%s,g) then pause,end
l=list(a,1/%s,g);l(2)("num")=%s+1;if l<>list(a,(%s+1)/%s,g) then pause,end
l=list(a,1/%s,g);l(2)("den")=%s^1+1;if l<>list(a,1/(%s^2+1),g) then pause,end
l=list(a,1/%s,g);l(2)("num")(1)=33;if l<>list(a,33/%s,g) then pause,end
l=list(a,1/%s,g);l(2)("num")(1)=%s+1;if l<>list(a,(%s+1)/%s,g) then pause,end
l=list(a,1/%s,g);l(2)("den")(1)=%s^1+1;if l<>list(a,1/(%s^2+1),g) then pause,end
l=list(a,1/%s,g);l(2)("num")(1,1)=33;if l<>list(a,33/%s,g) then pause,end
l=list(a,1/%s,g);l(2)("num")(1,1)=%s+1;if l<>list(a,(%s+1)/%s,g) then pause,end
l=list(a,1/%s,g);l(2)("den")(1,1)=%s^1+1;if l<>list(a,1/(%s^2+1),g) then pause,end

l=list(a,1/%s,g);l(2)("num")(1,2)=33;l(2)("den")(1,2)=%s+1;
if l<>list(a,[1 33]./[%s %s+1],g) then pause,end

//test with field names not defined in the structure here row1 and row2

function M=%to_e(varargin)
    M = varargin($),
    select varargin(1)
    case "row2" then
        M.N = M.N(2, eye())
        M.V = M.V(2, eye())
    case "row1" then
        M.N = M.N(1, eye())
        M.V = M.V(1, eye())
    else
        M.N = M.N(varargin(1:$ - 1))
        M.V = M.V(varargin(1:$ - 1))
    end
endfunction


%to_6=%to_e;

function M=%to_i_to(varargin)
    M=varargin($);N=varargin($-1)
    select varargin(1)
    case "row2"
        M.N(2,:)=N.N
        M.V(2,:)=N.V
    case "row1"
        M.N(1,:)=N.N
        M.V(1,:)=N.V
    else
        M.N(varargin(1:$-2))=N.N
        M.V(varargin(1:$-2))=N.V
    end
endfunction

M=mlist(["to","V","N"],[1 2 3;4 5 6],["a","b","c";"d","e","f"]);
M.row1(2)=M.row2(1);
assert_checkequal(M.V, [1 4 3;4 5 6]);
assert_checkequal(M.N, ["a","d","c";"d","e","f"]);
l=list(1,M,2);
l(2).row1(2)=M.row1(1);
assert_checkequal(l(1), 1);
assert_checkequal(l(2).V, [1 1 3;4 5 6]);
assert_checkequal(l(2).N, ["a","a","c";"d","e","f"]);
assert_checkequal(M.V, [1 4 3;4 5 6]);
assert_checkequal(M.N, ["a","d","c";"d","e","f"]);
l(1)="foo";
assert_checkequal(l(1), "foo");

//test of insertion in structs (a particular mlist)
clear S;
S.a=11;
S(2).a=12;
assert_checkequal(S.a, list(11,12));

clear S;
S.a=11;
S.a(2,2)=12;
assert_checkequal(S.a, diag([11,12]));

clear S;
S.a=11;
S(2).b=12;
assert_checkequal(S.a, list(11,[]));
assert_checkequal(S.b, list([],12));

clear S S1 S2;
S.a=11;
S(2).b=12;

S1.a=33;
S1.b="toto";
S(1)=S1;
assert_checkequal(S.a, list(33,[]));
assert_checkequal(S.b, list("toto",12));

S(2,2)=S1;
assert_checkequal(S.a, list(33,[],[],33));
assert_checkequal(S.b, list("toto",12,[],"toto"));

S2.a=-5;
S2.c=8;

S(2,2,2)=S2;
assert_checkequal(S.a, list(33,[],[],33,[],[],[],-5));
assert_checkequal(S.b, list("toto",12,[],"toto",[],[],[],[]));
assert_checkequal(S.c, list([],[],[],[],[],[],[],8));

S1=S(:,1,1);
assert_checkequal(S1.a, list(33,[]));
assert_checkequal(S1.b, list("toto",12));
assert_checkequal(S1.c, list([],[]));

S1=S(1:2,1,1);
assert_checkequal(S1.a, list(33,[]));
assert_checkequal(S1.b, list("toto",12));
assert_checkequal(S1.c, list([],[]));

S1=S([1 1],1,1);
assert_checkequal(S1.a, list(33,33));
assert_checkequal(S1.b, list("toto","toto"));
assert_checkequal(S1.c, list([],[]));

S1=S([1 2 1],1,1);
assert_checkequal(S1.a, list(33,[],33));
assert_checkequal(S1.b, list("toto",12,"toto"));
assert_checkequal(S1.c, list([],[],[]));

S1=S(1,:,1);
assert_checkequal(S1.a, list(33,[]));
assert_checkequal(S1.b, list("toto",[]));
assert_checkequal(S1.c, list([],[]));

S1=S(:,:);
assert_checkequal(S1.a, list(33,[],[],33,[],[],[],-5));
assert_checkequal(S1.b, list("toto",12,[],"toto",[],[],[],[]));
assert_checkequal(S1.c, list([],[],[],[],[],[],[],8));

S=struct();
S.a(2).b=1;
assert_checkequal(S.a.b, list([],1));

clear S;
S.a(2).b=1;
assert_checkequal(S.a.b, list([],1));

S=struct();
S(2).a.b=1;
assert_checkequal(S(1).a, []);
assert_checkequal(S(2).a.b, 1);

clear S;
S(2).a.b=1;
assert_checkequal(S(1).a, []);
assert_checkequal(S(2).a.b, 1);

clear S;
S(2).a(3).b=1;
assert_checkequal(S(1).a, []);
assert_checkequal(S(2).a.b, list([],[], 1));

//
S = struct("a", "1");
S(1,1,2).a = "2";
S(1).b = 2;
assert_checkequal(S.a, list("1", "2"));
assert_checkequal(S.b, list(2, []));
S(2).a=5;
assert_checkequal(S.a, list("1", 5));

S(1,3,2).a="foo";
assert_checkequal(S.a, list("1",[],[],5,[],"foo"));
assert_checkequal(S.b, list(2,[],[],[],[],[]));

S(1,3,2,2).b=[33 44];
assert_checkequal(S.a, list("1",[],[],5,[],"foo" ,[],[],[],[],[],[]));
assert_checkequal(S.b, list(2,[],[],[],[],[],[],[],[],[],[],[33,44]));

clear S;S(1,2).a=3;S(3).a=44 ;
assert_checkequal(S.a, list([],3,44));

clear S;S.a=3;S(3).a=44;
assert_checkequal(S.a, list(3,[],44));

clear S;S(4).a=3;
assert_checkequal(S.a, list([],[],[],3));

clear S;
S(1).a(1,1:4)=1;
S(1).b(1:3)=5;
assert_checkequal(S.a, [1,1,1,1]);
assert_checkequal(S.b, [5;5;5]);

S=struct();
S.b(1,1)=3;
assert_checkequal(S.b, 3);

clear S;
S(1,1).a(1,1:4)=1;
S(1,1).b(1,1:3)=5;
assert_checkequal(S.a, [1,1,1,1]);
assert_checkequal(S.b, [5,5,5]);

clear S;
S(1,1).a(1:4)=1;
S(1,1).b(1:3)=5;
assert_checkequal(S.a, [1;1;1;1]);
assert_checkequal(S.b, [5;5;5]);

clear S;
S(1,2).a(1:4)=1;
S(2,1).b(1:3)=5;
assert_checkequal(S.a, list([],[],[1,1,1,1]',[]));
assert_checkequal(S.b, list([],[5,5,5]',[],[]));

clear S;
S(1,2).a(2).b(1,2)=1;
assert_checkequal(S(1).a, []);
assert_checkequal(S(2).a.b, list([],[0,1]));

clear S;
S(1,2).a(1,2).b(1,2,3)=4;
assert_checkequal(S(1).a, []);
assert_checkequal(S(2).a.b, list([], matrix( [0;0;0;0;0;4], [1 2 3])));

tl=tlist(["ltype" "f1"], 12);
assert_checkequal(typeof(tl), "ltype");
tl(1) = 12;
assert_checkequal(typeof(tl), "list");
