import javasci.* ; 

class  Exemple8 {

  public static void main(String[] args) 
  {
  
    SciString SCIPATH = new SciString("SCILABROOTPATH");
    Scilab.Exec("SCILABROOTPATH=getenv(''SCI'')");
    
    Scilab.ExecuteScilabScript(SCIPATH.getData()+"/examples/callsci/callsciJava/others/test.sce");
    
    /* "notest.sce" not exist */
    if (! Scilab.ExecuteScilabScript(SCIPATH.getData()+"/examples/callsci/callsciJava/others/notest.sce") )
    {
    	System.out.println("Scilab Error code : "+Scilab.GetLastErrorCode());
    }
     
    Scilab.Finish();
  }
}

  
