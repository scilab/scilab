import javasci.* ; 


class  Exemple1 {

  public static void main(String[] args) {
    SciReal a = new SciReal("A",1,4,new double [] {1,2,3,4});
    a.Send();
    Scilab.Exec("P=poly(A,''x'',''coeffs'');");
    Scilab.Exec("Q=real(roots(P));");
    
    SciReal q = new SciReal("Q",1,3);
    q.Get();
    q.disp();
  }
}

