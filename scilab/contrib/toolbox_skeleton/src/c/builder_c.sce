// This file is released under the 3-clause BSD license. See COPYING-BSD.

src_c_path = get_absolute_file_path("builder_c.sce");

CFLAGS = "-I" + src_c_path;

tbx_build_src(["csum","csub"],    ..
              ["csum.c","csub.c"],..
              "c", ..             ..
              src_c_path,         ..
              "",                 ..
              "",                 ..
              CFLAGS);

clear tbx_build_src;
clear src_c_path;
clear CFLAGS;
