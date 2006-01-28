package javasci ;
/********************************************************************************************************/
/* Allan CORNET */
/* INRIA 2005 */
/********************************************************************************************************/
public class Scilab
{
/********************************************************************************************************/
  public static native void Events();  
  //! do a Scilab event 
  // See SCI/examples/callsci/callsciJava/others/ExempleEvent.java
  
  public static native boolean HaveAGraph();
  //! Detect if a Scilab graphic window is opened 

  public static native boolean Exec(String job);
  /* Execute a command in Scilab */

  /* See SCI/examples/callsci/callsciJava/others AND  SCI/examples/callsci/callsciJava/ihm
     for some simple examples */
/********************************************************************************************************/
  static 
  {
    System.loadLibrary("javasci");
  }
/********************************************************************************************************/
}
/********************************************************************************************************/
