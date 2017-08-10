//<-- CLI SHELL MODE -->
// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 - Sylvestre LEDRU
// Copyright (C) 2013 - Scilab Enterprises
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

function checkFile(filePath, expectedFilePath, minimalFileSize)
    assert_checkequal(filePath, expectedFilePath)
    info = fileinfo(filePath);
    assert_checktrue(info(1) > minimalFileSize);
    deletefile(filePath);
endfunction

function checkFileAndContent(filePath, expectedFilePath, minimalFileSize, keywordToFind)
    fd = mopen(filePath, "r");
    assert_checktrue(grep(mgetl(fd), keywordToFind) <> []);
    mclose(fd);
    checkFile(filePath, expectedFilePath, minimalFileSize);
endfunction

function checkContent(filePath, content, keyword, keywordToFind)
    assert_checktrue(length(content) > 10);
    assert_checktrue(grep(content, keywordToFind) <> []);
    deletefile(filePath);
endfunction

curdir = pwd();
destdir = fullfile(TMPDIR, "getURL");
mkdir(destdir);
cd(destdir);

// Check downloaded file
filePath = getURL("http://www.scilab.org");
checkFile(filePath, fullfile(destdir, "index.html"), 1000);

filePath = getURL("http://www.scilab.org/");
checkFile(filePath, fullfile(destdir, "index.html"), 1000);

filePath = getURL("http://www.scilab.org/product/man/numderivative.html");
checkFile(filePath, fullfile(destdir, "numderivative.html"), 1000);

filePath = getURL("www.scilab.org");
checkFile(filePath, fullfile(destdir, "index.html"), 1000);

filePath = getURL("www.scilab.org/product/man/numderivative.html");
checkFile(filePath, fullfile(destdir, "numderivative.html"), 1000);

filePath = getURL("ftp://ftp.free.fr/pub/Distributions_Linux/debian/README");
checkFile(filePath, fullfile(destdir, "README"), 10);

filePath = getURL("ftp://ftp.free.fr/pub/Distributions_Linux/debian/README", fullfile(destdir, "README_Debian"));
checkFileAndContent(filePath, fullfile(destdir, "README_Debian"), 10, "Linux");

filePath = getURL("ftp://ftp.free.fr/pub/Distributions_Linux/debian/README", destdir);
checkFileAndContent(filePath, fullfile(destdir, "README"), 10, "Linux");

// HTTPS
filePath = getURL("https://encrypted.google.com");
checkFileAndContent(filePath, fullfile(destdir, "index.html"), 100, "html");

filePath = getURL("http://httpbin.org/basic-auth/user/passwd", fullfile(destdir, "testauth"), "user", "passwd");
checkFileAndContent(filePath, fullfile(destdir, "testauth"), 10, "authenticated");

// Check returned content
[filePath, content] = getURL("http://www.scilab.org:80");
checkContent(filePath, content, 1000, "html");

[filePath, content] = getURL("http://plop:ae@www.scilab.org:80");
checkContent(filePath, content, 1000, "html");

[filePath, content] = getURL("http://www.scilab.org/aze");
checkContent(filePath, content, 100, "aze");

[filePath, content] = getURL("http://www.scilab.org");
checkContent(filePath, content, 1000, "html");

[filePath, content] = getURL("http://www.scilab.org/");
checkContent(filePath, content, 1000, "html");

[filePath, content] = getURL("ftp://ftp.free.fr/pub/Distributions_Linux/debian/README");
checkContent(filePath, content, 10, "Linux");

// HTTPS
[filePath, content] = getURL("https://encrypted.google.com");
checkContent(filePath, content, 100, "html");

[filePath, content] = getURL("http://httpbin.org/basic-auth/user/passwd", destdir, "user", "passwd");
checkContent(filePath, content, 10, "authenticated");

// Badly formated URL
assert_checkerror("getURL(''http://plop@ae:www.scilab.org:80'');", [], 999);

cd(curdir);
