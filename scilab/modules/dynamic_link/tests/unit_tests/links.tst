// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
// Copyright (C) 2009 - DIGITEO
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- ENGLISH IMPOSED -->
// <-- CLI SHELL MODE -->

ilib_verbose(0);

curPath = pwd();
pref="ext";
suf="f";
routines=["ext1f","ext2f","ext3f","ext8f","ext9f","ext11f","ext12f"];//[pref(ones(1,12))+string(1:12)+suf(ones(1,12))];
copyfile(SCI+filesep()+"modules"+filesep()+"dynamic_link"+filesep()+"tests"+filesep()+"unit_tests"+filesep()+"externals.f", TMPDIR);
chdir(TMPDIR);
ilib_for_link(routines,"externals.f",[],"f");

// load the shared library
exec loader.sce ;

//===========================================================
//(very) simple example 1
//===========================================================
a=[1,2,3];b=[4,5,6];n=3;
c=call("ext1f",n,1,"i",a,2,"d",b,3,"d","out",[1,3],4,"d");
if norm(c-(a+b)) > %eps then pause,end

//===========================================================
//Simple example #2
//===========================================================
a=[1,2,3];b=[4,5,6];n=3;
c=call("ext2f",n,1,"i",a,2,"d",b,3,"d","out",[1,3],4,"d");
if norm(c-(sin(a)+cos(b))) > %eps then pause,end

//===========================================================
//Example #3
//===========================================================
a=[1,2,3];b=[4,5,6];n=3;
c=call("ext3f","yes",1,"c",n,2,"i",a,3,"d",b,4,"d","out",[1,3],5,"d");
if norm(c-(sin(a)+cos(b)))> %eps then pause,end
c=call("ext3f","no",1,"c",n,2,"i",a,3,"d",b,4,"d","out",[1,3],5,"d");
if norm(c-(a+b)) > %eps then pause,end

//===========================================================
//Example #8
//===========================================================
//call ext8f argument function with dynamic link
yref=ode([1;0;0],0,[0.4,4],"ext8f");

//===========================================================
//Example #9
//===========================================================
//passing a parameter to ext9f routine by a list:
param=[0.04,10000,3d+7];
y=ode([1;0;0],0,[0.4,4],list("ext9f",param));
if norm(y-yref) > 10000*%eps then pause,end

//===========================================================
//Example #11
//===========================================================
//sharing common data
a=1:10;
n=10;a=1:10;
call("ext11f",n,1,"i",a,2,"r","out",2);  //loads b with a
c=call("ext12f",n,1,"i","out",[1,10],2,"r");  //loads c with b
if norm(c-a) > %eps then pause,end

//===========================================================
chdir(curPath);
