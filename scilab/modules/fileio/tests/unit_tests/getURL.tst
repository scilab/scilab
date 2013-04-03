// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 - Sylvestre LEDRU
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

//getURL

cd TMPDIR;
myFile=getURL("http://www.scilab.org");
assert_checkequal(myFile, "index.html")
inf=fileinfo(myFile);
assert_checktrue(inf(1) > 1000); // The homepage of Scilab is likely to be bigger than 1k

myFile=getURL("http://www.scilab.org/");
assert_checkequal(myFile, "index.html")
inf=fileinfo(myFile);
assert_checktrue(inf(1) > 1000); // The homepage of Scilab is likely to be bigger than 1k


myFile=getURL("ftp://ftp.free.fr/pub/Distributions_Linux/debian/README");

assert_checkequal(myFile, "README");
inf=fileinfo(myFile);
assert_checktrue(inf(1) > 10); // The file size should be more than 10


targetFile=TMPDIR+"/README_Debian";
myFile=getURL("ftp://ftp.free.fr/pub/Distributions_Linux/debian/README",targetFile);

assert_checkequal(myFile, targetFile);
inf=fileinfo(targetFile);
assert_checktrue(inf(1) > 10); // The file size should be more than 10
f1 = mopen(targetFile,"r");
assert_checktrue(grep(mgetl(f1),"Linux") <> []);
mclose(f1);


targetFile="README";
myFile=getURL("ftp://ftp.free.fr/pub/Distributions_Linux/debian/README",TMPDIR);

assert_checkequal(myFile, TMPDIR+"/"+targetFile);
inf=fileinfo(TMPDIR+"/"+targetFile);
assert_checktrue(inf(1) > 10); // The file size should be more than 10
f1 = mopen(TMPDIR+"/"+targetFile,"r");
assert_checktrue(grep(mgetl(f1),"Linux") <> []);
mclose(f1);

// HTTPS
myFile=getURL("https://encrypted.google.com");
assert_checkequal(myFile, "index.html");
inf=fileinfo(myFile);
assert_checktrue(inf(1) > 10); // The file size should be more than 10

targetFile=TMPDIR+"/testauth";
myFile=getURL("http://httpbin.org/basic-auth/user/passwd",targetFile,"user","passwd");

assert_checkequal(myFile, targetFile);
inf=fileinfo(targetFile);
assert_checktrue(inf(1) > 10); // The file size should be more than 10
f1 = mopen(targetFile,"r");
assert_checktrue(grep(mgetl(f1),"authenticated") <> []);
mclose(f1);

// Badly formated URL
assert_checkerror("getURL(''http://plop@ae:www.scilab.org:80'');", [], 999);

[f, HTMLContent]=getURL("http://www.scilab.org:80");
assert_checktrue(length(HTMLContent) > 10);
assert_checktrue(grep(HTMLContent,"html") <> []);

[f, HTMLContent]=getURL("http://plop:ae@www.scilab.org:80");
assert_checktrue(length(HTMLContent) > 10);
assert_checktrue(grep(HTMLContent,"html") <> []);

[f, HTMLContent]=getURL("http://www.scilab.org/aze");
assert_checktrue(length(HTMLContent) > 10);
assert_checkequal(grep(HTMLContent,"404"), []);



[f, HTMLContent]=getURL("http://www.scilab.org");
assert_checktrue(length(HTMLContent) > 10);
assert_checktrue(grep(HTMLContent,"html") <> []);

[f, HTMLContent]=getURL("http://www.scilab.org/");
assert_checktrue(length(HTMLContent) > 10);
assert_checktrue(grep(HTMLContent,"html") <> []);

[f, HTMLContent]=getURL("ftp://ftp.free.fr/pub/Distributions_Linux/debian/README");
assert_checktrue(length(HTMLContent) > 0);
assert_checktrue(grep(HTMLContent,"Linux") <> []);

// HTTPS
[f, HTMLContent]=getURL("https://encrypted.google.com");
assert_checktrue(length(HTMLContent) > 100);

[f, HTMLContent]=getURL("http://httpbin.org/basic-auth/user/passwd",TMPDIR,"user","passwd");
assert_checktrue(length(HTMLContent) > 10);
assert_checktrue(grep(HTMLContent,"authenticated") <> []);

// Badly formated URL
assert_checkerror("getURL(''http://plop@ae:www.scilab.org:80'');", [], 999);
