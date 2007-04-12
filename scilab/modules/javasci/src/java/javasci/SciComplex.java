package javasci ;
/**
 * Scilab Complex object
 * See SCI/modules/javasci/examples/others for some simple examples 
 * @author Allan CORNET - INRIA 2006
 * @author Sylvestre LEDRU - INRIA 2007
 */
public class SciComplex extends SciAbstractDataType implements java.io.Serializable  
{
	private String typeDescription="complex";	

	
	/**
	 * Constructs a Scilab Complex from a other SciComplex
	 * @param name the name of the Scilab variable
	 * @param Obj the SciComplex you want to copy
	 */
  public SciComplex(String name,SciComplex Obj) 
  {
  	double[] complexReal=new double[1];
  	double[] complexImaginary=new double[1];

  	complexReal[0]=Obj.getRealPartData();
  	complexImaginary[0]=Obj.getImaginaryPartData();
  	
	sciArray = new SciComplexArray(name,1,1,complexReal,complexImaginary);
 }

	/**
	 * Constructs a Scilab Complex
	 * initialized to 0 for both part (Real & Imaginary)
	 * @param name  the name of the Scilab Variable 
	 */ 
 public SciComplex(String name) 
 {
	double[] complexReal=new double[1];
  	double[] complexImaginary=new double[1];
  		
	sciArray = new SciComplexArray(name,1,1);
  }

	/**
	 * Constructs a Scilab Double from two Java Doubles (RealPart & ImaginaryPart)
	 * @param name the name of the Scilab Variable 
	 * @param RealPart the real value
	 * @param ImaginaryPart the imaginary value
	 */ 
  public SciComplex(String name,double RealPart,double ImaginaryPart)
  {
	double[] complexReal=new double[1];
  	double[] complexImaginary=new double[1];

  	complexReal[0]=RealPart;
  	complexImaginary[0]=ImaginaryPart;
	sciArray = new SciComplexArray(name,1,1,complexReal,complexImaginary);
 }

	/**
	 * Return the real part
	 * @return the real part
	 */
  public double getRealPartData() 
  {
    Get();
    double[] complexReal=((SciComplexArray)sciArray).getRealPartData();
    return complexReal[0];
  }

	/**
	 * Return the imaginary part
	 * @return the imaginary part
	 */
  public double getImaginaryPartData() 
  {
   Get();
   double[] complexImaginary=((SciComplexArray)sciArray).getImaginaryPartData();
   return complexImaginary[0];
  }

	/** 
	 * Display the complex
	 * @return the display of the complex
	 */
  public String toString() 
  {
   	double[] complexImaginary=((SciComplexArray)sciArray).getImaginaryPartData();
   	double[] complexReal=((SciComplexArray)sciArray).getRealPartData();

    Get();
	if (complexReal[0]!=0 && complexImaginary[0]>0) 
	{
		return complexReal[0]+" + "+complexImaginary[0]+"i";
	}
    
    if (complexReal[0]!=0 && complexImaginary[0]<0) 
    {
       return complexReal[0]+" - "+(-complexImaginary[0])+"i";
    }
    if (complexImaginary[0]==0) 
    {
       return String.valueOf(complexReal[0]);
    }

    if (complexReal[0]==0) 
    {
       return complexImaginary[0]+"i";
    }
    /*(unless Inf or NaN)*/
    return complexReal[0]+" + i*"+complexImaginary[0];
        
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

