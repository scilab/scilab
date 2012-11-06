// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
// Copyright (C) 2008-2011 - DIGITEO
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
// <-- CLI SHELL MODE -->
//

//insertion
v=[1 2 3];
v(2)=3;if or(v<>[1 3 3]) then pause,end
v(4)=-5;if or(v<>[1 3 3 -5]) then pause,end
v([1 4])=[2 5];if or(v<>[2 3 3 5]) then pause,end
v([1 1])=[2 5];if or(v<>[5 3 3 5]) then pause,end
v([1 1 1 1 1 1])=[2 5 6 7 8 9];if or(v<>[9 3 3 5]) then pause,end
v([4 2])=[8 5];if or(v<>[9 5 3 8]) then pause,end

v=[1 2 3];
v(2,4)=10;if or(v<>[1 2 3 0;0 0 0 10]) then pause,end
v([2 2],4)=[10;20];if or(v<>[1 2 3 0;0 0 0 20]) then pause,end
v([2 1],4)=[10;20];if or(v<>[1 2 3 20;0 0 0 10]) then pause,end
v([2 2 2],4)=[10;20;30];if or(v<>[1 2 3 20;0 0 0 30]) then pause,end

v=[1 2 3;0 0 10];
v(1,[1 1 1 1])=1:4;;if or(v<>[4 2 3;0 0 10]) then pause,end
v([2 2],[1 1 1 1])=[1:4;2:5];if or(v<>[4 2 3;5 0 10]) then pause,end
v(3,4)=7;if or(v<>[4 2 3 0;5 0 10 0;0 0 0 7]) then pause,end

v=[];
v(5)=1;if or(v<>[0;0;0;0;1]) then pause,end
v(2)=4;if or(v<>[0;4;0;0;1]) then pause,end

v=[1 2 3;4 5 6];
v(:,1)=[4;1];if or(v<>[4 2 3;1 5 6]) then pause,end
v(:,[1 1])=[4 5;1 2];if or(v<>[5 2 3;2 5 6]) then pause,end
v(:,[3 1])=[4 5;1 2];if or(v<>[5 2 4;2 5 1]) then pause,end
v(:,[3 3 3 3])=[4 5 6 7;1 2 3 4];if or(v<>[5 2 7;2 5 4]) then pause,end

v=[1 2 3;4 5 6];
v(1,:)=[2 3 4];if or(v<>[2 3 4;4 5 6]) then pause,end
v([1 1],:)=[2 3 4;4 5 6];if or(v<>[4 5 6;4 5 6]) then pause,end
v([2 2 2 2],:)=[2 3 4;4 5 6;7 8 9;10 11 12];
if or(v<>[4 5 6;10 11 12]) then pause,end

v=[1 2 3];
v(2)=[];if or(v<>[1 3]) then pause,end
v=[1 2 3];
v([3 2])=[];if v<>1 then pause,end

v=[1 2 3]';
v(2)=[];if or(v<>[1 3]') then pause,end
v=[1 2 3]';
v([3 2])=[];if v<>1 then pause,end

v=[1 2 3;4 5 6];
v(1,:)=[];if or(v<>[4 5 6]);then pause,end
v=[1 2 3;4 5 6];
v(:,[3 2])=[];if or(v<>[1;4]);then pause,end
//======================
v=[1 2 3];
v(2)=%i;if or(v<>[1 %i 3]) then pause,end
v(4)=-5;if or(v<>[1 %i 3 -5]) then pause,end
v([1 4])=[2*%i 5];if or(v<>[2*%i %i 3 5]) then pause,end
v([1 1])=[2 5*%i];if or(v<>[5*%i %i 3 5]) then pause,end
v([1 1 1 1 1 1])=[2 5 6 7 8 9];if or(v<>[9 %i 3 5]) then pause,end
v([4 2])=[8 5];if or(v<>[9 5 3 8]) then pause,end

v=[%i 2 3];
v(2,4)=10;if or(v<>[%i 2 3 0;0 0 0 10]) then pause,end
v([2 2],4)=[10;20*%i];if or(v<>[%i 2 3 0;0 0 0 20*%i]) then pause,end
v([2 1],4)=[10;20*%i];if or(v<>[%i 2 3 20*%i;0 0 0 10]) then pause,end
v([2 2 2],4)=[10;20;30];if or(v<>[%i 2 3 20*%i;0 0 0 30]) then pause,end

v=[1 2 3;0 0 10*%i];
v(1,[1 1 1 1])=1:4;if or(v<>[4 2 3;0 0 10*%i]) then pause,end
v([2 2],[1 1 1 1])=[1:4;2:5];if or(v<>[4 2 3;5 0 10*%i]) then pause,end
v(3,4)=7;if or(v<>[4 2 3 0;5 0 10*%i 0;0 0 0 7]) then pause,end

v=[];
v(5)=%i;if or(v<>[0;0;0;0;%i]) then pause,end
v(2)=4;if or(v<>[0;4;0;0;%i]) then pause,end

v=[1 2 3;4 5 6];
v(:,1)=[4;%i];if or(v<>[4 2 3;%i 5 6]) then pause,end
v(:,[1 1])=[4 5;1 2];if or(v<>[5 2 3;2 5 6]) then pause,end
v(:,[3 1])=[4 5;%i 2];if or(v<>[5 2 4;2 5 %i]) then pause,end
v(:,[3 3 3 3])=[4 5 6 7;1 2 3 4];if or(v<>[5 2 7;2 5 4]) then pause,end

v=[%i 2 3;4 5 6];
v(1,:)=[2 3 4];if or(v<>[2 3 4;4 5 6]) then pause,end
v([1 1],:)=[2 3 4;4*%i 5 6];if or(v<>[4*%i 5 6;4 5 6]) then pause,end
v([2 2 2 2],:)=[2 3 4;4 5 6;7 8 9;10 11 12];
if or(v<>[4*%i 5 6;10 11 12]) then pause,end

v=[%i 2 3];
v(2)=[];if or(v<>[%i 3]) then pause,end
v=[1 2 3*%i];
v([3 2])=[];if v<>1 then pause,end

v=[%i 2 3]';
v(2)=[];if or(v<>[%i 3]') then pause,end
v=[1 2 3*%i]';
v([3 2])=[];if v<>1 then pause,end

v=[%i 2 3;4 5 6];
v(1,:)=[];if or(v<>[4 5 6]);then pause,end
v=[%i 2 3;4 5 6];
v(:,[3 2])=[];if or(v<>[%i;4]);then pause,end

//hidden empty index
v=[1 2;3 4];v_ref=v;

v(1,[%f %f])=[];
if or(v<>v_ref) then pause,end
v([%f %f],1)=[];
if or(v<>v_ref) then pause,end
v([%f %f],[%f %f])=[];
if or(v<>v_ref) then pause,end

v = [(1) (2) (3)];
if or(v <> [1 2 3]) then pause, end

assert_checkequal([]-[],[]);

a=matrix(1:9,3,3);
assert_checkequal(a(3,2,1),6);
assert_checkequal(a(8),8);
assert_checkerror("a(3,3,3)", msprintf(_("Invalid index.\n")));
assert_checkerror("a(2,4,1)", msprintf(_("Invalid index.\n")));
assert_checkerror("a(11)", msprintf(_("Invalid index.\n")));

A = rand(6, 4, 2, 8);
[S(:,:,1), k(:,:,1)] = (A(:,:,1), A(:,:,2));
assert_checkequal(S(:,:,1), A(:,:,1));

function [a,b]=toto()
a=2;b=5;
endfunction
[t(1,3), g(2,1)] = toto();
assert_checkequal(t,[0,0,2]);
assert_checkequal(g,[0;5]);

