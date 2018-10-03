// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2018 - ESI Group - Antoine ELIAS
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->
// <-- LONG TIME EXECUTION -->
//
// <-- Non-regression test for bug 15033 -->
//
// <-- Bugzilla URL -->
// https://bugzilla.scilab.org/show_bug.cgi?id=15033
//
// <-- Short Description -->
// fileinfo returns wrong size on Windows ( size > 2^31 )

//generate a big file

x = 2**32 / (8 * 1024 * 1024);

data = rand(1, 1024 * 1024);

path = "TMPDIR/big_file_15033.dat";
f = mopen(path, "wb");
for i = 1:x
    mput(data, "l", f);
end

mclose(f);

info = fileinfo(path);
assert_checkequal(info(1), 8 * 1024 * 1024 * 512); //4Go
mdelete(path);