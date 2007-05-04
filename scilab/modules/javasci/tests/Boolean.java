import javasci.* ; 


class  Boolean {

  public static void main(String[] args) {

    boolean b;
  	
  	SciBoolean a = new SciBoolean("A");
  	Scilab.Exec("A=%t;");
  	System.out.println("----------------------------------------------------");
  	a.disp();
  	System.out.println("----------------------------------------------------");
  	b=a.getData();
		
  	System.out.println(b);
    
  	System.out.println("----------------------------------------------------");
	  SciBoolean d = new SciBoolean("D",true);
	  SciBoolean p = new SciBoolean("P");
    Scilab.Exec("P=~D;");
    p.disp();
    System.out.println("----------------------------------------------------");
    Scilab.Finish();
  }
}

