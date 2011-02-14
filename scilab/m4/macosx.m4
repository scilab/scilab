#
# Return MacOSX version using system_profile tool.
#
AC_DEFUN([AC_GET_MACOSX_VERSION],[
        AC_MSG_CHECKING([MacOSX Version])
        if eval system_profiler SPSoftwareDataType | grep "System Version" | grep "10\.6" > /dev/null; then
        AC_MSG_RESULT([MacOSX 10.6 - Snow Leopard.])
        macosx_version="10.6"
        else
        if eval system_profiler SPSoftwareDataType | grep "System Version" | grep "10\.5" > /dev/null; then
        AC_MSG_RESULT([MacOSX 10.5 - Leopard.])
        macosx_version="10.5"
        else
        AC_MSG_ERROR([MacOSX 10.5 or 10.6 is needed.])
        fi
        fi
])