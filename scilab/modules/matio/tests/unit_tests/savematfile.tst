// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Vincent Couvert <vincent.couvert@inria.fr>
// Copyright (C) 2018, 2019 - Samuel GOUGEON
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->
//
// unitary tests for savematfile() (using loadmatfile()..)
//  => in binary mode
//
// ===============================================================
// Naming conventions:
// ------------------
// Initials of variables names:
//  - "b" = booleans
//  - "d" = decimal real numbers
//  - "c" = complex numbers
//  - "spd" = sparse arrays of decimal real numbers
//  - "spc" = sparse arrays of complex numbers
//  - "spb" = sparse arrays of booleans
//  - "int"|"uint" = encoded integers
//  - "t" = text arrays
//  - "ce" = cells arrays
//  - "s"  = structures arrays
//
// Last character of variable names:
//  - "s" = Scalar
//  - "v" = Vectors
//  - "m" = Matrices
//  - "h" = Hyperarrays

// ---------------------------------------------------------------

clear
i = sqrt(-1);

// Real numbers
// ============
Empty = [];
ds = (rand(1,1)-0.5)*100;
dv = (rand(1,3)-0.5)*100;
dm = (rand(2,3)-0.5)*100;
dh = (rand(1,3,2)-0.5)*100;

// Complex numbers
// ===============
cs = ds*(1+i);
cv = dv*(1+i);
cm = dm*(1+i);
ch = dh*(1+i);

// Sparse reals
// ------------
spEmpty = sparse([]);
spds = sparse(ds);
spdv = sparse(dv);
spdm = sparse(dm);
// v4 save Scilab OK (Octave)  load Scilab crash    sparse([])=>[]  (savematfile)
// v6,7,7.3: save OK. load OK in both sides.        sparse([])=>[]
// -v4 load crash: http://bugzilla.scilab.org/15731

// Sparse complexes
// ----------------
spcs = sparse(cs);
spcv = sparse(cv);
spcm = sparse(cm);
// v4,6,7,7.3: save OK: read from Octave. load crash // http://bugzilla.scilab.org/15731

// Booleans
// ========
// Supported only in version 4
// Versions 6, 7, 7.3: http://bugzilla.scilab.org/15568 (missing support)
bs = abs(ds)<25
bv = abs(dv)<25
bm = abs(dm)<25
bh = abs(dh)<25

// Booleans sparse
// ---------------
// Not supported by the versions 6, 7, 7.3.
spbs = sparse(bs);
spbv = sparse(bv);
spbm = sparse(bm);
// v4: Saved as sparse doubles 0|1 (read from Octave)

// Encoded integers
// ================
// Supported in versions 6, 7 and 7.3, NOT in 4
// int8 uint8
int8s = int8(ds);
int8v = int8(dv);
int8m = int8(dm);
int8h = int8(dh);
uint8s = uint8(abs(ds));
uint8v = uint8(abs(dv));
uint8m = uint8(abs(dm));
uint8h = uint8(abs(dh));

// int16 uint16
int16s = int16(ds);
int16v = int16(dv);
int16m = int16(dm);
int16h = int16(dh);
uint16s = uint16(abs(ds));
uint16v = uint16(abs(dv));
uint16m = uint16(abs(dm));
uint16h = uint16(abs(dh));

// int32 uint32
int32s = int32(ds);
int32v = int32(dv);
int32m = int32(dm);
int32h = int32(dh);
uint32s = uint32(abs(ds));
uint32v = uint32(abs(dv));
uint32m = uint32(abs(dm));
uint32h = uint32(abs(dh));

// int64 uint64
int64s = int64(ds);
int64v = int64(dv);
int64m = int64(dm);
int64h = int64(dh);
uint64s = uint64(abs(ds));
uint64v = uint64(abs(dv));
uint64m = uint64(abs(dm));
uint64h = uint64(abs(dh));

// TEXT
// ====
EmptyStr = "";
ts = "Bonjour";
tv = ["a" "bc" "def" "ghij"];
tm = ["a" "bc" "def" ; "ghij" "klm" "no"];
th = cat(3,tv,tv);
// v4: row, matrix, hypermatrix => column (right-padded with spaces)
// TODO: Add tests with UTF8 (after fixing http://bugzilla.scilab.org/15570 )
// Issues for Text columns in v7: http://bugzilla.scilab.org/15569

// CELLS ARRAYS
// ============
// Bug: Booleans not accepted in cells: http://bugzilla.scilab.org/15727
EmptyC = {};
ces = {rand(2,3)};
cev = { "ABC", rand(1,3,2)};
cem = {1.1, int8(-5); rand(10,10), "abc"};
ceh = cat(3,cev, cev);
ceNested = {cev, cem};
ceWithSparse = {1.1, int8(-5); sprand(10,10,0.1), "abc"};
// v4: not accepted
// v6,7,7.3: save/load OK (Scilab & read from Octave)(Octave does not read 7.3)

// STRUCTURES
// ==========
// v4  not accepted
// http://bugzilla.scilab.org/15730
s0 = struct();
s0f.r = struct();
ss.r = %pi;             // Scalar structure
sv(1,2).r = rand(1,3);  // Vector of structures
sm(2,3).r = %e;         // Matrix of structures
struc = struct("age",30, "type","software");
// TODO: add more complex cases after fixing http://bugzilla.scilab.org/15730
// Case from http://bugzilla.scilab.org/6372 : only with v7.3
savgg_mes.x_values = struct("quantity", struct("label","Hz"), ..
                           "values", [], ..
                           "start_value", 1, ..
                           "increment", 4, ..
                           "number_of_values", 125);
savgg_mes.y_values = savgg_mes.x_values;
savgg_mes.function_record = struct("r",%pi);
//
structS = struct('f1', 10, 'ftwo', 'Hello', 'field3', int8(12));
structRow = struct('field1', 10, 'field2', 'Hello', 'field3', int8(12));
structRow(1,2).field1 = 'test';
structRow(1,2).field2 = eye(10, 10);
structRow(1,3).field2 = 'a f*%ield contents';
structRow(1,3).field3 = 1.23+4.56*%i;
structCol = structRow';
structMat = struct('field1', 10, 'field2', 'Hello', 'field3', int8(12));
structMat(1,2).field1 = 'test';
structMat(1,2).field2 = eye(10, 10);
structMat(1,3).field2 = 'a field contents';
structMat(1,3).field3 = 1.23+4.56*%i;
structMat(2,1).name = 'test';
structMat(2,1).phone = eye(10, 10);
structMat(3,1).phone = 'a field contents';
structMat(3,1).address = 1.23+4.56*%i;


// ===============================================================
// Collects names of all defined variables
varnames = who_user(%f);
// Remove unwished variables coming from the processing
varnames(grep(varnames,"/^"+["i"]+"$/","r")) = [];
// Start tests from a given name (to shorten)
//varnames(strcmp(varnames, "tv")<0) = [];

sleepT = 100; // ms
File = TMPDIR + "/tmp.mat";
ver = ["-v4" "-v6" "-v7" "-v7.3"];
// List of known problems ==> skipped
pbs = [ "EmptyStr" "-v7"
        "EmptyStr" "-v7.3"
        "s0"       "*"        // crash for all versions
        "s0f"      "*"        // crash for all versions
        "struc"    "-v7.3"
        "savgg_mes" "-v7.3"
        "bh"       "-v4"      // Only the first page is saved
        "bm"       "-v4"      // recovered as 0|1 instead of booleans
        "bv"       "-v4"      // "
        "bs"       "-v4"      // "
        "ceWithSparse" "*"    // loadmatfile bug http://bugzilla.scilab.org/15731
        "ch"       "-v4"      // Only the first page is saved
        "dh"       "-v4"      // Only the first page is saved
        "int64m"   "*"
        "int64s"   "*"
        "int64v"   "*"
        "structS"  "-v7"      // loadmatfile bug http://bugzilla.scilab.org/16256
        "structS"  "-v7.3"    // "
        "th"       "-v7.3"
        "tm"       "-v7.3"
        "tv"       "-v7.3"
        "ts"       "-v7.3"
      ];
// Cases with exclusive versions to be tested
only = ["savgg_mes"  "-v7.3"
        "structRow" "-v7.3"
        "structCol" "-v7.3"
        "structMat" "-v7.3"
        "bs"        "-v4"     // http://bugzilla.scilab.org/15568
        "bv"        "-v4"     //  "
        "bm"        "-v4"     //  "
        "bh"        "-v4"     //  "
    ];

for n = varnames'
    onlyVersion = only(find(only(:,1)==n),2);
    for v = ver
        execstr("t=type("+n+"); to=typeof("+n+");")
        if or(t==[13 130])  // functions not supported
            break
        end
        
        mprintf("\n%s %s",n,v);

        if ((t==8 | to=="ce"| to=="st") &  v=="-v4") | ..// Integers, cells, structs only in version >= 6
            t==6 & v~="-v4" | ..       // Sparse boolean (only with v4)
            (onlyVersion~=[] & onlyVersion~=v)
            mprintf(" : Not supported");
            continue
        else
            if vectorfind(pbs, [n "*"])~=[] | vectorfind(pbs, [n v])~=[]
                mprintf(" : Not fixed");
                continue
            end
        end

        ierr = execstr("savematfile(File, v, n);","errcatch");
        sleep(sleepT);
        assert_checkequal(ierr,0);
        if t==4 & v~="-v4"      // Booleans
            continue
        end

        if t==5                  // Sparse
            execstr("r=isreal("+n+");");
            if ~r | v=="-v4"
                continue
            end
        end

        execstr("ref = "+n+";");
        clear(n);
        loadmatfile(File);
        sleep(sleepT);
        err = execstr("assert_checktrue(isdef(n,""l""));", "errcatch");
        if err
            disp(["-------" n v]);
            continue
        end
        
        if t==6        // Sparse boolean
            execstr("assert_checkequal(bool2s(ref),"+n+");");
        elseif t==10   // Text
            execstr("assert_checkequal(justify(ref(:),''l''),"+n+");");
        else
            execstr("assert_checkequal(ref,"+n+")");
        end
    end
end
