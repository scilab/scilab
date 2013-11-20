dnl This code comes from evolution-data-server
dnl License LGPL

AC_DEFUN([AC_CUSTOM_LARGE_FILE], [
dnl *************************************
dnl *** Checks for large file support ***
dnl *************************************
    AC_SYS_LARGEFILE
    AC_CACHE_CHECK([for _LARGEFILE64_SOURCE value needed for large files], largefile64_cv_source,
    [
        AC_TRY_COMPILE([
                #include <sys/types.h>
                #include <sys/stat.h>
                #include <fcntl.h>
        ],[
                int fd = open ("__o_largefile", O_CREAT | O_RDWR | O_LARGEFILE, 0644);
        ],[
                largefile64_source="no"
        ],[
                AC_TRY_COMPILE([
                        #define _LARGEFILE64_SOURCE
                        #include <sys/types.h>
                        #include <sys/stat.h>
                        #include <fcntl.h>
                ],[
                        int fd = open ("__o_largefile", O_CREAT | O_RDWR | O_LARGEFILE, 0644);
                ],[
                        largefile64_source="yes"
                ],[
                        largefile64_source="unknown"
                ])
        ])
    ])

    if test "x$largefile64_source" = "xyes"; then
        LARGEFILE_CFLAGS="-D_LARGEFILE64_SOURCE"
        enable_largefile="yes"
    elif test "x$largefile64_source" = "xunknown"; then
        AC_DEFINE(O_LARGEFILE, 0, [Define to 0 if your system does not have the O_LARGEFILE flag])
    fi

    if test -n "$ac_cv_sys_large_files" -a "x$ac_cv_sys_large_files" != "xno"; then
        LARGEFILE_CFLAGS="$LARGEFILE_CFLAGS -D_LARGE_FILES"
        enable_largefile="yes"
    fi

    if test "x$ac_cv_sys_file_offset_bits" != "xno"; then
        LARGEFILE_CFLAGS="$LARGEFILE_CFLAGS -D_FILE_OFFSET_BITS=$ac_cv_sys_file_offset_bits"
        enable_largefile="yes"
    fi

])
