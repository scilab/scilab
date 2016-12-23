// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2015 - Scilab Enterprises - Paul Bignier
// Copyright (C) ????-2008 - INRIA
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

a = list("cos", [1.1 2]);
b = var2vec(a);
assert_checkequal(type(b), 1);
assert_checkequal(size(b), [15 1]);

// Double
a = [];
assert_checkequal(var2vec(a), [1; 2; 0; 0; 0]);
Mat = [10 11 12 13; 14 15 16 17];
assert_checkequal(var2vec(Mat), [1; 2; 2; 4; 0; 10; 14; 11; 15; 12; 16; 13; 17]);
HyperMat = matrix(1:8, [2, 2, 2]);
assert_checkequal(var2vec(HyperMat), [1; 3; 2; 2; 2; 0; (1:8)']);
cplx = HyperMat+HyperMat*%i;
assert_checkequal(var2vec(cplx), [1; 3; 2; 2; 2; 1; (1:8)'; (1:8)']);

// Integers
// int8
Mat8 = int8(Mat);
assert_checkequal(vec2var(var2vec(Mat8)), Mat8);
HyperMat8 = int8(HyperMat);
assert_checkequal(vec2var(var2vec(HyperMat8)), HyperMat8);
// uint8
uMat8 = uint8(Mat);
assert_checkequal(vec2var(var2vec(uMat8)), uMat8);
uHyperMat8 = uint8(HyperMat);
assert_checkequal(vec2var(var2vec(uHyperMat8)), uHyperMat8);
// int16
Mat16 = int16(Mat);
assert_checkequal(vec2var(var2vec(Mat16)), Mat16);
HyperMat16 = int16(HyperMat);
assert_checkequal(vec2var(var2vec(HyperMat16)), HyperMat16);
// uint16
uMat16 = uint16(Mat);
assert_checkequal(vec2var(var2vec(uMat16)), uMat16);
uHyperMat16 = uint16(HyperMat);
assert_checkequal(vec2var(var2vec(uHyperMat16)), uHyperMat16);
// int32
Mat32 = int32(Mat);
assert_checkequal(vec2var(var2vec(Mat32)), Mat32);
HyperMat32 = int32(HyperMat);
assert_checkequal(vec2var(var2vec(HyperMat32)), HyperMat32);
// uint32
uMat32 = uint32(Mat);
assert_checkequal(vec2var(var2vec(uMat32)), uMat32);
uHyperMat32 = uint32(HyperMat);
assert_checkequal(vec2var(var2vec(uHyperMat32)), uHyperMat32);

// Bool
bMat = (Mat>15);
assert_checkequal(vec2var(var2vec(bMat)), bMat);
bHyperMat = (HyperMat>4);
assert_checkequal(vec2var(var2vec(bHyperMat)), bHyperMat);

// String
sMat = string(Mat);
assert_checkequal(vec2var(var2vec(sMat)), sMat);
sHyperMat = string(HyperMat);
assert_checkequal(vec2var(var2vec(sHyperMat)), sHyperMat);

// List
// Double
l = list();
assert_checkequal(vec2var(var2vec(l)), l);
lEmpties = list([], "", [], "");
assert_checkequal(vec2var(var2vec(lEmpties)), lEmpties);
lMat = list(Mat);
assert_checkequal(vec2var(var2vec(lMat)), lMat);
lHyperMat = list(HyperMat);
assert_checkequal(vec2var(var2vec(lHyperMat)), lHyperMat);
// Integer
lMat8 = list(int8(Mat));
assert_checkequal(vec2var(var2vec(lMat8)), lMat8);
lHyperMat8 = list(int8(HyperMat));
assert_checkequal(vec2var(var2vec(lHyperMat8)), lHyperMat8);
// Bool
lbMat = list(Mat>15);
assert_checkequal(vec2var(var2vec(lbMat)), lbMat);
lbHyperMat = list(HyperMat>4);
assert_checkequal(vec2var(var2vec(lbHyperMat)), lbHyperMat);
// String
lsMat = list(string(Mat));
assert_checkequal(vec2var(var2vec(lsMat)), lsMat);
lsHyperMat = list(string(HyperMat));
assert_checkequal(vec2var(var2vec(lsHyperMat)), lsHyperMat);

// TList
tl = tlist("");
assert_checkequal(vec2var(var2vec(tl)), tl);
tlEmpties = tlist(["Type" "f1" "f2" "f3" "f4" "f5", "f6"]);
assert_checkequal(vec2var(var2vec(tlEmpties)), tlEmpties);
tlMixed = tlist(["Type" "f1" "f2" "f3" "f4" "f5", "f6"], [], Mat, HyperMat, int32(Mat), string(Mat), Mat>15);
assert_checkequal(vec2var(var2vec(tlMixed)), tlMixed);

// MList
ml = mlist("");
assert_checkequal(vec2var(var2vec(ml)), ml);
mlEmpties = mlist(["Type" "f1" "f2" "f3" "f4" "f5", "f6"]);
assert_checkequal(vec2var(var2vec(mlEmpties)), mlEmpties);
mlMixed = mlist(["Type" "f1" "f2" "f3" "f4" "f5", "f6"], [], Mat, HyperMat, int32(Mat), string(Mat), Mat>15);
assert_checkequal(vec2var(var2vec(mlMixed)), mlMixed);

// Struct
//st = struct();
//assert_checkequal(vec2var(var2vec(st)), st);
//stFull = struct("f1", [], "f2", Mat, "f3", HyperMat, "f4", int32(Mat), "f5", string(Mat), "f6", Mat>15);
//assert_checkequal(vec2var(var2vec(stFull)), stFull);

// List containing a full TList, taken from PDE.sci
params_pde = tlist(["paramspde";"a";"b";"txt_exp";"check_op1";"a1";"b1";"check_op2";"a2";"b2";..
"check_op3";"a3";"b3";"check_op4";"a4";"b4";"check_op5";"a5";"b5";"check_op6";"a6";"b6";..
"check_op7";"a7";"b7";"discr_cst";"discr_non_cst";"signe";"rad_automatique";"rad_manuel";..
"methode";"ord1";"ord2";"ord3";"degre";"nnode";"txt_pas";"CI";"dCI";"CLa";"CLa_exp";"CLb";..
"CLb_exp";"points"],"","","","0","","IN_EDP1(t)","0","","IN_EDP2(t)","0","","IN_EDP3(t)",..
"0","","IN_EDP4(t)","0","","IN_EDP5(t)","0","","IN_EDP6(t)","0","","IN_EDP7(t)","0","0",..
"0","0","0","0","","","","","","","","","0","IN_CL1(t)","0","IN_CL2(t)","");
label = list(params_pde, [], "");
assert_checkequal(vec2var(var2vec(params_pde)), params_pde);
assert_checkequal(vec2var(var2vec(label)), label);

// TList containing a List, taken from MBLOCK.sci
in = ["u1"];
intype = ["I"];
out = ["y1";"y2"];
outtype = ["I";"E"];
param = ["R";"L"];
paramv = list(0.1,.0001);
pprop = [0;0];
nameF = "generic";
exprs = tlist(["MBLOCK","in","intype","out","outtype",...
"param","paramv","pprop","nameF","funtxt"],...
sci2exp(in(:)),..
sci2exp(intype(:)),..
sci2exp(out(:)),..
sci2exp(outtype(:)),..
sci2exp(param(:)),..
list(string(0.1),string(.0001)),...
sci2exp(pprop(:)),..
nameF,[]);
assert_checkequal(vec2var(var2vec(exprs)), exprs);

// List containing a Struct
//lStFull = list(stFull, [], stFull, "test");
//assert_checkequal(vec2var(var2vec(lStFull)), lStFull);

// corner-cases
a = var2vec(tlist(["" ""],list()));
vec2var(a);

//a = var2vec(struct("toto", list(struct("titi", 12))));
//vec2var(a);

