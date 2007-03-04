package javasci ;
/********************************************************************************************************/
/* Allan CORNET */
/* INRIA 2005 */
/********************************************************************************************************/
public class Scilab
{
/********************************************************************************************************/
/**
 * Initialize Scilab interface
 */
 private static native void Initialize();

/**
 * do a Scilab event 
 * See SCI/modules/javasci/examples/others/ExempleEvent.java 
 */
 public static native void Events();  

/**
 * Detect if a Scilab graphic window is opened
 */
 public static native boolean HaveAGraph();

/**
 * Execute a command in Scilab
 */
 public static native boolean Exec(String job);

/**
 * Detect if Variable name exists in Scilab 
 */
 public static native boolean ExistVar(String VarName);

/**
 * Get Scilab type  of a variable 
 * -1 not exist 
 * 1 : real or complex constant matrix. 
 * 2 : polynomial matrix.
 * 4 : boolean matrix.
 * 5 : sparse matrix.
 * 6 : sparse boolean matrix.
 * 8 : matrix of integers stored on 1 2 or 4 bytes 
 * 9 : matrix of graphic handles 
 * 10 : matrix of character strings.
 * 11 : un-compiled function. 
 * 13 : compiled function.
 * 14 : function library.
 * 15 : list.
 * 16 : typed list (tlist)
 * 17 : mlist 
 * 128 : pointer 
 */
 public static native int TypeVar(String VarName);

/**
 * Get Last Error Code
 * 0 no error
 */
 public static native int GetLastErrorCode();

/**
 * Execute a scilab script .sce
 */
 public static boolean ExecuteScilabScript(String scriptfilename)
 {
	return Exec("exec('"+scriptfilename+"');");
 }

/**
 * When you finish to use Scilab Call scilab.quit, clean memory, ... 
 */
 public static native boolean Finish();

/**
 * See SCI/modules/javasci/examples/others 
 */
/********************************************************************************************************/
 static 
 {
	System.loadLibrary("javasci");
	Initialize();
 }
/********************************************************************************************************/
}
/********************************************************************************************************/
