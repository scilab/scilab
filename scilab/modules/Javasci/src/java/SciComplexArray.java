package javasci ;
/********************************************************************************************************/
/* Allan CORNET */
/* INRIA 2006 */
/********************************************************************************************************/
public class SciComplexArray implements java.io.Serializable  
{
/********************************************************************************************************/
  private double [] x ; /* Real part */
  private double [] y ; /* Imaginary part */
  
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
  
  public native double GetRealPartElement(int indr,int indc);
  /**
  * Get only ONE element from Scilab Matrix 
  * indr AND indc are indices in scilab 
  * Get Real Part
  */


  public native double GetImaginaryPartElement(int indr,int indc);
  /**
  * Get only ONE element from Scilab Matrix 
  * indr AND indc are indices in scilab 
  * Get Imaginary Part
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
  public SciComplexArray(String name,SciComplexArray Obj) 
  {
    this.name = name;
    this.m = Obj.getRow() ;
    this.n = Obj.getCol();
    
    this.x = new double[m*n];
    this.x =Obj.getRealPartData() ;
    
    this.y = new double[m*n];
    this.y =Obj.getImaginaryPartData() ;
    
    Send();
  }
/********************************************************************************************************/  
  public SciComplexArray(String name,int r,int c) 
  {
    this.m = r ;
    this.n = c ;
    
    this.x = new double[r*c];
    this.y = new double[r*c];
    
    this.name = name;
   
    for ( int i = 0 ; i < r*c ; i++)
    {
    	x[i]=0;
    	y[i]=0;
    }
    Send();
  }
 /********************************************************************************************************/  
  public SciComplexArray(String name,int r,int c,double [] x,double [] y )
  {
    if ( (r*c != x.length) && (r*c != y.length) )
    {
     throw new BadDataArgumentException("Bad Matrix call, size of third argument is wrong");
    }
    
    this.m = r ;
    this.n = c;
    
    this.x = x;
    this.y = y;
    
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
  public double[] getRealPartData() 
  {
   Get();
   return x;
  }
/********************************************************************************************************/    
  public double[] getImaginaryPartData() 
  {
   Get();
   return y;
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
