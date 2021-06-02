#
# Return MacOSX version using system_profile tool.
#
AC_DEFUN([AC_GET_MACOSX_VERSION],[
    AC_MSG_CHECKING([Mac OS X Version])
    [macosx_version=`sw_vers -productVersion`]
    case $macosx_version in
         11.*)
              AC_MSG_RESULT([macOS $macosx_version - Big Sur.])
         ;;
         10.15*)
              AC_MSG_RESULT([macOS 10.15 - Catalina.])
         ;;
         10.14*)
              AC_MSG_RESULT([macOS 10.14 - Mojave.])
         ;;
         10.13*)
              AC_MSG_RESULT([macOS 10.13 - High Sierra.])
         ;;
         *)
              AC_MSG_ERROR([macOS 10.13 to 11 are needed. Found $macosx_version])
         ;;
    esac
])

