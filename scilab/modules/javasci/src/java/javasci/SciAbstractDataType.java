package javasci;

/**
 * Defines the basic method that very SciDataType should implement
 * Cannot be implemented
 * @author Sylvestre LEDRU - INRIA 2007
 */
public abstract class SciAbstractDataType {
	/**
	 * the data 
	 */
	protected SciAbstractArray sciArray;

	/**
	 * Return the description of the DataType
	 * Description must set in any class which extends SciAbstractDataType
	 * @return the Description
	 *
	 */
	abstract String getTypeDescription();

	/**
	 * Returns the name of the Scilab variable
	 * @return the name
	 */
	public String getName()	{
		return sciArray.getName();
	}

	/**
	 * Load the data from Scilab
	 */
	public void Get() {
		sciArray.Get();
	}
	
	/**
	 * Send the data to Scilab 
	 */
	public void Send() {
		sciArray.Send();
	}
	

	/**
	 * Execute a command in Scilab 
	 * Deprecated. Use Scilab.Exec instead.
	 * @param job the Scilab job 
	 * @return the result of the operation
	 * @deprecated
	 */
	public boolean Job(String job) {
		return sciArray.Job(job);
	}  

	/** 
	 * Display the variable 
	 */
	public void disp() {
		Get();
		System.out.println(getTypeDescription() + " " + getName() + "=");
		//				System.out.println("Scilab.Exec( \"disp(" + getName() +");\");");
		Scilab.Exec("disp(" + getName() + ");");
	}
}
