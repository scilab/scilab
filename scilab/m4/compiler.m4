
dnl CHECK_COMPILER_ARG(LANG, ARG, ACTION-IF-NOT_FOUND)
AC_DEFUN([CHECK_COMPILER_ARG],[
    AC_LANG_PUSH([$1])
    AC_MSG_CHECKING([if the $1 compiler accepts $2])
    ac_save_$3="$$3"
    $3="$$3 -rdynamic"

    AC_COMPILE_IFELSE([AC_LANG_PROGRAM()],
       [AC_MSG_RESULT([yes])],
       [AC_MSG_RESULT([no])
       $3="$ac_save_$3"
       ]
    )
    AC_LANG_POP([$1])

])
