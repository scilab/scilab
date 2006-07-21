package javasci ;
/********************************************************************************************************/
/* Allan CORNET */
/* INRIA 2006 */
/********************************************************************************************************/
public class SciComplex implements java.io.Serializable  
{
/********************************************************************************************************/
  private SciComplexArray pObjSciComplex;
  private double[] pTmpComplexRealArray;
  private double[] pTmpComplexImaginaryArray;
/********************************************************************************************************/  
/**
* See SCI/examples/callsci/callsciJava/others for some simple examples 
*/
/********************************************************************************************************/  
  
/********************************************************************************************************/  
  public SciComplex(String name,SciComplex Obj) 
  {
  	pTmpComplexRealArray=new double[1];
  	pTmpComplexImaginaryArray=new double[1];
  	
  	pTmpComplexRealArray[0]=Obj.getRealPartData();
  	pTmpComplexImaginaryArray[0]=Obj.getImaginaryPartData();
  	
	pObjSciComplex = new SciComplexArray(name,1,1,pTmpComplexRealArray,pTmpComplexImaginaryArray);
  }
/********************************************************************************************************/  
  public SciComplex(String name) 
  {
	pTmpComplexRealArray=new double[1];
  	pTmpComplexImaginaryArray=new double[1];
  		
	pObjSciComplex = new SciComplexArray(name,1,1);
  }
 /********************************************************************************************************/  
  public SciComplex(String name,double RealPart,double ImaginaryPart)
  {
	pTmpComplexRealArray=new double[1];
  	pTmpComplexImaginaryArray=new double[1];
  	
  	pTmpComplexRealArray[0]=RealPart;
  	pTmpComplexImaginaryArray[0]=ImaginaryPart;
	pObjSciComplex = new SciComplexArray(name,1,1,pTmpComplexRealArray,pTmpComplexImaginaryArray);
  }
/********************************************************************************************************/
  public String getName()
  {
    return  pObjSciComplex.getName();
  }
/********************************************************************************************************/  
  public double getRealPartData() 
  {
    Get();
    pTmpComplexRealArray=pObjSciComplex.getRealPartData();
    return pTmpComplexRealArray[0];
  }
/********************************************************************************************************/  
  public double getImaginaryPartData() 
  {
   Get();
   pTmpComplexImaginaryArray=pObjSciComplex.getImaginaryPartData();
   return pTmpComplexImaginaryArray[0];
  }
/********************************************************************************************************/  
  public void Get() 
  {
   	pObjSciComplex.Get();
   	pTmpComplexImaginaryArray=pObjSciComplex.getImaginaryPartData();
   	pTmpComplexRealArray=pObjSciComplex.getRealPartData();
  }
 /********************************************************************************************************/     
  public boolean Job(String job)
  {
  	return pObjSciComplex.Job(job);
  }
 /********************************************************************************************************/     
  public void Send()
  {
  	pObjSciComplex.Send();
  }
  /********************************************************************************************************/    
  public void disp() 
  {
    Get();
	System.out.println("double "+ getName() +"=");
    Job( "disp(" + getName() +");");
  }
  /********************************************************************************************************/    
  public String toString() 
  {
    Get();
	if (pTmpComplexRealArray[0]!=0 && pTmpComplexImaginaryArray[0]>0) 
	{
		return pTmpComplexRealArray[0]+" + "+pTmpComplexImaginaryArray[0]+"i";
    }
    
    if (pTmpComplexRealArray[0]!=0 && pTmpComplexImaginaryArray[0]<0) 
    {
       return pTmpComplexRealArray[0]+" - "+(-pTmpComplexImaginaryArray[0])+"i";
    }
    
    if (pTmpComplexImaginaryArray[0]==0) 
    {
       return String.valueOf(pTmpComplexRealArray[0]);
    }
    
    if (pTmpComplexRealArray[0]==0) 
    {
       return pTmpComplexImaginaryArray[0]+"i";
    }
    
    /*(unless Inf or NaN)*/
    return pTmpComplexRealArray[0]+" + i*"+pTmpComplexImaginaryArray[0];
        
  }       
  /********************************************************************************************************/    
}
/********************************************************************************************************/  
