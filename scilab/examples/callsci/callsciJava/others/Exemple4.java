import javasci.* ; 


class  Exemple4 {

  public static void main(String[] args) 
  {
    SciString S = new SciString("Sentence","Who is the best ?");
    S.Send();
    
    S.disp();
    
    Scilab.Exec("Sentence=''You are the best !'';");
    
    Scilab.Exec("disp(Sentence);");
    
    S.disp();
    
  }
}

