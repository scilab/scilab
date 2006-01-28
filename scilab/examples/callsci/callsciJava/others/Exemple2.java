import javasci.* ; 

class  Exemple2 {

  public static void main(String[] args) {
  
    // creation d'une matrice 4x4 
    SciDoubleArray a = new SciDoubleArray("A",4,4);
    // on la remplit aléatoirement (loi uniforme sur (0,1)).
    a.Send();
    a.Job("A=rand(4,4)");
    // display Scilab de la matrice. 
    a.disp();
    SciDoubleArray b = new SciDoubleArray("B",2,2, new double [] {1,2,3,4} );
    b.Send();
    b.disp();
    SciDoubleArray v = new SciDoubleArray ("Void",0,0);
    v.Send();
    v.disp();
    // Attention aux ' il faut les doubler
    // on fait executer une instruction Scilab 
    Scilab.Exec("write(%io(2),''coucou'')");
    SciDoubleArray h = new SciDoubleArray("H",2,2, new double [] {1,2,3,4} );
    // on envoit h a Scilab 
    h.Send();
    // on fait executer une instruction Scilab 
    Scilab.Exec("disp(H)");
       
  }
}

  
