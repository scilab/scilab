package javasci ;
/********************************************************************************************************/
/* Allan CORNET */
/* INRIA 2006 */
/********************************************************************************************************/
public class SciDouble implements java.io.Serializable  
{
/********************************************************************************************************/
  private SciDoubleArray pObjSciDouble;
  private double[] pTmpDoubleArray;
/********************************************************************************************************/  
/**
* See SCI/examples/callsci/callsciJava/others for some simple examples 
*/
/********************************************************************************************************/  
  
/********************************************************************************************************/  
  public SciDouble(String name,SciDouble Obj) 
  {
  	pTmpDoubleArray=new double[1];
  	pTmpDoubleArray[0]=Obj.getData();
	pObjSciDouble = new SciDoubleArray(name,1,1,pTmpDoubleArray);
  }
/********************************************************************************************************/  
  public SciDouble(String name) 
  {
	pTmpDoubleArray=new double[1];
	pObjSciDouble = new SciDoubleArray(name,1,1);
  }
 /********************************************************************************************************/  
  public SciDouble(String name,double Value )
  {
	pTmpDoubleArray=new double[1];
  	pTmpDoubleArray[0]=Value;
	pObjSciDouble = new SciDoubleArray(name,1,1,pTmpDoubleArray);
  }
/********************************************************************************************************/
  public String getName()
  {
    return  pObjSciDouble.getName();
  }
/********************************************************************************************************/  
  public double getData() 
  {
    Get();
    pTmpDoubleArray=pObjSciDouble.getData();
    return pTmpDoubleArray[0];
  }
/********************************************************************************************************/  
  public void Get() 
  {
   	pObjSciDouble.Get();
  }
 /********************************************************************************************************/     
  public boolean Job(String job)
  {
  	return pObjSciDouble.Job(job);
  }
 /********************************************************************************************************/     
  public void Send()
  {
  	pObjSciDouble.Send();
  }
  /********************************************************************************************************/    
  public void disp() 
  {
    Get();
	System.out.println("double "+ getName() +"=");
    Job( "disp(" + getName() +");");
  }
}
/********************************************************************************************************/  
