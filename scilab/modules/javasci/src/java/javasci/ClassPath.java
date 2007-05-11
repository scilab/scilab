/*--------------------------------------------------------------------------*/
/**
 * Loading classes at runtime.
 */
/*--------------------------------------------------------------------------*/
package javasci;
/*--------------------------------------------------------------------------*/
import java.lang.reflect.Method;
import java.io.File;
import java.io.IOException;
import java.net.URL;
import java.net.URLClassLoader;
import java.net.URI;
/*--------------------------------------------------------------------------*/
/**
 * ClassPath to overload java classpath.
 */
public class ClassPath {

private static final Class[] Parameters = new Class[]{URL.class};

/**
 * add a filename to java classpath.
 * @param String s a filename
 * @throws IOException
 */
public static void addFile(final String s) throws IOException {
File f = new File(s);
addFile(f);
}
/*--------------------------------------------------------------------------*/
/**
 * add a file to java classpath.
 * @param File
 */
public static void addFile(final File f) throws IOException {
	
URI uri = f.toURI();
addURL(uri.toURL());
}
/*--------------------------------------------------------------------------*/
/**
 * add a URL to classpath.
 * @param u URL
 * @throws t 
 */
public static void addURL(final URL u) throws IOException {

URLClassLoader sysloader = (URLClassLoader) ClassLoader.getSystemClassLoader();
Class sysclass = URLClassLoader.class;

try {
Method method = sysclass.getDeclaredMethod("addURL", Parameters);
method.setAccessible(true);
method.invoke(sysloader , new Object[] {u });
} catch (Throwable t) {
t.printStackTrace();
throw new IOException("Error, could not add URL to system classloader");
}

}
/*--------------------------------------------------------------------------*/
/**
 * 
 */
public static String[] getClassPath() {
	
URLClassLoader sysloader = (URLClassLoader) ClassLoader.getSystemClassLoader();
URL[] path = sysloader.getURLs();
String[] paths = new String[path.length];
for (int i = 0; i < path.length; i++) {
paths[i] = path[i].getFile();
}
    return paths;
}
/*--------------------------------------------------------------------------*/
}
/*--------------------------------------------------------------------------*/

