// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2021 - Samuel GOUGEON - Le Mans Université
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->

// Checking the size of the result
// ===============================
for m = ["linear", "spline", "nearest"]
    for x = list(1:0.5:4, (1:0.5:4)')
        // Y vector => size(result) == size(xi)
        Xi = list(%pi, 1.2:0.2:2, (1.2:0.2:2)', 1+rand(2,5)*4, ..
             1+rand(1,2,5)*4, 1+rand(2,1,5)*4, 1+rand(2,5,6)*4);
        Y = list(5:11, (5:11)');
        for y = Y
            for xi = Xi
                r = interp1(x, y, xi, m);
                assert_checkequal(size(r), size(xi));
            end
        end

        // y = matrix or hypermatrix, xi = scalar or vector
        Y = list(rand(7,3), rand(7,1,3), rand(7,3,4));
        Xi = list(%pi, 1.2:0.2:2, (1.2:0.2:2)');
        for y = Y
            for xi = Xi
                r = interp1(x, y, xi, m);
                assert_checkequal(size(r), [length(xi) size(y)(2:$)]);
            end
        end

        // Y and xi are matrices or hypermatrices
        Y = list(rand(7,3), rand(7,1,3), rand(7,3,4));
        Xi = list(1+rand(2,5)*4, 1+rand(1,2,5)*4, 1+rand(2,1,5)*4, 1+rand(2,5,6)*4);
        for y = Y
            for xi = Xi
                r = interp1(x, y, xi, m);
                assert_checkequal(size(r), [size(xi) size(y)(2:$)]);
            end
        end
    end
end


// =============
// y is a vector
// =============
x = 1:4;
// method = "linear" (default)
// ---------------------------
// extrapolation "by_nan" (default)
for y = list(2:5, (2:5)')
    assert_checkequal(interp1(x,y, 0), %nan);
    assert_checkequal(interp1(x,y,1.5), 2.5);
    assert_checkequal(interp1(x,y,[0.5:5]), [%nan 2.5 3.5 4.5 %nan]);
    assert_checkequal(interp1(x,y,[0.5:5]'), [%nan 2.5 3.5 4.5 %nan]');
    assert_checkequal(interp1(x,y,[0.5 1.5 2.5;3.5 4.5 5.5]), [%nan 2.5 3.5;4.5 %nan %nan]);
    Ref = cat(3, [%nan 2.5 3.5], [4.5 %nan %nan]);
    assert_checkequal(interp1(x, y, cat(3,[0.5 1.5 2.5],[3.5 4.5 5.5])), Ref);
end
// extrapolation "extrap" = "linear"
for y = list(2:5, (2:5)')
    assert_checkequal(interp1(x,y, 0, ,"extrap"), 1);
    assert_checkequal(interp1(x,y,1.5, ,"extrap"), 2.5);
    assert_checkequal(interp1(x,y,[0.5:5], ,"extrap"), [1.5 2.5 3.5 4.5 5.5]);
    assert_checkequal(interp1(x,y,[0.5:5]', ,"extrap"), [1.5 2.5 3.5 4.5 5.5]');
    assert_checkequal(interp1(x,y,[0.5 1.5 2.5;3.5 4.5 5.5], ,"extrap"), ..
                      [1.5 2.5 3.5;4.5 5.5 6.5]);
    Ref = cat(3, [1.5 2.5 3.5], [4.5 5.5 6.5]);
    assert_checkequal(interp1(x, y, cat(3,[0.5 1.5 2.5],[3.5 4.5 5.5]), ,"extrap"), Ref);
end
// extrapolation by padding
for y = list(2:5, (2:5)')
    assert_checkequal(interp1(x,y, 0, ,%pi), %pi);
    assert_checkequal(interp1(x,y,1.5, ,%pi), 2.5);
    assert_checkequal(interp1(x,y,[0.5:5], ,%pi), [%pi 2.5 3.5 4.5 %pi]);
    assert_checkequal(interp1(x,y,[0.5:5]', ,%pi), [%pi 2.5 3.5 4.5 %pi]');
    assert_checkequal(interp1(x,y,[0.5 1.5 2.5;3.5 4.5 5.5], ,%pi), ..
                      [%pi 2.5 3.5;4.5 %pi %pi]);
    Ref = cat(3, [%pi 2.5 3.5], [4.5 %pi %pi]);
    assert_checkequal(interp1(x, y, cat(3,[0.5 1.5 2.5],[3.5 4.5 5.5]), ,%pi), Ref);
end
// periodic extrapolation
for y = list([2 3 4 2], [2 3 4 2]')
    assert_checkequal(interp1(x,y, 0, ,"periodic"), 4);
    assert_checkequal(interp1(x,y,1.5, ,"periodic"), 2.5);
    assert_checkequal(interp1(x,y,[0.5:5], ,"periodic"), [3 2.5 3.5 3 2.5]);
    assert_checkequal(interp1(x,y,[0.5:5]', ,"periodic"), [3 2.5 3.5 3 2.5]');
    assert_checkequal(interp1(x,y,[0.5 1.5 2.5;3.5 4.5 5.5], ,"periodic"), ..
                      [3 2.5 3.5 ; 3 2.5 3.5]);
    Ref = cat(3, [3 2.5 3.5], [3 2.5 3.5]);
    assert_checkequal(interp1(x, y, cat(3,[0.5 1.5 2.5],[3.5 4.5 5.5]), ,"periodic"), Ref);
end
// extrapolation by edgevalue
e = "edgevalue";
for y = list(2:5, (2:5)')
    assert_checkequal(interp1(x,y, 0, , e), 2);
    assert_checkequal(interp1(x,y,1.5, , e), 2.5);
    assert_checkequal(interp1(x,y,[0.5:5], , e), [2 2.5 3.5 4.5 5]);
    assert_checkequal(interp1(x,y,[0.5:5]', , e), [2 2.5 3.5 4.5 5]');
    assert_checkequal(interp1(x,y,[0.5 1.5 2.5;3.5 4.5 5.5], , e), [2 2.5 3.5 ; 4.5 5 5]);
    Ref = cat(3, [2 2.5 3.5], [4.5 5 5]);
    assert_checkequal(interp1(x, y, cat(3,[0.5 1.5 2.5],[3.5 4.5 5.5]), , e), Ref);
end

// method = "nearest"
// ------------------
m = "nearest";
// extrapolation "by_nan" (default)
for y = list(2:5, (2:5)')
    assert_checkequal(interp1(x,y, [0 5.4], m), [%nan %nan]);
    assert_checkequal(interp1(x,y,1.5, m), 3);
    assert_checkequal(interp1(x,y,[0.5:5], m), [%nan 3 4 5 %nan]);
    assert_checkequal(interp1(x,y,[0.5:5]', m), [%nan 3 4 5 %nan]');
    assert_checkequal(interp1(x,y,[0.5 1.5 2.5;3.5 4.5 5.5], m), [%nan 3 4;5 %nan %nan]);
    Ref = cat(3, [%nan 3 4], [5 %nan %nan]);
    assert_checkequal(interp1(x, y, cat(3,[0.5 1.5 2.5],[3.5 4.5 5.5]), m), Ref);
end
// extrapolation "extrap" = "nearest" = "edgevalue"
for e = ["extrap" "edgevalue" "nearest"]
    for y = list(2:5, (2:5)')
        assert_checkequal(interp1(x,y, [0 5.4], m, e), [2 5]);
        assert_checkequal(interp1(x,y,1.5, m, e), 3);
        assert_checkequal(interp1(x,y,[0.5:5], m, e), [2 3 4 5 5]);
        assert_checkequal(interp1(x,y,[0.5:5]', m, e), [2 3 4 5 5]');
        assert_checkequal(interp1(x,y,[0.5 1.5 2.5;3.5 4.5 5.5], m, e), ..
                          [2 3 4 ; 5 5 5]);
        Ref = cat(3, [2 3 4], [5 5 5]);
        assert_checkequal(interp1(x, y, cat(3,[0.5 1.5 2.5],[3.5 4.5 5.5]), m, e), Ref);
    end
end
// extrapolation by padding
for y = list(2:5, (2:5)')
    assert_checkequal(interp1(x,y, [0 5.4], m, %pi), [%pi %pi]);
    assert_checkequal(interp1(x,y, 1.5, m, %pi), 3);
    assert_checkequal(interp1(x,y, 0.5:5, m, %pi), [%pi 3 4 5 %pi]);
    assert_checkequal(interp1(x,y,[0.5:5]', m, %pi), [%pi 3 4 5 %pi]');
    assert_checkequal(interp1(x,y,[0.5 1.5 2.5;3.5 4.5 5.5], m, %pi), ..
                      [%pi 3 4 ; 5 %pi %pi]);
    Ref = cat(3, [%pi 3 4], [5 %pi %pi]);
    assert_checkequal(interp1(x, y, cat(3,[0.5 1.5 2.5],[3.5 4.5 5.5]), m, %pi), Ref);
end

// method = "spline"
// -----------------
// extrapolation default == "spline" (=="natural")
m = "spline";
for y = list(2:5, (2:5)')
    assert_checkalmostequal(interp1(x,y, [0 5.4], m), [1 6.4], 5*%eps);
    assert_checkequal(interp1(x,y,1.5, m), 2.5);
    assert_checkalmostequal(interp1(x,y,[0.5:5], m), 1.5:5.5, %eps);
    assert_checkalmostequal(interp1(x,y,[0.5:5]', m), (1.5:5.5)', %eps);
    assert_checkalmostequal(interp1(x,y,[0.5 1.5 2.5;3.5 4.5 5.5], m), [1.5 2.5 3.5;4.5 5.5 6.5], 5*%eps);
    Ref = cat(3, [1.5 2.5 3.5], [4.5 5.5 6.5]);
    assert_checkalmostequal(interp1(x, y, cat(3,[0.5 1.5 2.5],[3.5 4.5 5.5]), m), Ref, 5*%eps);
end
// extrapolation "extrap" == "spline"
for y = list(2:5, (2:5)')
    assert_checkalmostequal(interp1(x,y, [0 5.4], m, "extrap"), [1 6.4], 5*%eps);
    assert_checkequal(interp1(x,y,1.5, m, "extrap"), 2.5);
    assert_checkalmostequal(interp1(x,y,[0.5:5], m, "extrap"), 1.5:5.5, %eps);
    assert_checkalmostequal(interp1(x,y,[0.5:5]', m, "extrap"), (1.5:5.5)', %eps);
    Ref = [1.5 2.5 3.5 ; 4.5 5.5 6.5];
    assert_checkalmostequal(interp1(x,y,[0.5 1.5 2.5;3.5 4.5 5.5], m, "extrap"), Ref, 5*%eps);
    Ref = cat(3, [1.5 2.5 3.5], [4.5 5.5 6.5]);
    assert_checkalmostequal(interp1(x, y, cat(3,[0.5 1.5 2.5],[3.5 4.5 5.5]), m, "extrap"), Ref, 5*%eps);
end
// extrapolation by padding
for y = list(2:5, (2:5)')
    assert_checkequal(interp1(x,y, [0 5.4], m, %pi), [%pi %pi]);
    assert_checkequal(interp1(x,y, 1.5, m, %pi), 2.5);
    assert_checkequal(interp1(x,y, 0.5:5, m, %pi), [%pi 2.5 3.5 4.5 %pi]);
    assert_checkequal(interp1(x,y,[0.5:5]', m, %pi), [%pi 2.5 3.5 4.5 %pi]');
    assert_checkequal(interp1(x,y,[0.5 1.5 2.5;3.5 4.5 5.5], m, %pi), ..
                      [%pi 2.5 3.5 ; 4.5 %pi %pi]);
    Ref = cat(3, [%pi 2.5 3.5], [4.5 %pi %pi]);
    assert_checkequal(interp1(x, y, cat(3,[0.5 1.5 2.5],[3.5 4.5 5.5]), m, %pi), Ref);
end
// periodic extrapolation
for y = list([2 3 4 2], [2 3 4 2]')
    assert_checkalmostequal(interp1(x,y, [0 5.4], m, "periodic"), [4 3.736], %eps);
    assert_checkequal(interp1(x,y, 1.5, m, "periodic"), 2.125);
    assert_checkalmostequal(interp1(x,y, 0.5:5, m, "periodic"), [3 2.125 3.875 3 2.125], %eps);
    assert_checkalmostequal(interp1(x,y,[0.5:5]', m, "periodic"), [3 2.125 3.875 3 2.125]', %eps);
    assert_checkalmostequal(interp1(x,y,[0.5 1.5 2.5;3.5 4.5 5.5], m, "periodic"), ..
                      [3 2.125 3.875 ; 3 2.125 3.875], %eps);
    Ref = cat(3, [3 2.125 3.875], [3 2.125 3.875]);
    assert_checkalmostequal(interp1(x, y, cat(3,[0.5 1.5 2.5],[3.5 4.5 5.5]), m, "periodic"), Ref, %eps);
end
// extrapolation by edgevalue
e = "edgevalue";
for y = list(2:5, (2:5)')
    assert_checkequal(interp1(x,y, [0 5.4], m, e), [2 5]);
    assert_checkequal(interp1(x,y, 1.5, m, e), 2.5);
    assert_checkequal(interp1(x,y, 0.5:5, m, e), [2 2.5 3.5 4.5 5]);
    assert_checkequal(interp1(x,y,[0.5:5]', m, e), [2 2.5 3.5 4.5 5]');
    assert_checkequal(interp1(x,y,[0.5 1.5 2.5;3.5 4.5 5.5], m, e), ..
                                  [2 2.5 3.5 ; 4.5 5 5]);
    Ref = cat(3, [2 2.5 3.5], [4.5 5 5]);
    assert_checkequal(interp1(x, y, cat(3,[0.5 1.5 2.5],[3.5 4.5 5.5]), m, e), Ref);
end

// =============
// y is a matrix
// =============
x = 1:4;
y = [0 2 4 6 ; 0.5 1.5 2.5 3.5]';
// method = "linear" (default)
// ---------------------------
// extrapolation "by_nan" (default)
assert_checkequal(interp1(x,y, 0), [%nan %nan]);
assert_checkequal(interp1(x,y,1.5), [1 1]);
assert_checkequal(interp1(x,y, 0.5:5  ), [%nan 1 3 5 %nan; %nan 1 2 3 %nan]');
assert_checkequal(interp1(x,y,(0.5:5)'), [%nan 1 3 5 %nan; %nan 1 2 3 %nan]');
Ref = cat(3,[%nan 1 3 ; 5 %nan %nan], [%nan 1 2 ; 3 %nan %nan]);
assert_checkequal(interp1(x,y,[0.5 1.5 2.5;3.5 4.5 5.5]), Ref);
Ref = matrix([%nan 1 3 5 %nan %nan %nan 1 2 3 %nan %nan],[1 3 2 2]);
assert_checkequal(interp1(x, y, cat(3,[0.5 1.5 2.5],[3.5 4.5 5.5])), Ref);

// extrapolation "extrap" = "linear"
assert_checkequal(interp1(x,y, 0, ,"extrap"), [-2 -0.5]);
assert_checkequal(interp1(x,y,1.5, ,"extrap"), [1 1]);
assert_checkequal(interp1(x,y, 0.5:4,   ,"extrap"), [-1 1 3 5 ; 0 1 2 3]');
assert_checkequal(interp1(x,y,(0.5:4)', ,"extrap"), [-1 1 3 5 ; 0 1 2 3]');
Ref = cat(3, [-1 1 3 ; 5 7 9], [0 1 2 ; 3 4 5]);
assert_checkequal(interp1(x,y,[0.5 1.5 2.5;3.5 4.5 5.5], ,"extrap"), Ref);
Ref = matrix([-1 1 3 5 7 9 0 1 2 3 4 5],[1 3 2 2]);
assert_checkequal(interp1(x, y, cat(3,[0.5 1.5 2.5],[3.5 4.5 5.5]), ,"extrap"), Ref);

// extrapolation by padding
assert_checkequal(interp1(x,y, 0, ,%pi), [%pi %pi]);
assert_checkequal(interp1(x,y,1.5, ,%pi), [1 1]);
assert_checkequal(interp1(x,y, 0.5:5, ,%pi  ), [%pi 1 3 5 %pi; %pi 1 2 3 %pi]');
assert_checkequal(interp1(x,y,(0.5:5)', ,%pi), [%pi 1 3 5 %pi; %pi 1 2 3 %pi]');
Ref = cat(3,[%pi 1 3 ; 5 %pi %pi], [%pi 1 2 ; 3 %pi %pi]);
assert_checkequal(interp1(x,y,[0.5 1.5 2.5;3.5 4.5 5.5], ,%pi), Ref);
Ref = matrix([%pi 1 3 5 %pi %pi %pi 1 2 3 %pi %pi],[1 3 2 2]);
assert_checkequal(interp1(x, y, cat(3,[0.5 1.5 2.5],[3.5 4.5 5.5]), ,%pi), Ref);

// periodic extrapolation
y = [0 2 4 0 ; 0.5 1.5 2.5 0.5]';
assert_checkalmostequal(interp1(x,y, 5, ,"periodic"), [2 1.5], 2*%eps);
assert_checkalmostequal(interp1(x,y,1.5, ,"periodic"), [1 1], 2*%eps);
assert_checkalmostequal(interp1(x,y, 0.5:5, ,"periodic"  ), [2 1 3 2 1; 1.5 1 2 1.5 1]', 2*%eps);
assert_checkalmostequal(interp1(x,y,(0.5:5)', ,"periodic"), [2 1 3 2 1; 1.5 1 2 1.5 1]', 2*%eps);
Ref = cat(3,[2 1 3 ; 2 1 3], [1.5 1 2 ; 1.5 1 2]);
assert_checkalmostequal(interp1(x,y,[0.5 1.5 2.5;3.5 4.5 5.5], ,"periodic"), Ref, 2*%eps);
Ref = matrix([2 1 3 2 1 3 1.5 1 2 1.5 1 2],[1 3 2 2]);
assert_checkalmostequal(interp1(x, y, cat(3,[0.5 1.5 2.5],[3.5 4.5 5.5]), ,"periodic"), Ref, 2*%eps);

// extrapolation by edgevalue
e = "edgevalue";
y = [0 2 4 6 ; 0.5 1.5 2.5 3.5]';
assert_checkequal(interp1(x,y, 0, , e), [0 0.5]);
assert_checkequal(interp1(x,y,1.5, , e), [1 1]);
assert_checkequal(interp1(x,y, 0.5:5, , e  ), [0 1 3 5 6 ; 0.5 1 2 3 3.5]');
assert_checkequal(interp1(x,y,(0.5:5)', , e), [0 1 3 5 6 ; 0.5 1 2 3 3.5]');
Ref = cat(3,[0 1 3 ; 5 6 6], [0.5 1 2 ; 3 3.5 3.5]);
assert_checkequal(interp1(x,y,[0.5 1.5 2.5;3.5 4.5 5.5], , e), Ref);
Ref = matrix([0 1 3 5 6 6 0.5 1 2 3 3.5 3.5],[1 3 2 2]);
assert_checkequal(interp1(x, y, cat(3,[0.5 1.5 2.5],[3.5 4.5 5.5]), ,e), Ref);

// method = "nearest"
// ------------------
m = "nearest";
// extrapolation "by_nan" (default)
assert_checkequal(interp1(x,y, [0 5.4], m), [%nan %nan ; %nan %nan]);
assert_checkequal(interp1(x,y,1.5, m), [2 1.5]);
assert_checkequal(interp1(x,y,[0.5:4], m), [%nan %nan ; 2 1.5 ; 4 2.5 ; 6 3.5]);
assert_checkequal(interp1(x,y,[0.5:4]', m),[%nan %nan ; 2 1.5 ; 4 2.5 ; 6 3.5]);
Ref = cat(3, [%nan 2 4 ; 6 %nan %nan], [%nan 1.5 2.5 ; 3.5 %nan %nan]);
assert_checkequal(interp1(x,y,[0.5 1.5 2.5 ; 3.5 4.5 5.5], m), Ref);  // OErr
Ref = matrix([%nan 2 4 6 %nan %nan %nan 1.5 2.5 3.5 %nan %nan],[1 3 2 2]);
assert_checkequal(interp1(x, y, cat(3,[0.5 1.5 2.5],[3.5 4.5 5.5]), m), Ref);

// extrapolation "extrap" = "nearest" = "edgevalue"
for e = ["extrap" "nearest" "edgevalue"]
    assert_checkequal(interp1(x,y, [0 5.4], m, e), [0 0.5 ; 6 3.5]);
    assert_checkequal(interp1(x,y,1.5, m, e), [2 1.5]);
    assert_checkequal(interp1(x,y,[0.5:4], m, e), y);
    assert_checkequal(interp1(x,y,[0.5:4]', m, e), y);
    Ref = cat(3, [0 2 4 ; 6 6 6], [0.5 1.5 2.5 ; 3.5 3.5 3.5]);
    assert_checkequal(interp1(x,y,[0.5 1.5 2.5 ; 3.5 4.5 5.5], m, e), Ref);
    Ref = matrix([0 2 4 6 6 6 0.5 1.5 2.5 3.5 3.5 3.5],[1 3 2 2]);
    assert_checkequal(interp1(x, y, cat(3,[0.5 1.5 2.5],[3.5 4.5 5.5]), m, e), Ref);
end

// extrapolation by padding
assert_checkequal(interp1(x,y, [0 5.4], m, %pi), [%pi %pi ; %pi %pi]);
assert_checkequal(interp1(x,y,1.5, m, %pi), [2 1.5]);
assert_checkequal(interp1(x,y,[0.5:4], m, %pi), [%pi %pi ; 2 1.5 ; 4 2.5 ; 6 3.5]);
assert_checkequal(interp1(x,y,[0.5:4]', m, %pi),[%pi %pi ; 2 1.5 ; 4 2.5 ; 6 3.5]);
Ref = cat(3, [%pi 2 4 ; 6 %pi %pi], [%pi 1.5 2.5 ; 3.5 %pi %pi]);
assert_checkequal(interp1(x,y,[0.5 1.5 2.5 ; 3.5 4.5 5.5], m, %pi), Ref); // OErr
Ref = matrix([%pi 2 4 6 %pi %pi %pi 1.5 2.5 3.5 %pi %pi],[1 3 2 2]);
assert_checkequal(interp1(x, y, cat(3,[0.5 1.5 2.5],[3.5 4.5 5.5]), m, %pi), Ref); // OErr

// method = "spline"
// -----------------
m = "spline";
// extrapolation default == "spline" (=="natural")
assert_checkalmostequal(interp1(x,y, [0 5.4], m), [-2 -0.5 ; 8.8 4.9], 5*%eps);
assert_checkequal(interp1(x,y,1.5, m), [1 1]);
assert_checkequal(interp1(x,y,[0.5:4], m), [-1 1 3 5 ; 0 1 2 3]');
assert_checkequal(interp1(x,y,[0.5:4]', m), [-1 1 3 5 ; 0 1 2 3]');
Ref = cat(3,[-1 1 3 ; 5 7 9], [0 1 2 ; 3 4 5]);
assert_checkalmostequal(interp1(x,y,[0.5 1.5 2.5;3.5 4.5 5.5], m), Ref, 5*%eps);
Ref = matrix([-1  1  3  5  7  9  0  1  2  3  4  5 ],[1 3 2 2]);
assert_checkalmostequal(interp1(x, y, cat(3,[0.5 1.5 2.5],[3.5 4.5 5.5]), m), Ref, 5*%eps);

// extrapolation "extrap" == "spline"
assert_checkalmostequal(interp1(x,y, [0 5.4], m, "extrap"), [-2 -0.5 ; 8.8 4.9], 5*%eps);
assert_checkequal(interp1(x,y,1.5, m, "extrap"), [1 1]);
assert_checkequal(interp1(x,y,[0.5:4], m, "extrap"), [-1 1 3 5 ; 0 1 2 3]');
assert_checkequal(interp1(x,y,[0.5:4]', m, "extrap"), [-1 1 3 5 ; 0 1 2 3]');
Ref = cat(3,[-1 1 3 ; 5 7 9], [0 1 2 ; 3 4 5]);
assert_checkalmostequal(interp1(x,y,[0.5 1.5 2.5;3.5 4.5 5.5], m, "extrap"), Ref, 5*%eps);
Ref = matrix([-1  1  3  5  7  9  0  1  2  3  4  5 ],[1 3 2 2]);
assert_checkalmostequal(interp1(x, y, cat(3,[0.5 1.5 2.5],[3.5 4.5 5.5]), m, "extrap"), Ref, 5*%eps);

// extrapolation by padding
assert_checkalmostequal(interp1(x,y, [0 5.4], m, %pi), [%pi %pi ; %pi %pi], 5*%eps);
assert_checkequal(interp1(x,y,1.5, m, %pi), [1 1]);
assert_checkequal(interp1(x,y,[0.5:4], m, %pi),  [%pi 1 3 5 ; %pi 1 2 3]');
assert_checkequal(interp1(x,y,[0.5:4]', m, %pi), [%pi 1 3 5 ; %pi 1 2 3]');
Ref = cat(3,[%pi 1 3 ; 5 %pi %pi], [%pi 1 2 ; 3 %pi %pi]);
assert_checkalmostequal(interp1(x,y,[0.5 1.5 2.5;3.5 4.5 5.5], m, %pi), Ref, 5*%eps);
Ref = matrix([%pi 1  3  5  %pi %pi %pi  1  2  3  %pi %pi],[1 3 2 2]);
assert_checkequal(interp1(x, y, cat(3,[0.5 1.5 2.5],[3.5 4.5 5.5]), m, %pi), Ref);

// extrapolation by edgevalue
e = "edgevalue";
assert_checkequal(interp1(x,y, [0 5.4], m, e), [0 0.5 ; 6 3.5]);
assert_checkequal(interp1(x,y,1.5, m, e), [1 1]);
assert_checkequal(interp1(x,y,[0.5:4], m, e),  [0 1 3 5 ; 0.5 1 2 3]');
assert_checkequal(interp1(x,y,[0.5:4]', m, e), [0 1 3 5 ; 0.5 1 2 3]');
Ref = cat(3, [0 1 3 ; 5 6 6], [0.5 1 2 ; 3 3.5 3.5]);
assert_checkequal(interp1(x,y,[0.5 1.5 2.5;3.5 4.5 5.5], m, e), Ref);
Ref = matrix([0 1 3 5 6 6 0.5 1 2 3 3.5 3.5], [1 3 2 2]);
assert_checkequal(interp1(x, y, cat(3,[0.5 1.5 2.5],[3.5 4.5 5.5]), m, e), Ref);

// periodic extrapolation
y = [0 2 4 0 ; 0.5 1.5 2.5 0.5]';
assert_checkalmostequal(interp1(x,y, [0 5.4], m, "periodic"), [4 2.5 ; 3.136 2.068], 2*%eps);
assert_checkalmostequal(interp1(x,y,1.5, m, "periodic"), [0.625 0.8125], 5*%eps);
Ref = [3.125  2.0625 ; 0.625  0.8125 ; 3.375  2.1875 ; 3.125  2.0625];
assert_checkequal(interp1(x,y,[0.5:4], m, "periodic"),  Ref);
assert_checkequal(interp1(x,y,[0.5:4]', m, "periodic"), Ref);
Ref = cat(3, [3.125 0.625 3.375 ; 3.125 0.625 3.375], ..
             [2.0625 0.8125 2.1875 ; 2.0625 0.8125 2.1875]);
assert_checkalmostequal(interp1(x,y,[0.5 1.5 2.5;3.5 4.5 5.5], m, "periodic"), Ref, 5*%eps);
Ref = matrix([3.125 0.625 3.375 3.125 0.625 3.375 2.0625 0.8125 2.1875 2.0625 0.8125 2.1875],[1 3 2 2]);
assert_checkalmostequal(interp1(x, y, cat(3,[0.5 1.5 2.5],[3.5 4.5 5.5]), m, "periodic"), Ref, 5*%eps);


// ===================
// y is an hypermatrix
// ===================
y = cat(4,[0 2 4 6 ; 0.5 1.5 2.5 3.5]', [1:4 ; 2:5]');
// method = "linear" (default)
// ---------------------------
// extrapolation "by_nan" (default)
assert_checkequal(interp1(x,y, 0), cat(4,[%nan %nan],[%nan %nan]));    // OWrong size and values
assert_checkequal(interp1(x,y,1.5), cat(4,[1 1],[1.5 2.5]));           // OWrong size
Ref = cat(4,[%nan 1 3 5 ; %nan 1 2 3]', [%nan 1.5:3.5 ; %nan 2.5:4.5]');
assert_checkequal(interp1(x,y, 0.5:4), Ref);
assert_checkequal(interp1(x,y,(0.5:4)'), Ref);
Ref = matrix([%nan 5 1 %nan 3 %nan %nan 3 1 %nan 2 %nan %nan 3.5 1.5 %nan ..
   2.5 %nan %nan 4.5 2.5 %nan 3.5 %nan],[2 3 2 1 2]);
assert_checkequal(interp1(x,y,[0.5 1.5 2.5;3.5 4.5 5.5]), Ref);
Ref = matrix([%nan 1 3 5 %nan %nan %nan 1 2 3 %nan %nan %nan 1.5 2.5 3.5 ..
              %nan %nan %nan 2.5 3.5 4.5 %nan %nan],[1 3 2 2 1 2]);
assert_checkequal(interp1(x, y, cat(3,[0.5 1.5 2.5],[3.5 4.5 5.5])), Ref);

// extrapolation "extrap" = "linear"
assert_checkequal(interp1(x,y, 0, ,"extrap"), cat(4, [-2 -0.5],[0 1]));   // OWrong size
assert_checkequal(interp1(x,y,1.5, ,"extrap"), cat(4, [1 1],[1.5 2.5]));  // OWrong size
Ref = cat(4, [-1 1 3 5 ; 0 1 2 3]',[0.5:3.5 ; 1.5:4.5]');
assert_checkequal(interp1(x,y, 0.5:4,   ,"extrap"), Ref);
assert_checkequal(interp1(x,y,(0.5:4)', ,"extrap"), Ref);

Ref = matrix([-1 5 1 7 3 9 0 3 1 4 2 5 0.5 3.5 1.5 4.5 2.5 5.5 1.5 4.5 2.5 5.5 3.5 6.5],..
             [2 3 2 1 2]);
assert_checkequal(interp1(x,y,[0.5 1.5 2.5;3.5 4.5 5.5], ,"extrap"), Ref);
Ref = matrix([-1 1 3 5 7 9 0 1 2 3 4 5 0.5 1.5 2.5 3.5 4.5 5.5 1.5 2.5 3.5 4.5 5.5 6.5],..
             [1 3 2 2 1 2]);
assert_checkequal(interp1(x, y, cat(3,[0.5 1.5 2.5],[3.5 4.5 5.5]), ,"extrap"), Ref);

// extrapolation by padding
assert_checkequal(interp1(x,y, 0, , %pi), cat(4,[%pi %pi],[%pi %pi]));   // OWrong size and values
assert_checkequal(interp1(x,y,1.5, , %pi), cat(4,[1 1],[1.5 2.5]));
Ref = cat(4,[%pi 1 3 5 ; %pi 1 2 3]', [%pi 1.5:3.5 ; %pi 2.5:4.5]');
assert_checkequal(interp1(x,y, 0.5:4, , %pi), Ref);
assert_checkequal(interp1(x,y,(0.5:4)', , %pi), Ref);
Ref = matrix([%pi 5 1 %pi 3 %pi %pi 3 1 %pi 2 %pi %pi 3.5 1.5 %pi ..
   2.5 %pi %pi 4.5 2.5 %pi 3.5 %pi],[2 3 2 1 2]);
assert_checkequal(interp1(x,y,[0.5 1.5 2.5;3.5 4.5 5.5], , %pi), Ref);
Ref = matrix([%pi 1 3 5 %pi %pi %pi 1 2 3 %pi %pi %pi 1.5 2.5 3.5 ..
              %pi %pi %pi 2.5 3.5 4.5 %pi %pi],[1 3 2 2 1 2]);
assert_checkequal(interp1(x, y, cat(3,[0.5 1.5 2.5],[3.5 4.5 5.5]), , %pi), Ref);

// periodic extrapolation
y = cat(4,[0 2 4 0 ; 0.5 1.5 2.5 0.5]', [1 2 3 1 ; 2 3 4 2]');
assert_checkequal(interp1(x,y, 0, , "periodic"), cat(4,[4 2.5],[3 4]));
assert_checkequal(interp1(x,y,1.5, , "periodic"), cat(4,[1 1],[1.5 2.5]));
Ref = cat(4,[2 1 3 2 ; 1.5 1 2 1.5]', [2 1.5 2.5 2 ; 3 2.5 3.5 3]');
assert_checkequal(interp1(x,y, 0.5:4, , "periodic"), Ref);
assert_checkequal(interp1(x,y,(0.5:4)', , "periodic"), Ref);
Ref = matrix([2 2 1 1 3 3 1.5 1.5 1 1 2 2 2 2 1.5 1.5 ..
   2.5 2.5 3 3 2.5 2.5 3.5 3.5],[2 3 2 1 2]);
assert_checkalmostequal(interp1(x,y,[0.5 1.5 2.5 ; 3.5 4.5 5.5], , "periodic"), Ref, 2*%eps);
Ref = matrix([2 1 3 2 1 3 1.5 1 2 1.5 1 2 2 1.5 2.5 2 ..
              1.5 2.5 3 2.5 3.5 3 2.5 3.5],[1 3 2 2 1 2]);
assert_checkalmostequal(interp1(x, y, cat(3,[0.5 1.5 2.5],[3.5 4.5 5.5]), , "periodic"), Ref, 2*%eps);

// extrapolation by edgevalue
e = "edgevalue";
y = cat(4,[0 2 4 6 ; 0.5 1.5 2.5 3.5]', [1:4 ; 2:5]');
assert_checkequal(interp1(x,y, 0, , e) , cat(4,[0 0.5],[1 2]));
assert_checkequal(interp1(x,y,1.5, , e), cat(4,[1 1],[1.5 2.5]));
Ref = cat(4,[0 1 3 5 ; 0.5 1 2 3]', [1 1.5:3.5 ; 2 2.5:4.5]');
assert_checkequal(interp1(x,y, 0.5:4, , e), Ref);
assert_checkequal(interp1(x,y,(0.5:4)', , e), Ref);
Ref = matrix([0 5 1 6 3 6 0.5 3 1 3.5 2 3.5 1 3.5 1.5 4 ..
   2.5 4 2 4.5 2.5 5 3.5 5],[2 3 2 1 2]);
assert_checkequal(interp1(x, y,[0.5 1.5 2.5 ; 3.5 4.5 5.5], , e), Ref);
Ref = matrix([0 1 3 5 6 6 0.5 1 2 3 3.5 3.5 1 1.5 2.5 3.5 ..
              4 4 2 2.5 3.5 4.5 5 5],[1 3 2 2 1 2]);
assert_checkequal(interp1(x, y, cat(3,[0.5 1.5 2.5],[3.5 4.5 5.5]), , e), Ref);

// method = "nearest"
// ------------------
m = "nearest";
// extrapolation "by_nan" (default)
assert_checkequal(interp1(x,y, [0 5.4], m), %nan*ones(2,2,1,2));
assert_checkequal(interp1(x,y,1.5, m), cat(4,[2 1.5],[2 3]));
Ref = cat(4, [%nan %nan ; 2 1.5 ; 4 2.5 ; 6 3.5], [%nan %nan ; 2 3 ; 3 4 ; 4 5]);
assert_checkequal(interp1(x,y,[0.5:4], m), Ref);
assert_checkequal(interp1(x,y,[0.5:4]', m), Ref);
Ref = matrix([%nan 6 2 %nan 4 %nan %nan 3.5 1.5 %nan 2.5 %nan %nan 4 2 %nan 3 %nan %nan 5 3 %nan 4 %nan],[2 3 2 1 2]);
assert_checkequal(interp1(x,y,[0.5 1.5 2.5 ; 3.5 4.5 5.5], m), Ref);           // OErr
Ref = matrix([%nan 2 4 6 %nan %nan %nan 1.5 2.5 3.5 %nan %nan %nan 2 3 4 %nan %nan %nan 3 4 5 %nan%nan],[1 3 2 2 1 2]);
assert_checkequal(interp1(x, y, cat(3,[0.5 1.5 2.5],[3.5 4.5 5.5]), m), Ref);  // OErr

// extrapolation "extrap" = "nearest"
for e = ["extrap" "nearest" "edgevalue"]
    assert_checkequal(interp1(x,y, [0 5.4], m, e), cat(4,[0 0.5 ; 6 3.5],[1 2 ; 4 5]));
    assert_checkequal(interp1(x,y,1.5, m, e), cat(4,[2 1.5],[2 3]));         // OWrong values
    assert_checkequal(interp1(x,y,[0.5:4], m, e), y);
    assert_checkequal(interp1(x,y,[0.5:4]', m, e), y);
    Ref = matrix([0 6 2 6 4 6 0.5 3.5 1.5 3.5 2.5 3.5 1 4 2 4 3 4 2 5 3 5 4 5], [2 3 2 1 2]);
    assert_checkequal(interp1(x,y,[0.5 1.5 2.5 ; 3.5 4.5 5.5], m, e), Ref);
    Ref = matrix([0 2 4 6 6 6 0.5 1.5 2.5 3.5 3.5 3.5 1 2 3 4 4 4 2 3 4 5 5 5],[1 3 2 2 1 2]);
    assert_checkequal(interp1(x, y, cat(3,[0.5 1.5 2.5],[3.5 4.5 5.5]), m, e), Ref);
end

// extrapolation by padding
assert_checkequal(interp1(x,y, [0 5.4], m, %pi), %pi*ones(2,2,1,2));
assert_checkequal(interp1(x,y,1.5, m, %pi), cat(4,[2 1.5],[2 3]));
Ref = cat(4, [%pi %pi ; 2 1.5 ; 4 2.5 ; 6 3.5], [%pi %pi ; 2 3 ; 3 4 ; 4 5]);
assert_checkequal(interp1(x,y,[0.5:4], m, %pi), Ref);
assert_checkequal(interp1(x,y,[0.5:4]', m, %pi), Ref);
Ref = matrix([%pi 6 2 %pi 4 %pi %pi 3.5 1.5 %pi 2.5 %pi %pi 4 2 %pi 3 %pi %pi 5 3 %pi 4 %pi],[2 3 2 1 2]);
assert_checkequal(interp1(x,y,[0.5 1.5 2.5 ; 3.5 4.5 5.5], m, %pi), Ref);          // OErr
Ref = matrix([%pi 2 4 6 %pi %pi %pi 1.5 2.5 3.5 %pi %pi %pi 2 3 4 %pi %pi %pi 3 4 5 %pi%pi],[1 3 2 2 1 2]);
assert_checkequal(interp1(x, y, cat(3,[0.5 1.5 2.5],[3.5 4.5 5.5]), m, %pi), Ref);  // OErr

// method = "spline"
// -----------------
m = "spline";
// extrapolation default == "spline" (=="natural")
Ref = cat(4, [-2 -0.5 ; 8.8 4.9], [0 1 ; 5.4 6.4]);
assert_checkalmostequal(interp1(x,y, [0 5.4], m), Ref, 5*%eps);
assert_checkequal(interp1(x,y,1.5, m), cat(4,[1 1],[1.5 2.5]));              // OWrong values
Ref = cat(4,[-1 1 3 5 ; 0 1 2 3]',[0.5:3.5 ; 1.5:4.5]');
assert_checkequal(interp1(x,y,[0.5:4], m), Ref);
assert_checkequal(interp1(x,y,[0.5:4]', m), Ref);
Ref = matrix([-1 5 1 7 3 9 0 3 1 4 2 5 0.5 3.5 1.5 4.5 2.5 5.5 1.5 4.5 2.5 5.5 3.5 6.5],..
             [2 3 2 1 2]);
assert_checkalmostequal(interp1(x,y,[0.5 1.5 2.5 ; 3.5 4.5 5.5], m), Ref, 5*%eps);
Ref = matrix([-1 1 3 5 7 9 0 1 2 3 4 5 0.5 1.5 2.5 3.5 4.5 5.5 1.5 2.5 3.5 4.5 5.5 6.5],..
             [1 3 2 2 1 2]);
assert_checkalmostequal(interp1(x, y, cat(3,[0.5 1.5 2.5],[3.5 4.5 5.5]), m), Ref, 5*%eps);

// extrapolation "extrap" == "spline"
Ref = cat(4, [-2 -0.5 ; 8.8 4.9], [0 1 ; 5.4 6.4]);
assert_checkalmostequal(interp1(x,y, [0 5.4], m, "extrap"), Ref, 5*%eps);    // OWrong size
assert_checkequal(interp1(x,y,1.5, m, "extrap"), cat(4, [1 1], [1.5 2.5]));  // OWrong size
Ref = cat(4, [-1 1 3 5; 0:3]', [0.5:3.5 ; 1.5:4.5]');
assert_checkequal(interp1(x,y,[0.5:4], m, "extrap"), Ref);
assert_checkequal(interp1(x,y,[0.5:4]', m, "extrap"), Ref);
Ref = matrix([-1 5 1 7 3 9 0 3 1 4 2 5 0.5 3.5 1.5 4.5 2.5 5.5 1.5 4.5 2.5 5.5 3.5 6.5],..
             [2 3 2 1 2]);
assert_checkalmostequal(interp1(x,y,[0.5 1.5 2.5 ; 3.5 4.5 5.5], m, "extrap"), Ref, 5*%eps); // OWrong size
Ref = matrix([-1 1 3 5 7 9 0 1 2 3 4 5 0.5 1.5 2.5 3.5 4.5 5.5 1.5 2.5 3.5 4.5 5.5 6.5],..
             [1 3 2 2 1 2]);
assert_checkalmostequal(interp1(x, y, cat(3,[0.5 1.5 2.5],[3.5 4.5 5.5]), m, "extrap"), Ref, 5*%eps);

// extrapolation by padding
assert_checkalmostequal(interp1(x,y, [0 5.4], m, %pi), %pi*ones(2,2,1,2), 5*%eps);
assert_checkequal(interp1(x,y,1.5, m, %pi), cat(4,[1 1],[1.5 2.5]));         // OWrong values
Ref = cat(4,[%pi 1 3 5 ; %pi 1 2 3]',[%pi 1.5 2.5 3.5 ; %pi 2.5 3.5 4.5]');
assert_checkequal(interp1(x,y,[0.5:4], m, %pi), Ref);
assert_checkequal(interp1(x,y,[0.5:4]', m, %pi), Ref);
Ref = matrix([%pi 5 1 %pi 3 %pi %pi 3 1 %pi 2 %pi %pi 3.5 1.5 %pi 2.5 %pi %pi 4.5 2.5 %pi 3.5 %pi], [2 3 2 1 2]);
assert_checkalmostequal(interp1(x,y,[0.5 1.5 2.5 ; 3.5 4.5 5.5], m, %pi), Ref, 5*%eps);
Ref = matrix([%pi 1 3 5 %pi %pi %pi 1 2 3 %pi %pi %pi 1.5 2.5 3.5 %pi %pi %pi 2.5 3.5 4.5 %pi %pi], [1 3 2 2 1 2]);
assert_checkalmostequal(interp1(x, y, cat(3,[0.5 1.5 2.5],[3.5 4.5 5.5]), m, %pi), Ref, 5*%eps);

// extrapolation by edgevalue
e = "edgevalue";
assert_checkequal(interp1(x,y, [0 5.4], m, e), cat(4, [0 0.5 ; 6 3.5],[1 2 ; 4 5]));
assert_checkequal(interp1(x,y,1.5, m, e), cat(4,[1 1],[1.5 2.5]));
Ref = cat(4, [0 1 3 5 ; 0.5 1 2 3]', [1 1.5 2.5 3.5 ; 2 2.5 3.5 4.5]');
assert_checkequal(interp1(x,y,[0.5:4], m, e), Ref);
assert_checkequal(interp1(x,y,[0.5:4]', m, e), Ref);
Ref = matrix([0 5 1 6 3 6 0.5 3 1 3.5 2 3.5 1 3.5 1.5 4 2.5 4 2 4.5 2.5 5 3.5 5], [2 3 2 1 2]);
assert_checkequal(interp1(x,y,[0.5 1.5 2.5 ; 3.5 4.5 5.5], m, e), Ref);
Ref = matrix([0 1 3 5 6 6 0.5 1 2 3 3.5 3.5 1 1.5 2.5 3.5 4 4 2 2.5 3.5 4.5 5 5], [1 3 2 2 1 2]);
assert_checkequal(interp1(x, y, cat(3,[0.5 1.5 2.5],[3.5 4.5 5.5]), m, e), Ref);

// periodic extrapolation
y = cat(4,[0 2 4 0 ; 0.5 1.5 2.5 0.5]', [1 2 3 1 ; 2 3 4 2]');
Ref = cat(4, [4 2.5 ; 3.136 2.068], [3 4 ; 2.568 3.568]);
assert_checkalmostequal(interp1(x,y, [0 5.4], m, "periodic"), Ref, 5*%eps);
assert_checkequal(interp1(x,y,1.5, m, "periodic"), cat(4,[0.625 0.8125],[1.3125 2.3125]));
Ref = cat(4, [3.125 2.0625 ; 0.625 0.8125 ; 3.375 2.1875 ; 3.125 2.0625], ..
             [2.5625 3.5625; 1.3125 2.3125 ; 2.6875 3.6875 ; 2.5625 3.5625]);
assert_checkequal(interp1(x,y,[0.5:4], m, "periodic"), Ref);
assert_checkequal(interp1(x,y,[0.5:4]', m, "periodic"), Ref);
Ref = matrix([3.125 3.125 0.625 0.625 3.375 3.375 2.0625 2.0625 0.8125 0.8125 ..
      2.1875 2.1875 2.5625 2.5625 1.3125 1.3125 2.6875 2.6875 3.5625 3.5625 ..
      2.3125 2.3125 3.6875 3.6875], [2 3 2 1 2]);
assert_checkalmostequal(interp1(x,y,[0.5 1.5 2.5 ; 3.5 4.5 5.5], m, "periodic"), Ref, 5*%eps);
Ref = matrix([3.125 0.625 3.375 3.125 0.625 3.375 2.0625 0.8125 2.1875 2.0625 ..
      0.8125 2.1875 2.5625 1.3125 2.6875 2.5625 1.3125 2.6875 3.5625 2.3125 ..
      3.6875 3.5625 2.3125  3.6875], [1 3 2 2 1 2]);
assert_checkalmostequal(interp1(x, y, cat(3,[0.5 1.5 2.5],[3.5 4.5 5.5]), m, "periodic"), Ref, 5*%eps);


// y is complex
// ============
// method = "linear" (default)
// ---------------------------
assert_checkequal(interp1(0:3, (1:4)+%i, 0.5:2.5), (1.5:3.5)+%i);
// extrapolation linear
assert_checkequal(interp1(0:3,(1:4)+%i, 0.5:2:5, , "extrap"), [1.5 3.5 5.5]+%i);
// Default padding with %nan
assert_checkequal(interp1((1:4)+%i, 0.5:2.5), [%nan, 1.5+%i, 2.5+%i]);
// Padding real
assert_checkequal(interp1(1:3, (2:4)+%i, 0.5:2.5,,7), [7, 2.5+%i, 3.5+%i]);
// Padding complex
assert_checkequal(interp1(1:3, (2:4)+%i, [0.5 2.5 4.5],,7-%i), [7-%i, 3.5+%i, 7-%i]);

// method = "nearest"
// ------------------
assert_checkequal(interp1(0:3, (1:4)+%i, 0.5:2.5, "nearest"), (2:4)+%i);
// Default padding with %nan
assert_checkequal(interp1((1:4)+%i, 0.5:2.5, "nearest"), [%nan, 2+%i, 3+%i]);
// extrapolation with nearest
assert_checkequal(interp1((1:4)+%i, 0.5:2:5, "nearest", "extrap"), [1 3 4]+%i);
// Padding real
assert_checkequal(interp1(1:3,(2:4)+%i, 0.5:2.5, "nearest", 7), [7, 3+%i, 4+%i]);
// Padding complex
assert_checkequal(interp1(1:3,(2:4)+%i, [0.5 2.5 4.5], "nearest",7-%i), [7-%i, 4+%i, 7-%i]);

// method = "spline"
// -----------------
assert_checkequal(interp1(0:3, (1:4)+%i, 0.5:2.5, "spline"), (1.5:3.5)+%i);
// Default padding with spline
assert_checkequal(interp1((1:4)+%i, 0.5:2.5, "spline"), (0.5:2.5)+%i);
assert_checkequal(interp1((1:4)+%i, 0.5:2.5, "spline", "extrap"), (0.5:2.5)+%i);
// Padding real
assert_checkequal(interp1(1:3,(2:4)+%i, 0.5:2.5, "spline", 7), [7, 2.5+%i, 3.5+%i]);
// Padding complex
assert_checkequal(interp1(1:3,(2:4)+%i, [0.5 2.5 4.5], "spline",7-%i), [7-%i, 3.5+%i, 7-%i]);


// ==============
// Error messages
// ==============
msg = _("%s: Wrong number of input arguments: Must be between %d and %d.\n")
msg = msprintf(msg, "interp1", 2, 5);
assert_checkerror("interp1()", msg);
assert_checkerror("interp1(1)", msg);
assert_checkerror("interp1(1,2,3,4,5,6)", msg);
// x
msg = msprintf(_("%s: Argument #%d: Real numbers expected.\n"),"interp1",1);
assert_checkerror("interp1([1 %i],[1 2],1.5)", msg);
msg = msprintf(_("%s: Argument #%d: Vector expected.\n"), "interp1", 1);
assert_checkerror("interp1([1 2; 3 4],[1 2],1.5)", msg);
assert_checkerror("interp1(1,[1 2],1.5)", msg);
msg = msprintf(_("%s: Argument #%d: Nan value forbidden.\n"), "interp1", 1);
assert_checkerror("interp1([1 2 %nan],[1 2],1.5)", msg);
// y
msg = msprintf(_("%s: Argument #%d: vector or at least 2 rows expected.\n"), "interp1", 2);
assert_checkerror("interp1(1,1.5)", msg);
msg = msprintf(_("%s: Argument #%d: Decimal or complex numbers expected.\n"), "interp1",1);
assert_checkerror("interp1([1 %z],1.5)", msg);
assert_checkerror("interp1([%t %f],1.5)", msg);
//
msg = msprintf(_("%s: Arguments #%d and #%d: Same numbers of elements expected.\n"),"interp1", 1, 2);
assert_checkerror("interp1([1 2],1:3,3)", msg);
assert_checkerror("interp1(1:3,1:2,3)", msg);
msg = msprintf(_("%s: Arguments #%d and #%d: Same numbers of rows expected.\n"), "interp1", 1, 2);
assert_checkerror("interp1(1:3,rand(2,3),3)", msg);
    // periodicity
msg = msprintf(_("%s: Argument #%d: periodicity y($)==y(1) expected.\n"), "interp1", 2);
assert_checkerror("interp1(1:4,1:4,3,,""periodic"")", msg);
msg = msprintf(_("%s: Argument #%d: periodicity y($)==y(1) expected.\n"), "interp1", 1);
assert_checkerror("interp1(1:4,3,""linear"",""periodic"")", msg);
msg = msprintf(_("%s: Argument #%d: periodicity y($,:)==y(1,:) expected.\n"), "interp1", 2);
assert_checkerror("interp1(1:3,rand(3,2),3,,""periodic"")", msg);
msg = msprintf(_("%s: Argument #%d: periodicity y($,:)==y(1,:) expected.\n"), "interp1", 1);
assert_checkerror("interp1(rand(3,2),3,""linear"",""periodic"")", msg);

// xp
msg = msprintf(_("%s: Argument #%d: Decimal numbers expected.\n"), "interp1",2);
assert_checkerror("interp1(1:3,[%i 1 1])", msg);
msg = msprintf(_("%s: Argument #%d: Decimal numbers expected.\n"), "interp1",3);
assert_checkerror("interp1(1:3,rand(3,2),%i)", msg);
assert_checkerror("interp1(1:3,2:4,,""linear"")", msg);
// method
msg = msprintf(_("%s: Argument #%d: Scalar (1 element) expected.\n"), "interp1", 3);
assert_checkerror("interp1(1:3,2,[""linear"" ""spline""])", msg);
msg = msprintf(_("%s: Argument #%d: Scalar (1 element) expected.\n"), "interp1", 4);
assert_checkerror("interp1(1:3,2:4,5,[""linear"" ""spline""])", msg);
Set = """linear"",""spline"",""nearest""";
msg = msprintf(_("%s: Argument #%d: Must be in the set {%s}.\n"), "interp1", 3, Set);
assert_checkerror("interp1(1:3,5,""splines"")", msg);
msg = msprintf(_("%s: Argument #%d: Must be in the set {%s}.\n"), "interp1", 4, Set);
assert_checkerror("interp1(2:4,1:3,5,""splines"")", msg);
assert_checkerror("interp1(2:4,1:3,5,%z)", msg);
// extrapolation
msg0 = _("%s: Argument #%d: scalar number or one of {%s} expected.\n");
msg = msprintf(msg0, "interp1", 5, """extrap"",""linear"",""periodic"",""edgevalue""");
assert_checkerror("interp1(1:3,2:4,5,,""abc"")", msg);
assert_checkerror("interp1(1:3,2:4,5,,%z)", msg);
assert_checkerror("interp1(1:3,2:4,5,,1:2)", msg);
assert_checkerror("interp1(1:3,2:4,5,,int8(3))", msg);
msg = msprintf(msg0, "interp1", 5, """extrap"",""edgevalue"",""nearest""");
assert_checkerror("interp1(1:3,2:4,5,""nearest"",""linear"")", msg);
assert_checkerror("interp1(1:3,2:4,5,""nearest"",""periodic"")", msg);
msg = msprintf(msg0, "interp1", 4, """extrap"",""linear"",""periodic"",""edgevalue""");
assert_checkerror("interp1(1:3,2:4,""linear"",""abc"")", msg);
assert_checkerror("interp1(1:3,2:4,""linear"",%z)", msg);
assert_checkerror("interp1(1:3,2:4,""linear"",1:2)", msg);
assert_checkerror("interp1(1:3,2:4,""linear"",int8(3))", msg);
msg = msprintf(msg0, "interp1", 4, """extrap"",""edgevalue"",""nearest""");
assert_checkerror("interp1(1:3,2:4,""nearest"",""linear"")", msg);
assert_checkerror("interp1(1:3,2:4,""nearest"",""periodic"")", msg);
