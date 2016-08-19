#
# Return MacOSX version using system_profile tool.
#
AC_DEFUN([AC_GET_MACOSX_VERSION],[
    AC_MSG_CHECKING([Mac OS X Version])
    [macosx_version=`sw_vers -productVersion`]
    case $macosx_version in
         10.11*)
              AC_MSG_RESULT([Mac OS X 10.11 - El Capitan.])
         ;;
         10.10*)
              AC_MSG_RESULT([Mac OS X 10.10 - Yosemite.])
         ;;
         10.9*)
              AC_MSG_RESULT([Mac OS X 10.9 - Mavericks.])
         ;;
         10.8.3|10.8.4|10.8.5)
              AC_MSG_RESULT([Mac OS X $macosx_version - Mountain Lion.])
         ;;
         *)
              AC_MSG_ERROR([MacOSX 10.8.3 to 10.10 are needed. Found $macosx_version])
         ;;
    esac
])
