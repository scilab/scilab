// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

// Operations on strings

if 'a'=='b'  then pause,end
if 'a'==''   then pause,end
if ''=='b'   then pause,end
if 'a'=='ab' then pause,end
if 'a'<>'a'  then pause,end
if ''<>''    then pause,end

if execstr("''a''>''b''"  ,'errcatch') == 0 then pause,end
if execstr("''a''>=''b''" ,'errcatch') == 0 then pause,end
if execstr("''a''<=''b''" ,'errcatch') == 0 then pause,end
if execstr("''a''<''b''"  ,'errcatch') == 0 then pause,end

if 'abc'+'def' <> 'abcdef' then pause,end
if ['abc']     <> 'abc'    then pause,end


r=['abc','def'];

if or(size(r)    <> [1 2] ) then pause,end
if r(1)          <> 'abc'   then pause,end
if r(1,[%t, %f]) <> 'abc'   then pause,end
if or(r(:)       <> r'    ) then pause,end
if r(1,[%t, %f]) <>'abc'    then pause,end


r=['abc','def'
   ''   ,'1234'
   'x'  ,''    ];

if or(size(r)       <> [3 2]                  ) then pause,end
if or(r(1,:)        <> ['abc','def']          ) then pause,end
if or(r([1,1],1)    <> ['abc';'abc']          ) then pause,end
if or(r(:,[1  2 1]) <> [r(:,1) r(:,2) r(:,1)] ) then pause,end
if or(r(:,1)        <> ['abc';'';'x']         ) then pause,end
if or(r(:,:)        <> r                      ) then pause,end
if or(r(:)          <> matrix(r,-1,1)         ) then pause,end


r(2,:)=[];
if or(r<>['abc','def';'x','']) then pause,end

r(:,1)=[];
if or(r<>['def';'']) then pause,end
