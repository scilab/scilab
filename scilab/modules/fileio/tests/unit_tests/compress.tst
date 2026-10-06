// =============================================================================
// Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2023 - Dassault Systèmes S.E. - Cédric Delamarre
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->
//
// =============================================================================
// Unitary tests for compression and decompression function
//==============================================================================

work_dir = TMPDIR+"/compression";
mkdir(work_dir);
mkdir(work_dir+"/out");

r100 = rand(100,100);
expected = r100;
save(work_dir+"/rand100.sod", "r100");

/*** tar ***/
// tar.gz, auto detection
compress(work_dir+"/rand100.tar.gz", work_dir+"/rand100.sod");
decompress(work_dir+"/rand100.tar.gz", work_dir+"/out");
clear r100;
load(work_dir+"/out/rand100.sod");
deletefile(work_dir+"/out/rand100.sod");
assert_checkequal(r100, expected);

// tar.gz, auto detection, compression level
compress(work_dir+"/rand100l9.tar.gz", work_dir+"/rand100.sod", level=9);
decompress(work_dir+"/rand100l9.tar.gz", work_dir+"/out");
clear r100;
load(work_dir+"/out/rand100.sod");
deletefile(work_dir+"/out/rand100.sod");
assert_checkequal(r100, expected);

finfo1 = fileinfo(work_dir+"/rand100.tar.gz");
finfo2 = fileinfo(work_dir+"/rand100l9.tar.gz");
assert_checktrue(finfo1(1) > finfo2(1)); // check compression level well reduce the file size

// tar xz, set format + compression level
compress(work_dir+"/rand100l9.tar.xz", work_dir+"/rand100.sod", format="tar", compression="xz");
decompress(work_dir+"/rand100l9.tar.xz", work_dir+"/out");
clear r100;
load(work_dir+"/out/rand100.sod");
deletefile(work_dir+"/out/rand100.sod");
assert_checkequal(r100, expected);

// tar.zst, set format + compression + level
// note: the .zst extension is not managed by libarchive, so format and
// compression options are mandatory to create a zstd archive
compress(work_dir+"/rand100l22.tar.zst", work_dir+"/rand100.sod", format="tar", compression="zstd", level=22);
decompress(work_dir+"/rand100l22.tar.zst", work_dir+"/out");
clear r100;
load(work_dir+"/out/rand100.sod");
deletefile(work_dir+"/out/rand100.sod");
assert_checkequal(r100, expected);

/*** zip ***/
// auto detect, wildcard
filesin  = compress("TMPDIR/fileio_tests.zip", "SCI/modules/fileio/tests/unit_tests/*.tst");
filesout = decompress("TMPDIR/fileio_tests.zip", "TMPDIR/fileio_tests");
assert_checkequal(size(filesin), size(filesout));
assert_checkequal(gsort(filesin), gsort(ls("TMPDIR/fileio_tests/")));
[p, fname, ext]=fileparts(filesout);
assert_checkequal(filesin, fname+ext);

// auto detect, password
compress(work_dir+"/rand100pwd.zip", work_dir+"/rand100.sod", password="unit_test");
decompress(work_dir+"/rand100pwd.zip", work_dir+"/out", password="unit_test");
clear r100;
load(work_dir+"/out/rand100.sod");
deletefile(work_dir+"/out/rand100.sod");
assert_checkequal(r100, expected);

// set format, compression | decompress auto detect
compress(work_dir+"/rand100.zip", work_dir+"/rand100.sod", format="zip", compression="lzma");
decompress(work_dir+"/rand100.zip", work_dir+"/out");
clear r100;
load(work_dir+"/out/rand100.sod");
deletefile(work_dir+"/out/rand100.sod");
assert_checkequal(r100, expected);

/*** ATOMS ***/
files = decompress("SCI/modules/atoms/tests/unit_tests/toolbox_7V6_1.0-1.bin.zip", TMPDIR);
assert_checktrue(isfile("TMPDIR/toolbox_7V6_1.0/loader.sce"));

/*** JAR ***/
files = decompress("SCI/modules/scirenderer/jar/scirenderer.jar", TMPDIR)
assert_checktrue(isfile("TMPDIR/org/scilab/forge/scirenderer/Drawer.class"));
