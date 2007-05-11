/*-----------------------------------------------------------------------------------*/
/**
* Loading classes at runtime.
*/
/*-----------------------------------------------------------------------------------*/
package javasci;
/*-----------------------------------------------------------------------------------*/ 
import java.lang.reflect.*;
import java.io.*;
import java.net.*;
/*-----------------------------------------------------------------------------------*/ 
public class ClassPath {
 
private static final Class[] parameters = new Class[]{URL.class};
 
public static void addFile(String s) throws IOException {
	File f = new File(s);
	addFile(f);
}
/*-----------------------------------------------------------------------------------*/ 
public static void addFile(File f) throws IOException {
	URI uri = f.toURI();
	addURL(uri.toURL());
}
/*-----------------------------------------------------------------------------------*/  
public static void addURL(URL u) throws IOException {
		
	URLClassLoader sysloader = (URLClassLoader) ClassLoader.getSystemClassLoader();
	Class sysclass = URLClassLoader.class;
 
 getClassPath();
	try {
		Method method = sysclass.getDeclaredMethod("addURL", parameters);
		method.setAccessible(true);
		method.invoke(sysloader , new Object[] {u });
	} catch (Throwable t) {
		t.printStackTrace();
		throw new IOException("Error, could not add URL to system classloader");
	}
	getClassPath();
}
/*-----------------------------------------------------------------------------------*/ 
public static String[] getClassPath() {
	URLClassLoader sysloader = (URLClassLoader) ClassLoader.getSystemClassLoader();
	URL path[] = sysloader.getURLs();
	String[] paths = new String[path.length];
	for (int i = 0; i < path.length; i++) {
		paths[i] = path[i].getFile();
		//System.out.println(paths[i]);
    }
    return paths;
}
/*-----------------------------------------------------------------------------------*/ 
}
/*-----------------------------------------------------------------------------------*/ 

