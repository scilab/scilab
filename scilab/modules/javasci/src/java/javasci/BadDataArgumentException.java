package javasci;
/**
 * This class manages exceptions from the construction of SciDoubleArray , SciStringArray 
 * @author Allan Cornet - INRIA 2005
 */
public class BadDataArgumentException extends RuntimeException {
	/**
	 * Constructor of the exception
	 * @param s the error message
	 */
	BadDataArgumentException(String s) {
    super(s);
  }
}

