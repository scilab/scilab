import javasci.* ; 


class  Exemple10 {

  public static void main(String[] args) {
  	
  	SciComplex a = new SciComplex("A");
		Scilab.Exec("A=1. + 80*%i");
  	System.out.println("----------------------------------------------------");
  	a.disp();
  	System.out.println("----------------------------------------------------");
  	
   	System.out.println("A= "+a.getRealPartData()+"  "+a.getImaginaryPartData()+"  ");
  	System.out.println("----------------------------------------------------");
  	SciComplex s = new SciComplex("S",4,8);
  	s.disp();
  	System.out.println("----------------------------------------------------");
  	System.out.println("S= "+s.getRealPartData()+"  "+s.getImaginaryPartData()+"  ");
  	System.out.println("----------------------------------------------------");
  }
}

