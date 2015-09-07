// ============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2014 - Scilab Enterprises - Simon MARCHETTO
//
//  This file is distributed under the same license as the Scilab package.
// ============================================================================
//
// <-- CLI SHELL MODE -->

function generate_error(a, b, msg)
    disp(typeof(a));
    disp(a);
    if isequal(a, b) then
        disp(" == ");
    else
         disp(" <> ");
    end
    disp(typeof(b));
    disp(b);
    assert_generror(msg);
end

function check_inter_type(type1_values, type2_values)
    if isequal(type1_values(2), type2_values(2)) then
        msg = msprintf("check_inter_type fails: values %s(%d) and %s(%d) must not be equal !", type1_values(1), 2, type2_values(1), 2);
        generate_error(type1_values(2), type2_values(2), msg);
    end
end

function check_intra_type(type_values)
    for i=2:size(type_values)
        for j=i:size(type_values)
            if i <> j then
                if isequal(type_values(i), type_values(j)) then
                    msg = msprintf("check_intra_type fails: values %s(%d) and %s(%d) must not be equal !", ..
                      type_values(1), i, type_values(1), j);
                    generate_error(type_values(i), type_values(j), msg);
                end
            else
                if ~isequal(type_values(i), type_values(j)) then
                    msg = msprintf("check_intra_type fails: values %s(%d) and %s(%d) must be equal !", ..
                      type_values(1), i, type_values(1), j);
                    generate_error(type_values(i), type_values(j), msg);
                end
            end
        end
    end
end

function check_types(types)
    for i=1:size(types)
        for j=i+1:size(types)
            check_inter_type(types(i), types(j));
        end
        check_intra_type(types(i));
    end
end

// "constant" type
real_scalars = list(1, -1, 3.2, 3.2000005, %pi, %e, %inf, -%inf, %eps, []);
real_matrices = list([1.1, 2.2], [1.1 2.2; 3.3 4.4], [1.1 2.2; 3.3 4.4; 5.5 6.6], [1.1 2.2 3.3; 4.4 5.5 6.6]);
reals = lstcat(real_scalars, real_matrices);

// TODO: check %nan is not equal to anything

complex_scalars = list(%i, 3.2*%i, -%i, 1+0.5*%i);
complex_matrices = list([1+%i, 2+2*%i], [1+%i 2+2*%i; 3+3*%i 4+4*%i], ..
    [1+%i 2+2*%i; 3+3*%i 4+4*%i; 5+5*%i 6+6*%i], [1+%i 2+2*%i 3+3*%i; 4+4*%i 5+5*%i 6+6*%i]);
complexs = lstcat(complex_scalars, complex_matrices);

constants = lstcat(reals, complexs);
constants(0) = 'constants';

// "polynomial" type
polynoms = list(%s, -%s, %s+1, 1+%s+%s^2, 1+%i+%s-%s^2, 1+%s+%i*%s^3);
polynoms(0) = 'polynoms';

// boolean type
bool_scalars = list(%t, %f);
bool_matrices = list([%t, %f], [%t %f; %t %f], [%t %f; %t %f; %t %f], [%t %f %t; %t %f %t]);
bools = lstcat(bool_scalars, bool_matrices);
bools(0) = 'bools';

// "sparse" type
sparses = list(sparse([1]), sparse([0]), sparse([0 0]), sparse([0 0; 0 0]), sparse([1 2]), ..
    sparse([1 2; 3 4]), sparse([-1 2; 3 4]), sparse([1 2; 3 -1]), sparse([1 2; 3 0]), sparse([0 2; 3 4]), sparse([0 1; 2 3]));
sparses(0) = 'sparses';

// "boolean sparse" type
boolean_sparses = list(sparse([]), sparse([%t]), sparse([%f]), sparse([%f %f]), sparse([%f %f; %f %f]), sparse([%t %t]), ..
    sparse([%t %t; %t %t]), sparse([%f %t; %t %t]), sparse([%t %t; %t %f]));
boolean_sparses(0) = 'boolean_sparses';

// integer types
int8s_scalars = list(int8(1), int8(-1));
int8s_matrices = list(int8([1, 2]), int8([1 2; 3 4]), int8([1 2; 3 4; 5 6]), int8([1 2 3; 4 5 6]));
int8s = lstcat(int8s_scalars, int8s_matrices);

function int_type_values = convert_to_int_type(type_values, int_type)
    int_type_values = list();
    for i=1:size(type_values)
        int_type_values($+1) = iconvert(type_values(i), int_type);
    end
end

uint8s = convert_to_int_type(int8s, 11);
int16s = convert_to_int_type(int8s, 2);
uint16s = convert_to_int_type(int8s, 12);
int32s = convert_to_int_type(int8s, 4);
uint32s = convert_to_int_type(int8s, 14);
int64s = convert_to_int_type(int8s, 8);
uint64s = convert_to_int_type(int8s, 18);

ints = lstcat(int8s, uint8s, int16s, uint16s, int32s, uint32s, int64s, uint64s);
ints(0) = 'ints';

// "handle" type

// "string" type
string_scalars = list("", " ", "a", "A", "à", ascii(13), "aaaa", "aaab", "aaaa" + ascii(13));
string_matrices = list(["", ""], [" ", " "], ["a", "a"], ["a", "A"], ["a", "à"], ..
    ["a" "b"; "c" "d"], ["a" "b"; "c" "d"; "e" "f"], ["a" "b" "c"; "d" "e" "f"]);
strings = lstcat(string_scalars, string_matrices);
strings(0) = 'strings';

// "function" type
function funcNoParamNoBodyNoReturn(); end;
function funcWithParamNoBodyNoReturn(x); end;
function funcNoParamWithBodyNoReturn(); disp("Hello"); end;
function funcWithParamWithBodyNoReturn(x); disp(x); end;
function y = funcNoParamWithReturn(); y = 1; end;
function y = funcWithParamWithReturn(x); y = x; end;

functions = list(funcNoParamNoBodyNoReturn, ..
    funcWithParamNoBodyNoReturn, ..
    funcNoParamWithBodyNoReturn, ..
    funcWithParamWithBodyNoReturn, ..
    funcNoParamWithReturn, ..
    funcWithParamWithReturn);
functions(0) = 'functions';

// "fptr" type
fptrs = list(cos, sin);
fptrs(0) = 'fptrs';

// "list" type
lists = list(list(), list(1), list("a"), list(1, 2), list(list(1)), list(list(2)), list(list(1, 2)));
lists(0) = 'lists';

// "tlist" type
tlists = list(tlist(["t1"]), tlist(["t2"]), tlist(["t1", "f1"], "a"), tlist(["t1", "f2"], "a"), ..
    tlist(["t1", "f1", "f2"], "a", "b"), tlist(["t1", "f1"], "b"));
tlists(0) = 'tlists';

// "mlist" type
mlists = list(mlist(["m1"]), mlist(["m2"]), mlist(["m1", "f1"], "a"), mlist(["m1", "f2"], "a"), ..
    mlist(["m1", "f1", "f2"], "a", "b"), mlist(["m1", "f1"], "b"));
mlists(0) = 'mlists';

// "st" type
structs = list(struct("a", 1), struct("a", 2), struct("b", 1), struct("a", 1, "b", 2));
structs(0) = 'structs';

// "pointer" type

// TODO: add functions, fptrs, handles (when operator== will be fixed in that types)
types = list(constants, polynoms, bools, sparses, boolean_sparses, ints, strings, lists, tlists, mlists, structs);

check_types(types);


