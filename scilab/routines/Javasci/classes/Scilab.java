package javasci ;
/********************************************************************************************************/
/* Allan CORNET */
/* INRIA 2005 */
/********************************************************************************************************/
public class Scilab
{
/********************************************************************************************************/
  private static native void Initialize();
  /**
  * Initialize Scilab interface
  */
  
  public static native void Events();  
  /**
  * do a Scilab event 
  * See SCI/examples/callsci/callsciJava/others/ExempleEvent.java 
  */
  
  public static native boolean HaveAGraph();
  /**
  * Detect if a Scilab graphic window is opened
  */

  public static native boolean Exec(String job);
  /**
  * Execute a command in Scilab
  */
  
  public static native boolean ExistVar(String VarName);
  /**
  * Detect if Variable name exists in Scilab 
  */
  
  public static native int TypeVar(String VarName);
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
  
  public static native int GetLastErrorCode();
  /**
  * Get Last Error Code
  * 0 no error
  */
  
  public static boolean ExecuteScilabScript(String scriptfilename)
  /**
  * Execute a scilab script .sce
  */
  {
		return Exec("exec(''"+scriptfilename+"'');");
  }
  
  public static native boolean Finish();
  /**
  * When you finish to use Scilab Call scilab.quit, clean memory, ... 
  */

  /**
  * See SCI/examples/callsci/callsciJava/others 
  * and
  * SCI/examples/callsci/callsciJava/ihm
  * for some simple examples
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
