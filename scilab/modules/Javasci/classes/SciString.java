package javasci ;
/********************************************************************************************************/
/* Allan CORNET */
/* INRIA 2006 */
/********************************************************************************************************/
public class SciString implements java.io.Serializable  
{
/********************************************************************************************************/
  private SciStringArray pObjSciString;
  private String[] pTmpStringArray;
/********************************************************************************************************/  
/**
* See SCI/examples/callsci/callsciJava/others for some simple examples
*/
/********************************************************************************************************/  
  public SciString(String name,SciString Obj) 
  {
  	pTmpStringArray=new String[1];
  	pTmpStringArray[0]=Obj.getData();
	pObjSciString = new SciStringArray(name,1,1,pTmpStringArray);
  }
/********************************************************************************************************/  
  public SciString(String name) 
  {
  	pTmpStringArray=new String[1];
	pObjSciString = new SciStringArray(name,1,1);
  }
 /********************************************************************************************************/  
  public SciString(String name,String StrValue )
  {
  	pTmpStringArray=new String[1];
  	pTmpStringArray[0]=StrValue;
	pObjSciString = new SciStringArray(name,1,1,pTmpStringArray);
  }
/********************************************************************************************************/
  public String getName()
  {
    return  pObjSciString.getName();
  }
/********************************************************************************************************/  
  public void Get() 
  {
    pObjSciString.Get();
  }
 /********************************************************************************************************/
  public String getData() 
  {
    Get();
    pTmpStringArray=pObjSciString.getData();
    return pTmpStringArray[0];
  }
 /********************************************************************************************************/          
  public void Send()
  {
  	pObjSciString.Send();
  }
/********************************************************************************************************/    
  public void disp() 
  {
    Get();
	System.out.println("String "+ getName() +"=");
    Job( "disp(" + getName() +");");
  }
/********************************************************************************************************/    
  public boolean Job(String job)
  {
  	return pObjSciString.Job(job);
  }
/********************************************************************************************************/    
}
/********************************************************************************************************/  
  
