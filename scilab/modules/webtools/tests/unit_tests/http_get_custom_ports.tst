// =============================================================================
// Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2023 - Dassault Systèmes S.E. - Cédric DELAMARRE
// Copyright (C) 2026 - Dassault Systèmes S.E. - Clément DAVID
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//<-- CLI SHELL MODE -->
//<-- NO CHECK REF -->
//<-- NOT FIXED --> CI failed to connect to custom port
//
// http_get testing with custom protocol, fail on CI due to WAF

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

function checkContent(content, keywordToFind)
    assert_checktrue(length(content) > 10);
    assert_checktrue(grep(content, keywordToFind) <> []);
endfunction

curdir = pwd();
destdir = fullfile(TMPDIR, "http_get_custom_ports");
mkdir(destdir);
cd(destdir);

filePath = http_get("http://www.scilab.org:80", fullfile(destdir, "index.html"), follow=%t);
checkFile(filePath, fullfile(destdir, "index.html"), 1000);

content  = http_get("http://www.scilab.org:80", follow=%t);
checkContent(content, "html");

filePath = http_get("ftp://ftp.free.fr/pub/Distributions_Linux/debian/README", fullfile(destdir, "README"));
checkFile(filePath, fullfile(destdir, "README"), 10);

filePath = http_get("ftp://ftp.free.fr/pub/Distributions_Linux/debian/README", fullfile(destdir, "README_Debian"));
checkFileAndContent(filePath, fullfile(destdir, "README_Debian"), 10, "Linux");

filePath = http_get("ftp://ftp.free.fr/pub/Distributions_Linux/debian/README", fullfile(destdir, "README"));
checkFileAndContent(filePath, fullfile(destdir, "README"), 10, "Linux");

content  = http_get("ftp://ftp.free.fr/pub/Distributions_Linux/debian/README");
checkContent(content, "Linux");

content = http_get("https://httpbin.org/basic-auth/user/passwd", auth="user:passwd");
assert_checkequal(content.authenticated, %t);
assert_checkequal(content.user, "user");

filePath = http_get("https://httpbin.org/basic-auth/user/passwd", fullfile(destdir, "testauth"), auth="user:passwd");
checkFileAndContent(filePath, fullfile(destdir, "testauth"), 10, "authenticated");
