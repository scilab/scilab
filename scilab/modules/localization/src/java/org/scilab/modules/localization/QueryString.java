/**
 * @author Allan CORNET - INRIA 2007
 */
package org.scilab.modules.localization;
/*--------------------------------------------------------------------------*/
/**
 * Class used to get localized messages from Scilab
 * @author Allan CORNET - INRIA 2007
 */
 public class QueryString {
/**
 * Constructor
 */
	protected QueryString() {
	/*  indicate that the requested operation is not supported */
	throw new UnsupportedOperationException(); 		
}
	 
/**
* get string associated to the tag in errors table
* @param tag a string, ex : "ERROR_1" 
* @return a text associated to "ERROR_1"
*/
public static native String error(String tag);

/**
* get string associated to the tag in messages table
* @param tag a string, ex : "MESSAGE_1"
* @return a text associated to MESSAGE_1
*/
public static native String message(String tag);

/**
* get string associated to the tag in menus table
* @param tag a string, ex : "MENU_1"
* @return a text associated to "MENU_1"  
*/
public static native String menu(String tag);

static 
{
	try {
		System.loadLibrary("scilocalization");
	} catch (SecurityException e) {
		System.err.println("A security manager exists and does not allow the loading of the specified dynamic library :");
		e.printStackTrace(System.err);
	} catch (UnsatisfiedLinkError e)	{
		System.err.println("The native library scilocalization does not exist or cannot be found.");
		e.printStackTrace(System.err);
	}
}
 	
}
/*--------------------------------------------------------------------------*/
