package javasci ;
/********************************************************************************************************/
/* Allan CORNET */
/* INRIA 2006 */
/********************************************************************************************************/
public class SciStringArray implements java.io.Serializable  
{
/********************************************************************************************************/
 private String [] x ;
 private int m, n;
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
 * Deprecated.  Use Scilab.Exec instead.
 * Execute a command in Scilab
 */  
 public native boolean Job(String job);
  
 public native String GetElement(int indr, int indc);
  
 private native void SendString(String str,int indr, int indc);
  
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
 public SciStringArray(String name,SciStringArray Obj) 
 {
	this.name = name;
	this.m = Obj.getNumberOfRows() ;
	this.n = Obj.getNumberOfCols();
	this.x = new String[m*n];
   
	this.x =Obj.getData() ;
	Send();
 }
/********************************************************************************************************/  
 public SciStringArray(String name,int r,int c) 
 {
	this.m = r ;
	this.n = c ;
	this.x = new String[r*c];
	this.name = name;
    
	if ( (Scilab.TypeVar(name) == 10) & 
         (getNumberOfRowsFromScilab(name) == c) & 
         (getNumberOfColsFromScilab(name) == r) )
	{
		Get();
	}
	else
	{
		for ( int i = 0 ; i < r*c ; i++)x[i]="";
		Send();
	}
 }
 /********************************************************************************************************/  
 public SciStringArray(String name,int r,int c,String [] x )
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
 public String[] getData() 
 {
	Get();
	return x;
 }
/********************************************************************************************************/  
 public void Get() 
 {
	int indr, indc;
  	
	for(indr=1;indr<=m;indr++)
	{
		for(indc=1;indc<=n;indc++)
		{
			x[(indc-1)*m+(indr-1)]=GetElement(indr,indc);
		}
	}
 }
/********************************************************************************************************/  
 public void Send() 
 {
	int indx, indy;
  	
	for(indx=0;indx<m;indx++)
	{
		for(indy=0;indy<n;indy++)
		{
			SendString(x[indx+indy*m],indx,indy);
		}
	}
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
