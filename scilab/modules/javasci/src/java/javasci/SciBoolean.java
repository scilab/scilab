package javasci;

/**
 * Scilab Boolean object
 * See SCI/modules/javasci/examples/others for some simple examples 
 * @author Allan CORNET - INRIA 2007
 * @author Sylvestre LEDRU - INRIA 2007
 */
public class SciBoolean extends SciAbstractDataType implements java.io.Serializable  
{
	private String typeDescription="boolean";
	
	/**
	 * Constructs a Scilab Boolean from a other SciBoolean
	 * @param name the name of the Scilab variable
	 * @param Obj the Sciboolean you want to copy
	 */
	public SciBoolean(String name,SciBoolean Obj) 
	{
		boolean[] pTmpBooleanArray=new boolean[1];
		pTmpBooleanArray[0]=Obj.getData();
		sciArray = new SciBooleanArray(name,1,1,pTmpBooleanArray);
	}

	/**
	 * Constructs a Scilab Boolean
	 * @param name the name of the Scilab variable
	 */
	public SciBoolean(String name) 
	{
		boolean[] pTmpBooleanArray=new boolean[1];
		sciArray = new SciBooleanArray(name,1,1);

	}

	/**
	 * Constructs a Scilab Boolean from a java boolean
	 * @param name the name of the Scilab variable
	 * @param Value the boolean value
	 */
	public SciBoolean(String name, boolean value)
	{
		boolean[] pTmpBooleanArray=new boolean[1];
		pTmpBooleanArray[0]=value;
		sciArray = new SciBooleanArray(name,1,1,pTmpBooleanArray);
	}

	/**
	 * Returns the boolean value
	 * @return the value
	 */	   
	public boolean getData() 
	{
		Get();
		boolean[] pTmpBooleanArray=((SciBooleanArray)sciArray).getData();
		return pTmpBooleanArray[0];
	}

	
	/**
	 * Return the description of the DataType
	 * Description must set in any class which extends SciAbstractDataType
	 * @return the Description
	 *
	 */
	public String getTypeDescription()
	{
		return this.typeDescription;
	}

}
/********************************************************************************************************/  
