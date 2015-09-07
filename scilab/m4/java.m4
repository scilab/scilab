## Macros "stolen" from jacl (http://tcljava.sourceforge.net/)
## They made a great job on this part !

#------------------------------------------------------------------------
# AC_MSG_LOG( MSG, ?LOGONLY? )
#
#    Write the message out to the config.log file and the console.
#    If 1 is passed as the second argument, then write to the
#    config.log file only.
#
# Arguments:
#    1. The message to log
#    2. Optional boolean, if true then write to config.log only
#------------------------------------------------------------------------

AC_DEFUN([AC_MSG_LOG], [
    echo $1 >&AS_MESSAGE_LOG_FD
    m4_ifval([$2],,[echo $1])
])

#------------------------------------------------------------------------
# AC_GREP_FILE( PATTERN, FILE, ACTION-IF-FOUND, [ACTION-IF-NOT-FOUND])
#
#    Use grep to search for a pattern in a file. If the pattern
#    is not found then return a non zero exit status. No information
#    will be echoed to the screen by this macro.
#
# Arguments:
#    1. The pattern to search for
#    2. The name of the file to be grep'ed
#    3. The script to execute if PATTERN is found in FILE
#    4. The script to execute if PATTERN is not found in FILE (optional)
#------------------------------------------------------------------------

AC_DEFUN([AC_GREP_FILE], [
    AC_MSG_LOG([grep in $2 for pattern '"$1"'], 1)
    if (grep "$1" $2 > /dev/null 2>&1) ; then
        AC_MSG_LOG([grep result : yes], 1)
        $3
    else
        AC_MSG_LOG([grep result : no], 1)
        m4_ifval([$4], [
            $4
        ])dnl
    fi
])


#------------------------------------------------------------------------
# AC_PROG_JAVAC
#
#    If JAVAC is not already defined, then search for "javac" on
#    the path. If a java compiler is found, then test it to make
#    sure it actually works.
#
# Arguments:
#    NONE
#
# VARIABLES SET:
#    JAVAC can be set to the path name of the java compiler
#    JAVAC_FLAGS can be set to compiler specific flags
#    ac_java_jvm_dir can be set to the jvm's root directory
#------------------------------------------------------------------------

AC_DEFUN([AC_PROG_JAVAC], [
# Mac OS X
    if test "x$JAVAC" = "x" ; then
    case "$host_os" in
         *darwin* )
         # Don't follow the symlink since Java under MacOS is messy
         # Uses the wrapper providing by Apple to retrieve the path
         # See: http://developer.apple.com/mac/library/qa/qa2001/qa1170.html
           JAVAC=$(/usr/libexec/java_home --arch x86_64 --failfast --version 1.8+)/bin/javac
               DONT_FOLLOW_SYMLINK=yes
         ;;
    esac
    fi
    if test "x$JAVAC" = "x" ; then
        AC_PATH_PROG(JAVAC, javac)
        if test "x$JAVAC" = "x" ; then
            AC_MSG_ERROR([javac not found on PATH ... did you try with --with-jdk=DIR])
        fi
    fi
    if test ! -f "$JAVAC" ; then
        AC_MSG_ERROR([javac '$JAVAC' does not exist.
        Perhaps Java is not installed or you passed a bad dir to a --with option.])
    fi

    # Check for installs which uses a symlink. If it is the case, try to resolve JAVA_HOME from it
    if test -h "$JAVAC" -a "x$DONT_FOLLOW_SYMLINK" != "xyes"; then
        FOLLOW_SYMLINKS($JAVAC,"javac")
        JAVAC=$SYMLINK_FOLLOWED_TO
        TMP=`dirname $SYMLINK_FOLLOWED_TO`
        TMP=`dirname $TMP`
        ac_java_jvm_dir=$TMP
        echo "Java base directory (probably) available here : $ac_java_jvm_dir"
    fi



    # If we were searching for javac, then set ac_java_jvm_dir
    if test "x$ac_java_jvm_dir" = "x"; then
        TMP=`dirname $JAVAC`
        TMP=`dirname $TMP`
        ac_java_jvm_dir=$TMP
    fi

    # Look for a setting for the CLASSPATH, we might need one to run JAVAC
    AC_JAVA_CLASSPATH

    # FIXME : add detection of command line arguments for JAVAC

    JAVAC_FLAGS=-g
    JAVAC_D_FLAG=-d

    dnl Test out the Java compiler with an empty class
    AC_MSG_CHECKING([to see if the java compiler works])
    AC_JAVA_TRY_COMPILE(,,"no",works=yes)
    if test "$works" = "yes" ; then
        AC_MSG_RESULT($works)
    else
        AC_MSG_ERROR([Could not compile simple Java program with '$JAVAC'. Try with the Sun JDK (1.5 or 6).])
    fi

    AC_MSG_LOG([Using JAVAC=$JAVAC])
])


#------------------------------------------------------------------------
# AC_JAVA_TRY_COMPILE(imports, main-body, try-to-run, action-if-worked, [action-if-failed])
#
#    Try to compile a Java program. This works a lot like AC_TRY_COMPILE
#    except is supports Java instead of C or C++. This macro will create
#    a file named Test.java and try to compile it.
#
# Arguments:
#    imports should contain Java import statements like [import java.util.*;]
#       main-body should contain the code to appear in the main() method
#    action-if-worked should contain the code to run if the compile worked
#    action-if-failed should contain the code to run if the compile failed (optional)
#------------------------------------------------------------------------

AC_DEFUN([AC_JAVA_TRY_COMPILE], [
    cat << \EOF > conftest.java
// [#]line __oline__ "configure"
import java.util.regex.Pattern;

[$1]

public class conftest {
    public static void main(String[[]] argv) {
        [$2]
    }

    private static int compare(String v1, String v2) {
        String s1 = normalisedVersion(v1);
        String s2 = normalisedVersion(v2);
        return s1.compareTo(s2);
    }

    private static String normalisedVersion(String version) {
        return normalisedVersion(version, ".", 4);
    }

    private static String normalisedVersion(String version, String sep, int maxWidth) {
        String[[]] split = Pattern.compile(sep, Pattern.LITERAL).split(version);
        StringBuilder sb = new StringBuilder();
        for (String s : split) {
            sb.append(String.format("%" + maxWidth + 's', s));
        }
        return sb.toString();
    }
}
EOF

    CLASSPATH=$ac_java_classpath
    export CLASSPATH
    cmd="$JAVAC ${JAVAC_FLAGS} conftest.java"
    if (echo $cmd >&AS_MESSAGE_LOG_FD ; eval $cmd >conftest.java.output 2>&AS_MESSAGE_LOG_FD) ; then
       if test "$3" = "no"; then
           echo "yes" >&AS_MESSAGE_LOG_FD
              $4
       else
              cmd="$JAVA conftest"
              if (echo $cmd >&AS_MESSAGE_LOG_FD ; eval $cmd >conftest.java.output 2>&AS_MESSAGE_LOG_FD); then
               echo "yes" >&AS_MESSAGE_LOG_FD
                  $4
            else
                echo "configure: failed program was:" >&AS_MESSAGE_LOG_FD
                cat conftest.java >&AS_MESSAGE_LOG_FD
                if test -s conftest.java.output; then
                   STDOUT=`cat conftest.java.output`
                fi
                echo "configure: CLASSPATH was $CLASSPATH" >&AS_MESSAGE_LOG_FD
                m4_ifval([$5],
                [  $5
                ])dnl
            fi
        fi
        if test -f conftest.java.output; then
           rm conftest.java.output
        fi
    else
        echo "configure: failed program was:" >&AS_MESSAGE_LOG_FD
        cat conftest.java >&AS_MESSAGE_LOG_FD
        echo "configure: CLASSPATH was $CLASSPATH" >&AS_MESSAGE_LOG_FD
        m4_ifval([$5],
        [  $5
        ])dnl
    fi
])


#------------------------------------------------------------------------
# AC_JAVA_DETECT_JVM
#
#    Figure out what JVM to build with. If no JVM was already defined
#    using a --with command line option then we search for one
#    by looking for the javac executable.
#
# Arguments:
#    NONE
#
# VARIABLES SET:
#    JAVAC
#    ac_java_jvm_version can be set to 1.4, 1.5, 1.6, 1.7, 1.8
#    ac_java_jvm_dir can be set to the jvm's root directory
#
# DEPENDS ON:
#    This macro can depend on the values set by the following macros:
#    AC_JAVA_WITH_JDK
#    AC_PROG_JAVAC
#------------------------------------------------------------------------

AC_DEFUN([AC_JAVA_DETECT_JVM], [
    AC_MSG_CHECKING([JAVA_HOME variable])
    # check if JAVA_HOME is set. If it is the case, try to use if first
    if test ! -z "$JAVA_HOME" && test "x$ac_java_jvm_dir" = "x"; then
        if test -x $JAVA_HOME/bin/javac${EXEEXT}; then
            AC_MSG_RESULT([JAVA_HOME variable found, use it as JVM root directory])
                ac_java_jvm_dir=`cd $JAVA_HOME ; pwd`
            JAVAC=$ac_java_jvm_dir/bin/javac${EXEEXT}
        else
            AC_MSG_RESULT([JAVA_HOME variable found, but unable to find javac
Maybe JAVA_HOME is pointing to a JRE (Java Runtime Environment) instead of a JDK (Java Developement Kit) ? )])
        fi
    else
        AC_MSG_RESULT([not defined])
    fi

# Mac OS default path
    if test "x$JAVAC" = "x" && test "x$ac_java_jvm_dir" != "x"; then
        case "$host_os" in
             *darwin* )
            AC_MSG_RESULT([Darwin (Mac OS X) found. Use the standard paths.])
            # See: http://developer.apple.com/mac/library/qa/qa2001/qa1170.html
            ac_java_jvm_dir=$(/usr/libexec/java_home --arch x86_64 --failfast --version 1.8+)
            JAVAC=$ac_java_jvm_dir/bin/javac
            ;;
        esac
    fi

    # if we do not know the jvm dir, javac will be found on the PATH
    if test "x$JAVAC" = "x" && test "x$ac_java_jvm_dir" != "x"; then
        ac_java_jvm_dir=`cd $ac_java_jvm_dir ; pwd`
        JAVAC=$ac_java_jvm_dir/bin/javac${EXEEXT}
    fi

    # Search for and test the javac compiler
    AC_PROG_JAVAC

    AC_MSG_LOG([Java found in $ac_java_jvm_dir])

    # Try to detect non JDK JVMs. If we can't, then just assume a jdk

    AC_MSG_CHECKING([type of jvm])

    if test "x$ac_java_jvm_name" = "x" ; then
        AC_JAVA_TRY_COMPILE([import gnu.java.io.EncodingManager;],,"no",ac_java_jvm_name=gcj)
    fi

    if test "x$ac_java_jvm_name" = "x" ; then
       ac_java_jvm_name=jdk
    fi

    AC_MSG_RESULT([$ac_java_jvm_name])

    case "$ac_java_jvm_name" in
        gcj) DO=nothing ;;
        jdk) DO=nothing ;;
        *) AC_MSG_ERROR(['$ac_java_jvm_name' is not a supported JVM]) ;;
    esac

    # Try to detect the version of java that is installed

    AC_MSG_CHECKING([java API version])

    # The class java.nio.charset.Charset is new to 1.4
    AC_JAVA_TRY_COMPILE([import java.nio.charset.Charset;], , "no", ac_java_jvm_version=1.4)

    # The class java.lang.StringBuilder is new to 1.5
    AC_JAVA_TRY_COMPILE([import java.lang.StringBuilder;], , "no", ac_java_jvm_version=1.5)

    # The class java.util.ArrayDeque is new to 1.6
    AC_JAVA_TRY_COMPILE([import java.util.ArrayDeque;], , "no", ac_java_jvm_version=1.6)

    # The class java.nio.file.Path is new to 1.7
    AC_JAVA_TRY_COMPILE([import java.nio.file.Path;], , "no", ac_java_jvm_version=1.7)

    # The class java.util.stream.DoubleStream is new to 1.8
    AC_JAVA_TRY_COMPILE([import java.util.stream.DoubleStream;], , "no", ac_java_jvm_version=1.8)

    if test "x$ac_java_jvm_version" = "x" ; then
        AC_MSG_ERROR([Could not detect Java version, 1.4 or newer is required])
    fi

    AC_MSG_RESULT([$ac_java_jvm_version])

])


#------------------------------------------------------------------------
# AC_JAVA_CLASSPATH
#
#    Find out which .zip or .jar files need to be included on
#    the CLASSPATH if we are setting it via an env variable.
#
# Arguments:
#    NONE
#
# VARIABLES SET:
#    ac_java_classpath
#
# DEPENDS ON:
#    This macro is used by the AC_JAVA_DETECT_JVM macro.
#    It depends on the ac_java_jvm_dir variable.
#------------------------------------------------------------------------

AC_DEFUN([AC_JAVA_CLASSPATH], [
    AC_MSG_CHECKING([for zip or jar files to include on CLASSPATH])

    if test "x$ac_java_jvm_dir" = "x" ; then
        AC_MSG_ERROR([jvm directory not set])
    fi

    # GNU gcj does not need to set the CLASSPATH.

    # Assume that JDK 1.4 and newer systems will
    # not need to explicitly set the CLASSPATH.

    # Append CLASSPATH if env var is set. Avoid append
    # under msys because CLASSPATH is in Win32 format
    # and we can't combine it with a msys path.
    if test "x$CLASSPATH" != "x" && test "$ac_cv_tcl_win32" != "yes" ; then
        AC_MSG_LOG([Adding user supplied CLASSPATH env var])
        ac_java_classpath="${ac_java_classpath}:${CLASSPATH}"
    fi

    AC_MSG_LOG([Using CLASSPATH=$ac_java_classpath],1)
    AC_MSG_RESULT($ac_java_classpath)
])


#------------------------------------------------------------------------
# AC_JAVA_JNI_INCLUDE
#
#    Figure out where jni.h and jni_md.h include files are installed.
#
# Arguments:
#    NONE
#
# VARIABLES SET:
#    ac_java_jvm_jni_include_flags : Flags that we pass to the compiler
#           so that it can locate JNI headers. (for example: -I/usr/jdk/include)
#
# DEPENDS ON:
#    This macro must be run after the AC_JAVA_DETECT_JVM macro as
#    it depends on the ac_java_jvm_dir variable.
#------------------------------------------------------------------------

AC_DEFUN([AC_JAVA_JNI_INCLUDE], [

    # JAVA_HOME specificed, check if we can find jni.h in this path
    if test !  -z "$JAVA_HOME"; then
        ac_java_jvm_dir=$JAVA_HOME
    fi
    # Look for jni.h in the subdirectory $ac_java_jvm_dir/include

    F=$ac_java_jvm_dir/include/jni.h
    if test -f "$F" ; then
         ac_java_jvm_jni_include_flags="-I`dirname $F`"
    else
         F=`ls $ac_java_jvm_dir/include/*/jni.h 2>/dev/null`
         if test -f "$F" ; then
             ac_java_jvm_jni_include_flags="-I`dirname $F`"
         else
        case "$host_os" in
             *darwin* )
                       ac_java_jvm_jni_include_flags="-I/Developer/SDKs/MacOSX${macosx_version}.sdk/System/Library/Frameworks/JavaVM.framework/Headers -I$(/usr/libexec/java_home --arch x86_64 --failfast --version 1.8+)/include/ -I/System/Library/Frameworks/JavaVM.framework/Versions/A/Headers/"
                  ;;
              *)
                       AC_MSG_ERROR([Could not locate Java's jni.h include file])
               ;;
               esac
         fi
    fi

    # Look for jni_md.h in an arch specific subdirectory
    # we assume that there is only one arch subdirectory,
    # if that is not the case we would need to use $host

    F=`ls $ac_java_jvm_dir/include/*/jni_md.h 2>/dev/null`
    if test -f "$F" ; then
        ac_java_jvm_jni_include_flags="$ac_java_jvm_jni_include_flags -I`dirname $F`"
    fi

    AC_MSG_LOG([Using the following JNI include flags $ac_java_jvm_jni_include_flags])

    # Make sure a simple #include <jni.h> will compile.

    AC_REQUIRE([AC_PROG_CC])

    AC_CACHE_CHECK(if jni.h can be included,
        ac_cv_java_jvm_jni_working,[
        AC_LANG_PUSH(C)
        ac_saved_cflags=$CFLAGS
        CFLAGS="$CFLAGS $ac_java_jvm_jni_include_flags"
        AC_TRY_COMPILE([
            #include <jni.h>
        ],[return 0;],
        ac_cv_java_jvm_jni_working=yes,
        AC_MSG_ERROR([could not compile file that includes jni.h. If you run Mac OS X please make sure you have 'Java developer package'. This is available on http://connect.apple.com/ ]))
        AC_LANG_POP()
        CFLAGS=$ac_saved_cflags
    ])

    # FIXME: should we look for or require a include/native_threads dir?
])



#------------------------------------------------------------------------
# AC_JAVA_JNI_LIBS
#
#    Figure out where the native threads libraries for JNI live.
#
# Arguments:
#    NONE
#
# VARIABLES SET:
#    ac_java_jvm_ld_preload : list of libraries to include in LD_PRELOAD
#    ac_java_jvm_ld_bind_now : if set to 1, then use LD_BIND_NOW=1
#    ac_java_jvm_jni_lib_flags : library flags that we will pass to the compiler.
#        For instance, we might pass -L/usr/jdk/lib -ljava
#    ac_java_jvm_jni_lib_runtime_path : colon separated path of directories
#        that is typically passed to rld.
#
# DEPENDS ON:
#    This macro must be run after the AC_JAVA_DETECT_JVM macro as
#    it depends on the ac_java_jvm_dir variable.
#------------------------------------------------------------------------

AC_DEFUN([AC_JAVA_JNI_LIBS], [
    machine=`uname -m`
    case "$machine" in
        i?86)
          machine=i386
          ;;
        i86pc)
        # Solaris 10 x86
          machine=i386
          ;;
        sun*|sparc64)
       # Sun
          machine=sparc
          ;;
        powerpc)
          machine=ppc
          ;;
        ppc64|ppc64le)
          machine=ppc64
          ;;
        armv*)
          machine=arm
          ;;
        sh*)
            machine=sh
          ;;
        mips64)
            machine=mips
          ;;
    esac

    AC_MSG_LOG([Looking for JNI libs with $machine as machine hardware name])

    # Check for known JDK installation layouts

    # Gives the name of the symbol we want to look for.
    # Mac OS X add a trailing _Impl
    libSymbolToTest="JNI_GetCreatedJavaVMs"

    if test "$ac_java_jvm_name" = "jdk"; then
        # Sun/Blackdown 1.4 for Linux (client JVM)

        F=jre/lib/$machine/libjava.so
        if test "x$ac_java_jvm_jni_lib_flags" = "x" ; then
            AC_MSG_LOG([Looking for $ac_java_jvm_dir/$F])
            if test -f $ac_java_jvm_dir/$F ; then
                AC_MSG_LOG([Found $ac_java_jvm_dir/$F])
                D=`dirname $ac_java_jvm_dir/$F`
                ac_java_jvm_jni_lib_runtime_path=$D
                ac_java_jvm_jni_lib_flags="-L$D -ljava -lverify"
                D=$ac_java_jvm_dir/jre/lib/$machine/client
        if test ! -f $D/libjvm.so; then # Check if it is in the client or server directory
            # Try the server directory
            D=$ac_java_jvm_dir/jre/lib/$machine/server
            if test ! -f $D/libjvm.so; then
                AC_MSG_ERROR([Could not find libjvm.so in
                jre/lib/$machine/client/ or in jre/lib/$machine/server/.
                Please report to http://bugzilla.scilab.org/])
            fi
        fi
                ac_java_jvm_jni_lib_runtime_path="${ac_java_jvm_jni_lib_runtime_path}:$D"
                ac_java_jvm_jni_lib_flags="$ac_java_jvm_jni_lib_flags -L$D -ljvm"
                D=$ac_java_jvm_dir/jre/lib/$machine/native_threads
                if test -d $D; then
                  ac_java_jvm_jni_lib_runtime_path="${ac_java_jvm_jni_lib_runtime_path}:$D"
                  ac_java_jvm_jni_lib_flags="$ac_java_jvm_jni_lib_flags -L$D"
                fi
            fi
        fi

        # Sun JDK 1.5 for AMD64 Linux (server JVM)

        F=jre/lib/amd64/libjava.so
        if test "x$ac_java_jvm_jni_lib_flags" = "x" ; then
            AC_MSG_LOG([Looking for $ac_java_jvm_dir/$F])
            if test -f $ac_java_jvm_dir/$F ; then
                AC_MSG_LOG([Found $ac_java_jvm_dir/$F])

                D=`dirname $ac_java_jvm_dir/$F`
                ac_java_jvm_jni_lib_runtime_path=$D
                ac_java_jvm_jni_lib_flags="-L$D -ljava -lverify"

                D=$ac_java_jvm_dir/jre/lib/amd64/server
                ac_java_jvm_jni_lib_runtime_path="${ac_java_jvm_jni_lib_runtime_path}:$D"
                ac_java_jvm_jni_lib_flags="$ac_java_jvm_jni_lib_flags -L$D -ljvm"
            fi
        fi

        # Eclipse/IBM Java Compiler

        F=jre/lib/i386/client/libjvm.so
        if test "x$ac_java_jvm_jni_lib_flags" = "x" ; then
            AC_MSG_LOG([Looking for $ac_java_jvm_dir/$F])
            if test -f $ac_java_jvm_dir/$F ; then
                AC_MSG_LOG([Found $ac_java_jvm_dir/$F])

                D=`dirname $ac_java_jvm_dir/$F`
                ac_java_jvm_jni_lib_runtime_path=$D
                ac_java_jvm_jni_lib_flags="-L$D -ljvm"

                D=$ac_java_jvm_dir/jre/lib/i386/server
                ac_java_jvm_jni_lib_runtime_path="${ac_java_jvm_jni_lib_runtime_path}:$D"
                ac_java_jvm_jni_lib_flags="$ac_java_jvm_jni_lib_flags -L$D -ljvm"
            fi
        fi

        # IBM Java Compiler under Suse (and probably others)

        F=jre/bin/classic/libjvm.so
        if test "x$ac_java_jvm_jni_lib_flags" = "x" ; then
            AC_MSG_LOG([Looking for $ac_java_jvm_dir/$F])
            if test -f $ac_java_jvm_dir/$F ; then
                AC_MSG_LOG([Found $ac_java_jvm_dir/$F])

                D=`dirname $ac_java_jvm_dir/$F`
                ac_java_jvm_jni_lib_runtime_path=$D
                ac_java_jvm_jni_lib_flags="-L$D -ljvm"

                D=$ac_java_jvm_dir/bin/
                ac_java_jvm_jni_lib_runtime_path="${ac_java_jvm_jni_lib_runtime_path}:$D"
                ac_java_jvm_jni_lib_flags="$ac_java_jvm_jni_lib_flags -L$D -ljava"
            fi
        fi

        # Sun JDK 1.4 and 1.5 for Win32 (client JVM)

        F=lib/jvm.lib
        if test "x$ac_java_jvm_jni_lib_flags" = "x" ; then
            AC_MSG_LOG([Looking for $ac_java_jvm_dir/$F])
            if test -f $ac_java_jvm_dir/$F ; then
                # jre/bin/client must contain jvm.dll
                # jre/bin/server directory could also contain jvm.dll,
                # just assume the user wants to use the client JVM.
                DLL=jre/bin/client/jvm.dll
                if test -f $ac_java_jvm_dir/$DLL ; then
                    AC_MSG_LOG([Found $ac_java_jvm_dir/$F])
                    D1=$ac_java_jvm_dir/jre/bin
                    D2=$ac_java_jvm_dir/jre/bin/client
                    ac_java_jvm_jni_lib_runtime_path="${D1}:${D2}"
                    ac_java_jvm_jni_lib_flags="$ac_java_jvm_dir/$F"
                fi
            fi
        fi
    fi

        # Under GNU/Debian on a mipsel CPU, uname -m is still returning mips
        # causing a confusion with mips... Therefor, I have to hardcode this
        # test
        # Note that most of the code is duplicated from
        # Sun/Blackdown 1.4 for Linux (client JVM) tests
        F=jre/lib/mipsel/libjava.so
        if test "x$ac_java_jvm_jni_lib_flags" = "x" ; then
            AC_MSG_LOG([Looking for $ac_java_jvm_dir/$F])
            if test -f $ac_java_jvm_dir/$F ; then
                AC_MSG_LOG([Found $ac_java_jvm_dir/$F])
                D=`dirname $ac_java_jvm_dir/$F`
                ac_java_jvm_jni_lib_runtime_path=$D
                ac_java_jvm_jni_lib_flags="-L$D -ljava -lverify"
                D=$ac_java_jvm_dir/jre/lib/mipsel/client
        if test ! -f $D/libjvm.so; then # Check if it is in the client or server directory
            # Try the server directory
            D=$ac_java_jvm_dir/jre/lib/mipsel/server
        fi
                ac_java_jvm_jni_lib_runtime_path="${ac_java_jvm_jni_lib_runtime_path}:$D"
                ac_java_jvm_jni_lib_flags="$ac_java_jvm_jni_lib_flags -L$D -ljvm"
                D=$ac_java_jvm_dir/jre/lib/mipsel/native_threads
                if test -d $D; then
                  ac_java_jvm_jni_lib_runtime_path="${ac_java_jvm_jni_lib_runtime_path}:$D"
                  ac_java_jvm_jni_lib_flags="$ac_java_jvm_jni_lib_flags -L$D"
                fi
            fi
        fi

    # Generate error for unsupported JVM layout

    if test "x$ac_java_jvm_jni_lib_flags" = "x" ; then
        AC_MSG_ERROR([Could not detect the location of the Java
            shared library. You will need to update java.m4
            to add support for this JVM configuration.])
    fi

    AC_MSG_LOG([Using the following JNI library flags $ac_java_jvm_jni_lib_flags])
    AC_MSG_LOG([Using the following runtime library path $ac_java_jvm_jni_lib_runtime_path])

    AC_MSG_LOG([Using LD_PRELOAD=$ac_java_jvm_ld_preload],1)
    AC_MSG_LOG([Using LD_BIND_NOW=$ac_java_jvm_ld_bind_now],1)

    # Make sure we can compile and link a trivial JNI program

    AC_REQUIRE([AC_PROG_CC])

    AC_CACHE_CHECK(to see if we can link a JNI application,
        ac_cv_java_jvm_working_jni_link,[
        AC_LANG_PUSH(C)
        ac_saved_cflags=$CFLAGS
        ac_saved_libs=$LIBS
        CFLAGS="$CFLAGS $ac_java_jvm_jni_include_flags"
        LIBS="$LIBS $ac_java_jvm_jni_lib_flags"
        AC_TRY_LINK([
            #include <jni.h>
        ],[$libSymbolToTest(NULL,0,NULL);],
            ac_cv_java_jvm_working_jni_link=yes,
            ac_cv_java_jvm_working_jni_link=no)
        AC_LANG_POP()
        CFLAGS=$ac_saved_cflags
        LIBS=$ac_saved_libs
    ])

    # gcc can't link with some JDK .lib files under Win32.
    # Work around this problem by linking with win/libjvm.dll.a

    if test "$ac_cv_java_jvm_working_jni_link" != "yes" &&
      test "$ac_cv_tcl_win32" = "yes"; then
        AC_LANG_PUSH(C)
        ac_saved_cflags=$CFLAGS
        ac_saved_libs=$LIBS
        CFLAGS="$CFLAGS $ac_java_jvm_jni_include_flags"
        LIBS="$LIBS -L$srcdir/win -ljvm"
        AC_TRY_LINK([
            #include <jni.h>
        ],[$libSymbolToTest(NULL,0,NULL);],
            ac_cv_java_jvm_working_jni_link=yes,
            ac_cv_java_jvm_working_jni_link=no)
        AC_LANG_POP()
        CFLAGS=$ac_saved_cflags
        LIBS=$ac_saved_libs

        if test "$ac_cv_java_jvm_working_jni_link" = "yes"; then
            AC_MSG_LOG([Using custom JNI link lib])
            ac_java_jvm_jni_lib_flags="-L$srcdir/win -ljvm"
        fi
    fi

    if test "$ac_cv_java_jvm_working_jni_link" != "yes"; then
        AC_MSG_ERROR([could not link file that includes jni.h
        Either the configure script does not know how to deal with
        this JVM configuration or the JVM install is broken or corrupted.])
    fi
])


#------------------------------------------------------------------------
# AC_JAVA_WITH_JDK
#
#    Check to see if the --with-jdk command line option is given.
#    If it was, then set ac_java_with_jdk to the DIR argument.
#
# Arguments:
#    NONE
#
# VARIABLES SET:
#    ac_java_with_jdk can be set to the directory where the jdk lives
#    ac_java_jvm_name can be set to "jdk"
#------------------------------------------------------------------------

AC_DEFUN([AC_JAVA_WITH_JDK], [
    AC_ARG_WITH(jdk,
    AC_HELP_STRING([--with-jdk=DIR],[use JDK from DIR]))

    if test "$with_jdk" = "no" -o -z "$with_jdk"; then
        NO=op
    elif test "$with_jdk" = "yes" -o \( ! -d "$with_jdk" \); then
        AC_MSG_ERROR([--with-jdk=DIR option, must pass a valid DIR])
    elif test "$with_jdk" != "no" ; then
        AC_MSG_RESULT([Use JDK path specified ($with_jdk)])
        ac_java_jvm_dir=$with_jdk
        ac_java_jvm_name=jdk
    fi
])

#------------------------------------------------------------------------
# AC_JAVA_TOOLS
#
#    Figure out the paths of any Java tools we will need later on.
#
# Arguments:
#    NONE
#
# VARIABLES SET:
#    JAVA
#    JAVA_G
#    JAVAC
#    JAVAH
#    JAR
#    JDB
#    JAVADOC
#
# DEPENDS ON:
#    This macro must be run after the AC_JAVA_DETECT_JVM macro as
#    it depends on the ac_java_jvm_name, ac_java_jvm_version and
#    ac_java_jvm_dir variables
#------------------------------------------------------------------------

AC_DEFUN([AC_JAVA_TOOLS], [

    ac_java_jvm_bin_dir=$ac_java_jvm_dir/bin

    AC_JAVA_TOOLS_CHECK(JAVA, java, $ac_java_jvm_bin_dir)

    # Don't error if java_g can not be found
    AC_JAVA_TOOLS_CHECK(JAVA_G, java_g, $ac_java_jvm_bin_dir, 1)

    if test "x$JAVA_G" = "x" ; then
        JAVA_G=$JAVA
    fi

    TOOL=javah
    AC_JAVA_TOOLS_CHECK(JAVAH, $TOOL, $ac_java_jvm_bin_dir)

    AC_JAVA_TOOLS_CHECK(JAR, jar, $ac_java_jvm_bin_dir)

    AC_JAVA_TOOLS_CHECK(JAVADOC, javadoc, $ac_java_jvm_bin_dir)

    # Don't error if jdb can not be found
    AC_JAVA_TOOLS_CHECK(JDB, jdb, $ac_java_jvm_bin_dir, 1)

    case "$ac_java_jvm_version" in
        *)
            # JDK on Win32 does not allow connection with suspend=n
            if test "$ac_cv_tcl_win32" = "yes"; then
                suspend="y"
            else
                suspend="n"
            fi
            JDB_ATTACH_FLAGS="-attach 8757"
            JAVA_G_FLAGS="-Xdebug -Xrunjdwp:transport=dt_socket,address=8757,server=y,suspend=$suspend"
            JDB_ATTACH_FLAGS="-attach 8757"
            ;;
    esac
])



#------------------------------------------------------------------------
# AC_JAVA_ANT
#
#    Figure out if ant is available and where
#
# Arguments:
#    PATH
#
# VARIABLES SET:
#    ANT
#
#------------------------------------------------------------------------

AC_DEFUN([AC_JAVA_ANT], [
    AC_ARG_WITH(ant,
    AC_HELP_STRING([--with-ant=DIR],[Use ant from DIR]),
    ANTPATH=$withval, ANTPATH=no)
    if test "$ANTPATH" = "no" ; then
        if test -d "$SCI_SRCDIR_FULL/java/ant"; then # Scilab thirdparties
            ANTPATH=$SCI_SRCDIR_FULL/java/ant
            AC_JAVA_TOOLS_CHECK(ANT, ant, $ANTPATH/bin $ANTPATH)
        else
            AC_JAVA_TOOLS_CHECK(ANT, ant)
        fi
    elif test ! -d "$ANTPATH"; then
        AC_MSG_ERROR([--with-ant=DIR option, must pass a valid DIR])
    else
        AC_JAVA_TOOLS_CHECK(ANT, ant, $ANTPATH/bin $ANTPATH)
    fi
])

#------------------------------------------------------------------------
# AC_JAVA_CHECK_PACKAGE
#
# Check if the package (generally a jar file) is available and the class
# usable
#
# Arguments:
#    1. name of the package
#   2. name of the class to test
#   3. used by (Comment)
#   4. Do not stop on error
# VARIABLES SET:
#
#
#------------------------------------------------------------------------

AC_DEFUN([AC_JAVA_CHECK_PACKAGE], [
    AC_MSG_CHECKING($1)
    PACKAGE_JAR_FILE=
    found_jar=no
    saved_ac_java_classpath=$ac_java_classpath
    DEFAULT_JAR_DIR="/usr/share/java /usr/lib/java /usr/share/java /usr/share/java/jar /opt/java/lib /usr/local/java /usr/local/java/jar /usr/local/share/java /usr/local/share/java/jar /usr/local/lib/java $(find /usr/share/java/ -maxdepth 1 -type d 2>/dev/null) $(find /usr/lib64/ -maxdepth 1 -type d 2>/dev/null) $(find  /usr/lib/ -maxdepth 1 -type d 2>/dev/null)  $(find /usr/share/*/lib -maxdepth 1 -type d 2>/dev/null)"
    for jardir in "`pwd`/thirdparty" "`pwd`/jar" $DEFAULT_JAR_DIR; do
      for jar in "$jardir/$1.jar" "$jardir/lib$1.jar" "$jardir/lib$1-java.jar" "$jardir/$1*.jar"; do

# TODO check the behaviour when spaces
        jars_resolved=`ls $jar 2>/dev/null`
        for jar_resolved in $jars_resolved; do # If several jars matches
          if test -e "$jar_resolved"; then
            export ac_java_classpath="$jar_resolved:$ac_java_classpath"
            AC_JAVA_TRY_COMPILE([import $2;], , "no", [
              AC_MSG_RESULT([$jar_resolved])
              found_jar=yes
              PACKAGE_JAR_FILE=$jar_resolved
              break 3
            ], [
            ac_java_classpath=$saved_ac_java_classpath

            ])
          fi
        done
      done
    done
    if test "$found_jar" = "no"; then
      AC_MSG_RESULT([no])
      if test "$4" = "yes"; then
         AC_MSG_WARN([Could not find or use the Java package/jar $1 used by $3 (looking for package $2)])
      else
          AC_MSG_ERROR([Could not find or use the Java package/jar $1 used by $3 (looking for package $2)])
      fi
    fi
])

#------------------------------------------------------------------------
# AC_JAVA_TOOLS_CHECK(VARIABLE, TOOL, PATH, NOERR)
#
#    Helper function that will look for the given tool on the
#    given PATH. If cross compiling and the tool can not
#    be found on the PATH, then search for the same tool
#    on the users PATH. If the tool still can not be found
#    then give up with an error unless NOERR is 1.
#
# Arguments:
#    1. The variable name we pass to AC_PATH_PROG
#    2. The name of the tool
#    3. The path to search on
#    4. Pass 1 if you do not want any error generated
#------------------------------------------------------------------------

AC_DEFUN([AC_JAVA_TOOLS_CHECK], [
    if test "$cross_compiling" = "yes" ; then
        AC_PATH_PROG($1, $2)
    else
        AC_PATH_PROG($1, $2, , $3)
    fi

    # Check to see if $1 could not be found

    m4_ifval([$4],,[
    if test "x[$]$1" = "x" ; then
        AC_MSG_ERROR([Cannot find $2])
    fi
    ])
])
