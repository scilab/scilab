package javasci ;
/********************************************************************************************************/
/* Allan CORNET */
/* INRIA 2007 */
/********************************************************************************************************/
public class SciBooleanArray implements java.io.Serializable  
{
/********************************************************************************************************/
  private boolean [] x ;
  
  private int m, n;
  /* m number of rows */
  /* n number of colons */
  
  private String name; 
/********************************************************************************************************/
  private static native void Initialize();
  /**
  * Initialize Scilab interface
  */
    
  public native boolean Job(String job);
  /**
  * Execute a command in Scilab 
  */
  
  public native void Get();
  /**
  * Get Matrix from Scilab
  */
  
  public native void Send();
  /**
  * Send Matrix to Scilab 
  */
  
  public native boolean GetElement(int indr, int indc);
  /**
  * Get only ONE element from Scilab Matrix 
  * indr AND indc are indices in scilab 
  * in Scilab A=[%t,%t;%t,%f];
  * A(1,1)=%t 
  * A(2,2)=%f 
  */
  
  /**
  * See SCI/examples/callsci/callsciJava/others for some simple examples 
  */
/********************************************************************************************************/  
  static 
  {
    System.loadLibrary("javasci");
    Initialize();
  }
/********************************************************************************************************/  
  public SciBooleanArray(String name,SciBooleanArray Obj) 
  {
    this.name = name;
    this.m = Obj.getRow() ;
    this.n = Obj.getCol();
    this.x = new boolean[m*n];
   
    this.x =Obj.getData() ;
    Send();
  }
/********************************************************************************************************/  
  public SciBooleanArray(String name,int r,int c) 
  {
    this.m = r ;
    this.n = c ;
    this.x = new boolean[r*c];
    this.name = name;
   
    for ( int i = 0 ; i < r*c ; i++)x[i]=false;
    Send();
  }
 /********************************************************************************************************/  
  public SciBooleanArray(String name,int r,int c,boolean [] x )
  {
    if ( r*c != x.length) 
    {
     throw new BadDataArgumentException("Bad Matrix call, size of third argument is wrong");
    }
    this.m = r ;
    this.n = c;
    this.x = x;
    this.name = name;
    Send();
  }
/********************************************************************************************************/
  public int getRow() 
  {
   return m;
  }
/********************************************************************************************************/  
  public int getCol() 
  {
   return n;
  }
/********************************************************************************************************/  
  public String getName()
  {
   return name;
  }
/********************************************************************************************************/  
  public boolean[] getData() 
  {
   Get();
   return x;
  }
/********************************************************************************************************/  
  public void disp() 
  {
	Get();
    System.out.println("Matrix "+ getName() +"=");
    Job( "disp(" + getName() +");");
  }
}
/********************************************************************************************************/
