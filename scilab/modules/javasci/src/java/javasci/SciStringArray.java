package javasci;

/**
 * Scilab Array of String object
 * See SCI/modules/javasci/examples/others for some simple examples
 * @author Allan CORNET - INRIA 2006
 * @author Sylvestre LEDRU - INRIA 2007
 */
public class SciStringArray extends javasci.SciAbstractArray implements java.io.Serializable {
	/********************************************************************************************************/
	private String [] x;



	/**
	 * Constructs a Scilab String Array from a other SciStringArray
	 * @param name the name of the Scilab variable
	 * @param Obj the SciStringArray you want to copy
	 */
	public SciStringArray(String name, SciStringArray Obj) {
		this.name = name;
		this.m = Obj.getNumberOfRows();
		this.n = Obj.getNumberOfCols();
		this.x = new String[m * n];

		this.x = Obj.getData();
		Send();
	}

	/**
	 * Constructs a Scilab String Array 
	 * All cells are initialized to ""
	 * @param name  the name of the Scilab Variable 
	 * @param r number of rows
	 * @param c number of columns
	 */
	public SciStringArray(String name, int r, int c) {
		this.m = r;
		this.n = c;
		this.x = new String[r * c];
		this.name = name;
    
		if ((Scilab.TypeVar(name) == 10)
			& (getNumberOfRowsFromScilab(name) == c) 
			& (getNumberOfColsFromScilab(name) == r)) {
				Get();
			} else {
				for (int i = 0; i < (r * c); i++) {
						x[i] = "";
				}
				Send();
			}
	}

	/**
	 * Constructs a Scilab String Array from a java string array
	 * @param name  the name of the Scilab Variable 
	 * @param r number of rows
	 * @param c number of columns
	 * @param stringArray the array of string with want to copy into
	 */
	public SciStringArray(String name, int r, int c, String [] stringArray) {
		if ((r * c) != stringArray.length) {
				throw new BadDataArgumentException("Bad Matrix call, size of third argument is wrong");
			}
		this.m = r;
		this.n = c;
		this.x = stringArray;
		this.name = name;
		Send();
	}

	/**
	 * Return a specific element in the Array
	 * @param indr Row index
	 * @param indc Column index
	 * @return The element
	 */
	public native String GetElement(int indr, int indc);

	/**
	 * @param str The String
	 * @param indr Row index
	 * @param indc Column index
	 */
	private native void SendString(String str, int indr, int indc);
	
	/**
	 * Return the data
	 * @return the data
	 */
	public String[] getData() {
		Get();
		return x;
	}

	
	/**
	 * Get Matrix from Scilab
	 */
	public void Get() {
		int indr;
		int indc;
  	
		for (indr = 1; indr <= m; indr++) {
				for (indc = 1; indc <= n; indc++) {
						int index = (indc - 1) * m + (indr - 1);
						x[index] = GetElement(indr, indc);
					}
     
			}
	}
	/**
	 * Send Matrix to Scilab
	 */
	public void Send() {
		int indx;
		int indy;

		for (indx = 0; indx < m; indx++) {
				for (indy = 0; indy < n; indy++) {
						int index = indx + indy * m;
						SendString(x[index], indx, indy);
					}
			}
	}
}
/********************************************************************************************************/
