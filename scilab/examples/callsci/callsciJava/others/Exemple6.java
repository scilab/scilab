import javasci.* ; 


class  Exemple6 {

  public static void main(String[] args) {
  double b;
  	
  	SciDouble a = new SciDouble("A");
  	Scilab.Exec("A=4;");
  	a.Get();
  	System.out.println("----------------------------------------------------");
  	a.disp();
  	System.out.println("----------------------------------------------------");
  	b=a.getData();
  	
  	System.out.println(b);
    
  	System.out.println("----------------------------------------------------");
 	  SciDouble d = new SciDouble("D",8);
    d.Send();
    Scilab.Exec("P=D*2;");
    Scilab.Exec("Q=P*3;");
    
    SciDouble q = new SciDouble("Q");
    Scilab.Exec("Q=P*3;");
    q.Get();
    q.disp();
    System.out.println("----------------------------------------------------");
    Scilab.Finish();
  }
}

