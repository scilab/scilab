// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- JVM NOT MANDATORY -->
warning('off')
//================================
a=[5 1 3 2 4];
if or(sort(a)<>[5 4 3 2 1]) then pause,end
if or(sort(a+0)<>[5 4 3 2 1]) then pause,end
[s,k]=sort(a);
if or(k<>[1 5 3 4 2]) then pause,end
if or(s<>[5 4 3 2 1]) then pause,end
[s,k]=sort(a+0);
if or(k<>[1 5 3 4 2]) then pause,end
if or(s<>[5 4 3 2 1]) then pause,end
//================================
a=string([5 1 3 2 4]);
if or(sort(a)<>string(1:5)) then pause,end
if or(sort(string([5 1 3 2 4]))<>string(1:5)) then pause,end
[s,k]=sort(a);
if or(k<>[2 4 3 5 1]) then pause,end
if or(s<>string(1:5)) then pause,end
[s,k]=sort(string([5 1 3 2 4]));
if or(k<>[2 4 3 5 1]) then pause,end
if or(s<>string(1:5)) then pause,end
//================================
a=[];
if sort(a)<>[] then pause,end
[s,k]=sort(a);if s<>[]|k<>[] then pause,end
//================================
if sort([])<>[] then pause,end
[s,k]=sort([]);
if s<>[]|k<>[] then pause,end
//================================
//sort string
[s]=sort(['abc','abd','aa','bxe']);
if or(s<>['aa','abc','abd','bxe']) then pause,end
[s,k]=sort(['abc','abd','aa','bxe']);
if or(s<>['aa','abc','abd','bxe']) then pause,end
if or(k<>[3 1 2 4])  then pause,end
if (sort('abc')<>'abc') then pause,end
if (sort([]) <> []) then pause,end
//================================
// sort matrix
A=[1,2,5;3,4,2];
[A_trie,indices]=sort(A);
if (indices <> [5,2,6;4,3,1]) then pause,end
if (A_trie <> [5,3,2;4,2,1]) then pause,end
//================================
// sort sparse matrix
pos = [1,8;1,12;1,4];
vals = [1,4,5];
sp = sparse(pos,vals);
[s] = sort(sp);
if ( size(s) <> size(sp) ) then pause,end
[out1,out2,out3] = spget(s);
if ( out1 <> gsort(pos,'r','i')  ) then pause,end
if ( out2 <> sort(vals)'  ) then pause,end
if ( out3 <> size(sp)  ) then pause,end
//================================
// sort complex matrix
A = [18 21 10 7 5];
B = [1  3  22 8 4];
y = complex(A,B);
[a,b] = sort(y);
if or( b <> [3 2 1 4 5] ) then pause,end
if ( y(b) <> a  ) then pause,end
//================================
warning('on')