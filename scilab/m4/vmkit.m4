AC_DEFUN([AC_VMKIT], [

######
#### VMKIT disabled by default
######

AC_ARG_WITH(vmkit,
    AC_HELP_STRING([--with-vmkit],[Uses VMKIT infrastructure]))

AC_ARG_WITH(vmkit-package,
    AC_HELP_STRING([--with-vmkit-package],[Uses VMKIT infrastructure with the packages]))

AC_ARG_WITH(llvm-src,
    AC_HELP_STRING([--with-llvm-src],[Directory containing the source code of LLVM. Needed with --with-vmkit]))

AC_ARG_WITH(llvm-bin,
    AC_HELP_STRING([--with-llvm-bin],[Directory containing the binaries of LLVM. Default is llvm-src/Release+Asserts]))

AC_ARG_WITH(vmkit-src,
    AC_HELP_STRING([--with-vmkit-src],[Directory containing the source code of VMKIT. Needed with --with-vmkit]))

AC_ARG_WITH(vmkit-bin,
    AC_HELP_STRING([--with-vmkit-bin],[Directory containing the binaries of VMKIT. Default is vmkit-src/Release+Asserts]))

AM_CONDITIONAL(WITH_VMKIT, test -n "$with_vmkit" -o -n "$with_vmkit_package")

if test -n "$with_vmkit_package"; then
   # Hardcode path for Debian and Ubuntu
   # This will be improved by an automatic detection
   LLVM_SRC_DIR="/usr/include/llvm-3.3/"
   LLVM_BIN_DIR="/usr/lib/llvm-3.3/lib/"
   VMKIT_SRC_DIR="/usr/include/vmkit/"
   VMKIT_BIN_DIR="/usr/bin/"
   VMKIT_ACPPFLAGS="-I$LLVM_SRC_DIR/ -I$VMKIT_SRC_DIR/ -I$VMKIT_SRC_DIR/MMTk/ -D_DEBUG -D_GNU_SOURCE -D__STDC_CONSTANT_MACROS -D__STDC_FORMAT_MACROS -D__STDC_LIMIT_MACROS -O3 -fomit-frame-pointer -fvisibility-inlines-hidden -fPIC -Woverloaded-virtual -Wcast-qual"
   VMKIT_ALDFLAGS="-L$LLVM_BIN_DIR/ -lLLVMBitReader -lLLVMipo -lLLVMVectorize -lLLVMInstrumentation -lLLVMX86CodeGen -lLLVMX86Desc -lLLVMX86Info -lLLVMX86AsmPrinter -lLLVMX86Utils -lLLVMSelectionDAG -lLLVMAsmPrinter -lLLVMMCParser -lLLVMJIT -lLLVMRuntimeDyld -lLLVMExecutionEngine -lLLVMCodeGen -lLLVMScalarOpts -lLLVMInstCombine -lLLVMTransformUtils -lLLVMipa -lLLVMAnalysis -lLLVMTarget -lLLVMMC -lLLVMObject -lLLVMCore -lLLVMSupport"
   VMKIT_ALDADD="$VMKIT_BIN_DIR/../lib/libFinalMMTk.a $VMKIT_BIN_DIR/../lib/libJ3.a $VMKIT_BIN_DIR/../lib/libClasspath.a $VMKIT_BIN_DIR/../lib/libVmkit.a $VMKIT_BIN_DIR/../lib/libVmkitCompiler.a $VMKIT_BIN_DIR/../lib/libCommonThread.a"
   AC_DEFINE([VMKIT_ENABLED],[],[VMKIT Enabled])

else

if test -n "$with_vmkit";then
        if (test "$with_llvm_src" == "" || test "x$with_llvm_src" == "xyes");then
                AC_MSG_ERROR([Due the use of --with-vmkit, the use of --with-llvm-src=<dir> is needed])
        fi

        if (test "$with_vmkit_src" == ""|| test "x$with_vmkit_src" == "xyes");then
                AC_MSG_ERROR([Due the use of --with-vmkit, the use of --with-vmkit-src=<dir> is needed])a
        fi

        LLVM_SRC_DIR="$with_llvm_src"
        VMKIT_SRC_DIR="$with_vmkit_src"
        AC_SUBST([LLVM_SRC_DIR])
        AC_SUBST([VMKIT_SRC_DIR])
	AC_DEFINE([VMKIT_ENABLED],[],[VMKIT Enabled])
fi

if test -n "$with_llvm_bin";then
        if (test "$with_llvm_bin" == "" || test "x$with_llvm_bin" == "xyes");then
                AC_MSG_ERROR([Use --with-llvm-bin=<dir>])
        else
                LLVM_BIN_DIR="$with_llvm_bin"
        fi
else
        LLVM_BIN_DIR="$with_llvm_src/Release+Asserts"
fi
AC_SUBST([LLVM_BIN_DIR])

if test -n "$with_vmkit_bin";then
        if (test "$with_vmkit_bin" == "" || test "x$with_vmkit_bin" == "xyes");then
                AC_MSG_ERROR([Use --with-vmkit-bin=<dir>])
        else
                VMKIT_BIN_DIR="$with_vmkit_bin"
        fi
else
        VMKIT_BIN_DIR="$with_vmkit_src/Release+Asserts"
fi
AC_SUBST([VMKIT_BIN_DIR])


VMKIT_ACPPFLAGS="-I$LLVM_SRC_DIR/include -I$LLVM_SRC_DIR/ -I$VMKIT_SRC_DIR/ -I$VMKIT_SRC_DIR/MMTk/ -D_DEBUG -D_GNU_SOURCE -D__STDC_CONSTANT_MACROS -D__STDC_FORMAT_MACROS -D__STDC_LIMIT_MACROS -O3 -fomit-frame-pointer -fvisibility-inlines-hidden -fPIC -Woverloaded-virtual -Wcast-qual"
AC_SUBST([VMKIT_ACPPFLAGS])

VMKIT_ALDFLAGS="-L$LLVM_BIN_DIR/lib -lLLVMBitReader -lLLVMipo -lLLVMVectorize -lLLVMInstrumentation -lLLVMX86CodeGen -lLLVMX86Desc -lLLVMX86Info -lLLVMX86AsmPrinter -lLLVMX86Utils -lLLVMSelectionDAG -lLLVMAsmPrinter -lLLVMMCParser -lLLVMJIT -lLLVMRuntimeDyld -lLLVMExecutionEngine -lLLVMCodeGen -lLLVMScalarOpts -lLLVMInstCombine -lLLVMTransformUtils -lLLVMipa -lLLVMAnalysis -lLLVMTarget -lLLVMMC -lLLVMObject -lLLVMCore -lLLVMSupport"
AC_SUBST([VMKIT_ALDFLAGS])


VMKIT_ALDADD="$VMKIT_BIN_DIR/../lib/libFinalMMTk.a $VMKIT_BIN_DIR/../lib/libJ3.a $VMKIT_BIN_DIR/../lib/libClasspath.a $VMKIT_BIN_DIR/../lib/libVmkit.a $VMKIT_BIN_DIR/../lib/libVmkitCompiler.a $VMKIT_BIN_DIR/../lib/libCommonThread.a"
AC_SUBST([VMKIT_ALDADD])

fi

])
