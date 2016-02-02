// Copyright (C) 2008 - INRIA - Michael Baudin
// Copyright (C) 2010 - DIGITEO - Michael Baudin
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

// <-- CLI SHELL MODE -->
// <-- ENGLISH IMPOSED -->

function flag = MY_assert_equal ( computed , expected )
    if computed==expected then
        flag = 1;
    else
        flag = 0;
    end
    if flag <> 1 then pause,end
endfunction

format("v",10);

//
dmax = -log(2^(-53))/log(10);
//
computed = assert_computedigits ( 1 , 1 );
MY_assert_equal ( computed , dmax );
//
computed = assert_computedigits ( 0 , 0 );
MY_assert_equal ( computed , dmax );
//
computed = assert_computedigits ( 1 , 0 );
MY_assert_equal ( computed , 0 );
//
computed = assert_computedigits ( 0 , 1 );
MY_assert_equal ( computed , 0 );
//
computed = assert_computedigits ( 3.1415926 , %pi );
MY_assert_equal ( computed , 7.467037797136421240 );
//
computed = assert_computedigits ( 3.1415926 , %pi , 2 );
MY_assert_equal ( computed , 24.804962643893318841037 );
//
computed = assert_computedigits ( [0 0 1 1] , [0 1 0 1] );
MY_assert_equal ( computed , [dmax 0 0 dmax] );
//
computed = assert_computedigits(ones(3,2),ones(3,2));
MY_assert_equal ( computed , dmax * ones(3,2) );
//
computed = assert_computedigits([%nan %nan %nan %nan],[%nan %inf -%inf 0]);
MY_assert_equal ( computed , [dmax 0 0 0] );
//
computed = assert_computedigits([%inf %inf %inf %inf],[%nan %inf -%inf 0]);
MY_assert_equal ( computed , [0 dmax 0 0] );
//
computed = assert_computedigits([-%inf -%inf -%inf -%inf],[%nan %inf -%inf 0]);
MY_assert_equal ( computed , [0 0 dmax 0] );
//
computed = assert_computedigits([0 0 0 0],[%nan %inf -%inf 0]);
MY_assert_equal ( computed , [0 0 0 dmax] );
//
computed = assert_computedigits(1.224646799D-16,8.462643383D-18);
MY_assert_equal ( computed , 0 );
//
computed = assert_computedigits ( 1.2345 + %i*6.7891 , 1.23456789 + %i*6.789123456 );
MY_assert_equal ( computed , 3.9586791728311578886235 );
//
// The sign bit of the number of digits may be wrong because
// ieee(2); z=max(-0,0); 1/z is -%inf
back = ieee();
ieee(2);
computed = assert_computedigits ( 1.e-305 , 0 );
MY_assert_equal ( 1/computed , %inf );
//
computed = assert_computedigits ( 0 , 1.e-305 );
MY_assert_equal ( 1/computed , %inf );
ieee(back);

//
// An empirically found test case
a = [
3.982729777831130693D-59
2.584939414228211484D-26
4.391531370352049090D+43
1.725436586898508346D+68
];
b = [
3.982729777831130693D-59
2.584939414228211484D-26
4.391531370352048595D+43
1.725436586898508107D+68
];
c = assert_computedigits ( a , b , 2 );
e = [
53.
53.
51.977632
51.678072
];
assert_checkalmostequal ( c , e , 1.e-7 );
//
// Check that the vectorization was correct, i.e. no specific
// case in the processing of the data is forgotten.
//
function pI = permInverse(p)
    // Given the permutation p, compute the
    // inverse permutation pI.
    N = size(p,"*")
    pI(p) = (1:N)'
endfunction

a = [
1.234567891234567891
1.2345678912345678
1.23456789123456
1.234567891234
1.2345678912
1.23456789
1.234567
1.2345
1.23
1.2
1.
0.
%nan
%nan
%nan
%inf
%inf
%inf
-%inf
-%inf
-%inf
0.
0.
-0.
-0.
];

N = size(a,"*");
for k = 1 : 10
    mprintf("Test #%d\n",k);
    p1 = grand(1,"prm",(1:N)');
    p2 = grand(1,"prm",(1:N)');
    computed = a(p1);
    expected = a(p2);
    d1 = assert_computedigits(computed,expected);
    // Permute both computed and expected with the same permutation p3:
    // d must not change.
    p3 = grand(1,"prm",(1:N)');
    computedP = computed(p3);
    expectedP = expected(p3);
    d2 = assert_computedigits(computedP,expectedP);
    // Apply inverse permutation on d2.
    pI = permInverse(p3);
    d2 = d2(pI);
    assert_checkequal(d1,d2);
end


