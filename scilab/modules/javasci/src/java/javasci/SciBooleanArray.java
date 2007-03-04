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
/**
 * Initialize Scilab interface
 */
 private static native void Initialize();
  
/**
 * internal method to know dim (Row) in scilab 
 * returns >= 0 OK
 * returns -1 name not exist
 */
 private native int getNumberOfRowsFromScilab(String name);
  
/**
 * internal method to know dim (Col) in scilab
 * returns >= 0 OK
 * returns -1 name not exist
 */
 private native int getNumberOfColsFromScilab(String name);
    
/**
 * @deprecated 
 * Deprecated.  Use Scilab.Exec instead.
 * Execute a command in Scilab 
 */
 public native boolean Job(String job);
  
/**
 * Get Matrix from Scilab
 */  
 public native void Get();

/**
 * Send Matrix to Scilab 
 */
 public native void Send();
  
/**
 * Get only ONE element from Scilab Matrix 
 * indr AND indc are indices in scilab 
 * in Scilab A=[%t,%t;%t,%f];
 * A(1,1)=%t 
 * A(2,2)=%f 
 */
 public native boolean GetElement(int indr, int indc);
  
/**
 * See SCI/modules/javasci/examples/others for some simple examples 
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
	this.m = Obj.getNumberOfRows() ;
	this.n = Obj.getNumberOfCols();
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
    
	if ( (Scilab.TypeVar(name) == 4) & 
			(getNumberOfRowsFromScilab(name) == r) & 
			(getNumberOfColsFromScilab(name) == c) )
  {
		Get();
  }
  else
  {
		for ( int i = 0 ; i < r*c ; i++)x[i]=false;
		Send();
  }
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
 public int getNumberOfRows() 
 {
	return m;
 }
/********************************************************************************************************/  
 public int getNumberOfCols() 
 {
	return n;
 }
/********************************************************************************************************/  
/**
 * @deprecated 
 * Deprecated.  Use getNumberOfRows() instead.
 */
 public int getRow()
 {
	return m;
 }
/********************************************************************************************************/  
/**
 * @deprecated 
 * Deprecated.  Use getNumberOfCols() instead.
 */
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
	Scilab.Exec( "disp(" + getName() +");");
 }
}
/********************************************************************************************************/
