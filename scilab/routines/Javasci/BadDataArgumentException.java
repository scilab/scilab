package javasci ;

/**
 * Cette classe permet de gérer les exceptions dans le 
 * constructeur de <tt>Matrix</tt>
 */

public class BadDataArgumentException  extends RuntimeException {
  BadDataArgumentException(String s) {
    super(s);
  }
}

