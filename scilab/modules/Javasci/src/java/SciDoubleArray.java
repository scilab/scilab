package javasci ;
/********************************************************************************************************/
/* Allan CORNET */
/* INRIA 2005 */
/********************************************************************************************************/
public class SciDoubleArray implements java.io.Serializable  
{
/********************************************************************************************************/
  private double [] x ;
  
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
  
  public native double GetElement(int indr, int indc);
  /**
  * Get only ONE element from Scilab Matrix 
  * indr AND indc are indices in scilab 
  * in Scilab A=[1,2;3,4];
  * A(1,1)=1 
  * A(2,2)=4 
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
  public SciDoubleArray(String name,SciDoubleArray Obj) 
  {
    this.name = name;
    this.m = Obj.getRow() ;
    this.n = Obj.getCol();
    this.x = new double[m*n];
   
    this.x =Obj.getData() ;
    Send();
  }
/********************************************************************************************************/  
  public SciDoubleArray(String name,int r,int c) 
  {
    this.m = r ;
    this.n = c ;
    this.x = new double[r*c];
    this.name = name;
   
    for ( int i = 0 ; i < r*c ; i++)x[i]=0;
    Send();
  }
 /********************************************************************************************************/  
  public SciDoubleArray(String name,int r,int c,double [] x )
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
  public double[] getData() 
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
