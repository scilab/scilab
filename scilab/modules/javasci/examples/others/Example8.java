import javasci.* ; 

class  Example8 {

  public static void main(String[] args) 
  {
  
    SciString SCIPATH = new SciString("SCILABROOTPATH");
    Scilab.Exec("SCILABROOTPATH=getenv('SCI');");
    
    Scilab.ExecuteScilabScript(SCIPATH.getData()+"/modules/Javasci/examples/others/test.sce");
    
    /* "notest.sce" not exist */
    if (! Scilab.ExecuteScilabScript(SCIPATH.getData()+"/modules/Javasci/examples/others/notest.sce") )
    {
    	System.out.println("Scilab Error code : "+Scilab.GetLastErrorCode());
    }
     
    Scilab.Finish();
  }
}

  
