// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Sylvestre LEDRU
// Copyright (C) 2011 - DIGITEO - Antoine ELIAS
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- ENGLISH IMPOSED -->
// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->

// unit tests for structs
// =============================================================================

date_st=struct('jour',25,'mois','DEC','annee',2006);

if date_st.jour <> 25 then pause, end
if date_st.mois <> 'DEC' then pause, end
if date_st.annee <> 2006 then pause, end

date_st.jour=19;
if date_st.jour <> 19 then pause, end

date_st.mois='AOU';
if date_st.mois <> 'AOU' then pause, end

date_st.annee=1973;
if date_st.annee <> 1973 then pause, end

date_st.semaine=32;
if date_st.semaine <> 32 then pause, end

// Example from bug #7244
clear;
foo(1) = 1;
foo(2) = 2;
foo(3) = 3;
foo = foo([1 3]);
foo(3) = 2;
if [1; 3; 2] <> foo then pause, end

clear;
foo(1) = 1;
foo(2) = 2;
foo(3) = 3;
foo = foo([%T %F %T]);
foo(3) = 2;
if or([1; 3; 2] <> foo) then pause, end

clear;
foo(1).bar = 1;
foo(2).bar = 2;
foo(3).bar = 3;
foo = foo([1 3]);
foo(3).bar = 2;
if foo(1).bar <> 1 then pause, end
if foo(2).bar <> 3 then pause, end
if foo(3).bar <> 2 then pause, end

out.a=1;
out.b=2;
if size(out,'*') <> 1 then pause, end

out(2,3).b=2;
if or(size(out) <> [2 3]) then pause, end

s=struct("txt","Hello","num",%pi,"pol",%z^2+1);
if s.pol <> %z^2+1 then pause, end
if s.txt <> "Hello" then pause, end

s.txt=null();s.num=null();s.pol=null();
if isfield( s , "txt"  ) then pause, end
if isfield( s , "num"  ) then pause, end
if isfield( s , "pol"  ) then pause, end
if or(isfield( s , ["pol", "num", "txt"]  )) then pause, end
if or(size(s) <> [1 1]) then pause, end

z.y = 42;
y.o = z;
y.d.e = z;
y.f.r.h = z;
y.i.j.k.l = z;
z.m = y;
z.m.o.y = z;
if z.m.o.y.m.i.j.k.l.y <> 42 then pause, end
if ~isfield(z.m.o.y.m.i.j.k.l,"y") then pause, end

z.z = 21;
z.b = z;
z.b.c = z;
if z.z <> 21 then pause, end
if z.b.c.z <> 21 then pause, end
if ~isfield(z,"b") then pause, end
if ~isfield(z.b,"c") then pause, end

z(1).x.x = 1;
if execstr('z(42).x','errcatch')<>999 then pause,end

// -----------------------------------------------------------------

//function struct

firstname       = "firstname";
firstname_new   = "firstname_new";
lastname        = "lastname";
lastname_new    = "lastname_new";
email           = "email";
email_new       = "email_new";
phone_new       = "phone_new";

// create a one dimensional struct
st = struct("firstname", firstname, "lastname", lastname, "email", email);

assert_checkequal(st.firstname, firstname);
assert_checkequal(st.lastname, lastname);
assert_checkequal(st.email, email);

//change some values
st.firstname    = firstname_new;
st.lastname     = lastname_new;
st.email        = email_new;

// add a phone field
st.phone        = phone_new;

assert_checkequal(st.firstname, firstname_new);
assert_checkequal(st.lastname, lastname_new);
assert_checkequal(st.email, email_new);
assert_checkequal(st.phone, phone_new);
clear st;

//create multi-dimentional struct (4x3x2)
n1 = 4;
n2 = 3;
n3 = 2;

firstnames{n1,n2,n3}    = [];
lastnames{n1,n2,n3}     = [];
emails{n1,n2,n3}        = [];


for i = 1:(n1*n2*n3)
    sz              = string(i);
    firstnames{i}   = "firstname_" + sz;
    lastnames{i}    = "lastname_" + sz;
    emails{i}       = "email_" + sz;
end

st = struct("firstname", firstnames, "lastname", lastnames, "email", emails);

for i = 1:24
    if st(i).firstname <> firstnames{i} then pause end
    if st(i).lastname <> lastnames{i} then pause end
    if st(i).email <> emails{i} then pause end
end
clear firstnames lastnames emails st

//creating one dimensional structure by insertion
firstname   = "firstname";
lastname    = "lastname";
email       = "email";

st.firstname    = firstname;
st.lastname     = lastname;
st.email        = email;

assert_checkequal(st.firstname, firstname);
assert_checkequal(st.lastname, lastname);
assert_checkequal(st.email, email);
clear st;

//create multi dimentional struct by insertion
n1 = 4;
n2 = 3;
n3 = 2;

firstnames(n1,n2,n3)    = "";
lastnames(n1,n2,n3)     = "";
emails(n1,n2,n3)        = "";


for i = 1:(n1*n2*n3)
    sz              = string(i);
    firstnames(i)   = "firstname_" + sz;
    lastnames(i)    = "lastname_" + sz;
    emails(i)       = "email_" + sz;
end

//set dimension to 4x3x2
st(4,3,2).firstname = firstnames($);
st(4,3,2).lastname = lastnames($);

//fill struct as vector
for i = 1:(n1*n2*n3)
    st(i).firstname = firstnames(i);
    st(i).lastname  = lastnames(i);
    st(i).email     = emails(i);
end

//check data on each dimension
for i = 1:n1
    for j = i:n2
        for k = 1:n3
            assert_checkequal(st(i,j,k).firstname, firstnames(i,j,k));
            assert_checkequal(st(i,j,k).lastname, lastnames(i,j,k));
            assert_checkequal(st(i,j,k).email, emails(i,j,k));
        end
    end
end
clear firstnames lastnames emails st

//fields of an empty struct
fields = getfield(1,struct());
assert_checkequal(fields(1), "st");
assert_checkequal(fields(2), "dims");

// struct concatenation
clear st;
st.a = [];
st.a = [st.a;struct("field1", 1, "field2", 2)];
st.a = [st.a;struct("field1", 1, "field2", 2)];
st.a(1).field1 = 12;
st.a(1).field2 = 42;
assert_checkequal(st.a(1).field1, 12);
assert_checkequal(st.a(1).field2, 42);
