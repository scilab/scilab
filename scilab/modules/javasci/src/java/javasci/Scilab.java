package javasci ;

/** 
 * Java interface for Scilab
 * @author Allan CORNET - INRIA
 */
public class Scilab
{
  /**
  * Initialize Scilab interface
  */
  private static native void Initialize();

  /**
  * do a Scilab event 
  * See SCI/modules/javasci/examples/others/ExempleEvent.java 
  * @see ExempleEvent
  */  
  public static native void Events();  

  /**
  * Detect if a Scilab graphic window is opened
  * @return if have a graph or not
  */
  public static native boolean HaveAGraph();

  /**
  * Execute a command in Scilab
  * @param job the scilab job
  * @return
  */
  public static native boolean Exec(String job);

  /**
  * Detect if Variable name exists in Scilab 
  * @param varName 
  * @return
  */
  public static native boolean ExistVar(String varName);

	/**
	 * Get Scilab type of a variable 
	 * <ul>
	 * <li> -1 : does not exist
	 * <li> 1 : real or complex constant matrix. 
	 * <li> 2 : polynomial matrix.
	 * <li> 4 : boolean matrix.
	 * <li> 5 : sparse matrix.
	 * <li> 6 : sparse boolean matrix.
	 * <li> 8 : matrix of integers stored on 1 2 or 4 bytes 
	 * <li> 9 : matrix of graphic handles 
	 * <li> 10 : matrix of character strings.
	 * <li> 11 : un-compiled function. 
	 * <li> 13 : compiled function.
	 * <li> 14 : function library.
	 * <li> 15 : list.
	 * <li> 16 : typed list (tlist)
	 * <li> 17 : mlist 
	 * <li> 128 : pointer 
	 * </ul>
	 *
	 * @param varName the name of the variable defined into Scilab
	 * @return the code of the type of Scilab variable
	 */  
	public static native int TypeVar(String varName);

  /**
  * Get Last Error Code
  * @return the error code
  * @return the last error code (0 if no error)
  */  
  public static native int GetLastErrorCode();

  /**
  * Execute a scilab script .sce
  * @param scriptFilename the path to the .sce file
  * @return 
  */
  public static boolean ExecuteScilabScript(String scriptFilename)
  {
		return Exec("exec('"+scriptFilename+"');");
  }

  /**
  * When you finish to use Scilab Call scilab.quit, clean memory, ... 
  * @return 
  */  
  public static native boolean Finish();

/********************************************************************************************************/

	static 
	{
		try 
			{
				System.loadLibrary("javasci");
				Initialize();
			} 
		catch(SecurityException e)
			{
				System.err.println("A security manager exists and does not allow the loading of the specified dynamic library :");
				e.printStackTrace(System.err);
			} 
		catch(UnsatisfiedLinkError e)
			{
				System.err.println("The native library javasci does not exist or cannot be found.");
				e.printStackTrace(System.err);
			}
		
	}
}

