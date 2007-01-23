package javasci ;
/********************************************************************************************************/
/* Allan CORNET */
/* INRIA 2007 */
/********************************************************************************************************/
public class SciBoolean implements java.io.Serializable  
{
/********************************************************************************************************/
  private SciBooleanArray pObjSciBoolean;
  private boolean[] pTmpBooleanArray;
/********************************************************************************************************/  
/**
* See SCI/examples/callsci/callsciJava/others for some simple examples 
*/
/********************************************************************************************************/  
  
/********************************************************************************************************/  
  public SciBoolean(String name,SciBoolean Obj) 
  {
  	pTmpBooleanArray=new boolean[1];
  	pTmpBooleanArray[0]=Obj.getData();
	  pObjSciBoolean = new SciBooleanArray(name,1,1,pTmpBooleanArray);
  }
/********************************************************************************************************/  
  public SciBoolean(String name) 
  {
	pTmpBooleanArray=new boolean[1];
	pObjSciBoolean = new SciBooleanArray(name,1,1);
  }
 /********************************************************************************************************/  
  public SciBoolean(String name,boolean Value )
  {
	pTmpBooleanArray=new boolean[1];
  pTmpBooleanArray[0]=Value;
	pObjSciBoolean = new SciBooleanArray(name,1,1,pTmpBooleanArray);
  }
/********************************************************************************************************/
  public String getName()
  {
    return  pObjSciBoolean.getName();
  }
/********************************************************************************************************/  
  public boolean getData() 
  {
    Get();
    pTmpBooleanArray=pObjSciBoolean.getData();
    return pTmpBooleanArray[0];
  }
/********************************************************************************************************/  
  public void Get() 
  {
   	pObjSciBoolean.Get();
  }
 /********************************************************************************************************/     
  public boolean Job(String job)
  {
  	return pObjSciBoolean.Job(job);
  }
 /********************************************************************************************************/     
  public void Send()
  {
  	pObjSciBoolean.Send();
  }
  /********************************************************************************************************/    
  public void disp() 
  {
    Get();
	System.out.println("boolean "+ getName() +"=");
    Job( "disp(" + getName() +");");
  }
}
/********************************************************************************************************/  
