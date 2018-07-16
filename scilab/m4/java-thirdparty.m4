dnl
dnl Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
dnl Copyright (C) DIGITEO - 2010 - Sylvestre Ledru
dnl Copyright (C) Scilab Enterprises - 2015 - Clement David
dnl 
dnl Copyright (C) 2012 - 2016 - Scilab Enterprises
dnl
dnl This file is hereby licensed under the terms of the GNU GPL v2.0,
dnl pursuant to article 5.3.4 of the CeCILL v.2.1.
dnl This file was originally licensed under the terms of the CeCILL v2.1,
dnl and continues to be available under such terms.
dnl For more information, see the COPYING file which you should have received
dnl along with this program.
dnl

#------------------------------------------------------------------------
# AC_JAVA_COMPILE_CHECKER_CLASS (internally used)
#
# Compile a shared conftestSharedChecker.java class that will be used by 
# AC_JAVA_CHECK_JAR later. Using only one shared class to check all 
# java package slightly reduce the configure time.
#
# Arguments: None
# VARIABLES SET:
#   None but the conftestSharedChecker.class will exist
#
#------------------------------------------------------------------------
AC_DEFUN([AC_JAVA_COMPILE_CHECKER_CLASS], [
    cat << \EOF > conftestSharedChecker.java
// [#]line __oline__ "configure"
import java.util.regex.Pattern;
import java.io.File;
import java.io.IOException;
import java.lang.reflect.Field;
import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;
import java.net.URL;
import java.net.URLClassLoader;
import java.util.ArrayList;

public class conftestSharedChecker {

    /**
     * Required arguments :
     *  argv[[0]]: name of the jar package
     *  argv[[1]]: name of the class (with the package prefix)
     *  argv[[2]]: possible paths of the jar package (separated by ':')
     *
     * Optional arguments:
     *  argv[[3]]: field or method used to retrieve the version
     *  argv[[4]]: expected version
     *  argv[[5]]: String comparator :
     *      "="  for exact version
     */
    public static void main(String[[]] argv) {
        final String packageName = argv[[0]];
        final String className = argv[[1]];
        final String[[]] jarList = argv[[2]].split(":");

        ArrayList<URL> found = new ArrayList<>();
        for(String jar : jarList)
        {
            try {
                File f = new File(jar);
                if (f.exists()) {
                    found.add(f.toURI().toURL());
                }
            } catch (IOException ex) {
                System.err.println(ex.toString());
            }
        }
        
        URLClassLoader localClassLoader = new URLClassLoader(found.toArray(new URL[[found.size()]]));
        URL klassURL = null;
        Class<?> klass = null;
        try {
            String resourceName = className.replace(".", "/") + ".class";
            klassURL = localClassLoader.getResource(resourceName);
            klass = localClassLoader.loadClass(className);
        } catch (ClassNotFoundException ex) {
            System.err.println(className + " not found");
            System.exit(-1);
        }
        
        String pathURL = klassURL.getPath().substring(0, klassURL.getPath().indexOf('!'));
        String path = pathURL.substring(pathURL.indexOf(':') + 1);
        System.err.println("found: " + path);
        
        String version;
        if (!"".equals(argv[[3]])) {
            version = checkVersion(klass, argv);
        } else {
            version = klass.getPackage().getSpecificationVersion();
        }
        
        System.out.println(path + " " + version);
    }

    private static String checkVersion(Class<?> klass, String[[]] argv) throws SecurityException, IllegalArgumentException {
        final String versionMethod = argv[[3]];
        final String expected = argv[[4]];
        final String atLeastOrEqual;
        if (!"".equals(argv[[5]])) {
            atLeastOrEqual = argv[[5]];
        } else {
            atLeastOrEqual = "<=";
        }
        
        String value = "";
        if (versionMethod.isEmpty()) {
            value = klass.getPackage().getSpecificationVersion();
            compareAndDisplay(atLeastOrEqual, expected, value);
            return value;
        }

        try {
            try {
                Field field = klass.getField(versionMethod);
                value = String.valueOf(field.get(null));
                compareAndDisplay(atLeastOrEqual, expected, value);
            } catch (NoSuchFieldException fe) {
                Method method = null;
                try {
                    method = klass.getMethod(versionMethod);
                } catch (NoSuchMethodException ex) {
                    System.err.println(ex.toString());
                    System.exit(-3);
                }
                
                try {
                    value = String.valueOf(method.invoke(null));
                    compareAndDisplay(atLeastOrEqual, expected, value);
                } catch (NullPointerException ex) {
                    value = String.valueOf(method.invoke(klass.newInstance()));
                    compareAndDisplay(atLeastOrEqual, expected, value);
                }
            }
        } catch (IllegalAccessException ex) {
            System.err.println(ex);
            System.exit(-2);
        } catch (InvocationTargetException ex) {
            System.err.println(ex);
            System.exit(-2);
        } catch (InstantiationException ex) {
            System.err.println(ex);
            System.exit(-2);
        }
        return value;
    }

    private static void compareAndDisplay(String atLeastOrEqual, String expected, String value) {
        int cmp = compare(expected, value);
        
        switch(atLeastOrEqual) {
            case ">":
                if (cmp > 0) {
                    return;
                } else {
                    System.err.println("expected "+atLeastOrEqual+""+expected+" but got "+value);
                    System.exit(-4);
                }
            case "==":
                if (cmp == 0) {
                    return;
                } else {
                    System.err.println("expected "+atLeastOrEqual+""+expected+" but got "+value);
                    System.exit(-4);
                }
            case "<=":
                if (cmp <= 0) {
                    return;
                } else {
                    System.err.println("expected "+atLeastOrEqual+""+expected+" but got "+value);
                    System.exit(-4);
                }
            default:
                System.err.println("unable to compare with "+atLeastOrEqual);
                System.exit(-4);
        }
    }
    
    private static int compare(String v1, String v2) {
        String s1 = normalisedVersion(v1);
        String s2 = normalisedVersion(v2);

        System.err.println("compare: " + v1 + " normalised to " + s1);
        System.err.println("compare: " + v2 + " normalised to " + s2);
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
    cmd="$JAVAC ${JAVAC_FLAGS} conftestSharedChecker.java"
    if (echo $cmd >&AS_MESSAGE_LOG_FD ; eval $cmd >conftestSharedChecker.java.output 2>&AS_MESSAGE_LOG_FD) ; then
        echo "configure: conftestSharedChecker.class available" >&AS_MESSAGE_LOG_FD
    else
        echo "configure: failed program was:" >&AS_MESSAGE_LOG_FD
        cat conftestSharedChecker.java >&AS_MESSAGE_LOG_FD
        echo "configure: CLASSPATH was $CLASSPATH" >&AS_MESSAGE_LOG_FD
    fi
])


#------------------------------------------------------------------------
# AC_JAVA_CHECK_JAR
#
# Check if the package (generally a jar file) is available and the class
# usable
#
# Arguments:
#   1. name of the package
#   2. name of the class to test
#   3. used by (Comment)
#   4. (optional) warn if "yes" else error on not found
#   5. (optional, needed to check the version) version field or method, SpecificationVersion by default
#   6. (optional, needed to check the version) version value to check
#   7. (optional) "=" or "<=" to compare version
#------------------------------------------------------------------------
AC_DEFUN([AC_JAVA_CHECK_JAR], [
    AC_REQUIRE([AC_JAVA_COMPILE_CHECKER_CLASS])

    AC_MSG_CHECKING($1)
    PACKAGE_JAR_FILE=

    DEFAULT_JAR_DIR="$(pwd)/thirdparty/ $(pwd)/jar/ /usr/local/java/ /usr/local/java/jar /usr/local/share/java/ /usr/local/share/java/jar/ /usr/local/lib/java/ $(find /usr/share/java/ -maxdepth 1 -type d 2>/dev/null | sort) $(find /usr/lib64/ -maxdepth 1 -type d 2>/dev/null) $(find  /usr/lib/ -maxdepth 1 -type d 2>/dev/null) $(find /usr/share/*/lib -maxdepth 1 -type d 2>/dev/null) /opt/java/lib/"
   
    jar_resolved="$(find $DEFAULT_JAR_DIR -maxdepth 1 \( -type f -name '$1.jar' -or -name 'lib$1.jar' -or -name 'lib$1-java.jar' -or -name '$1*.jar' \) 2>/dev/null |tr '\n' ':')."

    if test ! -f conftestSharedChecker.class ; then
	AC_JAVA_COMPILE_CHECKER_CLASS()
    fi

    CLASSPATH=$ac_java_classpath
    export CLASSPATH
    echo "CLASSPATH="$CLASSPATH >&AS_MESSAGE_LOG_FD
    cmd="$JAVA conftestSharedChecker \"$1\" \"$2\" \"$jar_resolved\" \"$5\" \"$6\" \"$7\""
    if (echo $cmd >&AS_MESSAGE_LOG_FD ; eval $cmd >conftestSharedChecker.java.output 2>&AS_MESSAGE_LOG_FD); then
        read PACKAGE_JAR_FILE PACKAGE_JAR_VERSION << EOF
$(tail -n 1 conftestSharedChecker.java.output)
EOF
        AC_MSG_RESULT([ $PACKAGE_JAR_FILE $PACKAGE_JAR_VERSION ])
        echo "yes" >&AS_MESSAGE_LOG_FD
        # append the found file to the classpath to manage jar dependency
        ac_java_classpath="$ac_java_classpath:$PACKAGE_JAR_FILE"
    else
      AC_MSG_RESULT([no])
      if test "$4" = "yes"; then
         AC_MSG_WARN([Could not find or use the Java package/jar $1 used by $3 (looking for package $2)])
      else
          AC_MSG_ERROR([Could not find or use the Java package/jar $1 used by $3 (looking for package $2)])
      fi
    fi
    if test -f conftestSharedChecker.java.output; then
        rm conftestSharedChecker.java.output
    fi
])

