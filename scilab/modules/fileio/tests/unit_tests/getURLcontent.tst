// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 - Sylvestre LEDRU
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

//getURL

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

[f, HTMLContent]=getURL("http://httpbin.org/basic-auth/user/passwd","user","passwd");
assert_checktrue(length(HTMLContent) > 10);
assert_checktrue(grep(HTMLContent,"authenticated") <> []);

// Badly formated URL
assert_checkerror("getURL(''http://plop@ae:www.scilab.org:80'');", [], 999);
