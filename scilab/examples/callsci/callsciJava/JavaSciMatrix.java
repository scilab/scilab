// Une petite classe de Matrices 

class  JavaSciMatrix {
  private double [] x ;
  private int m,n;
  String name; 

  JavaSciMatrix(String name,int m,int n) 
  {
    x = new double[m*n];
    this.m = m ;
    this.n = n;
    this.name = name;
  }

  JavaSciMatrix(String name,int m,int n,double []x )
  {
    this.x = x ;  this.m = m ;  this.n = n;
    this.name = name;
  }

  public native void scilabJob(String job) ;

  public void inv() { scilabJob( name + "=inv(" + name +");");}
  public void show() {
    System.out.println("Matrix "+ name +"=");
    scilabJob( "disp(" + name +");");
  }

  public void rand() { 
    scilabJob(  name + "=rand(" + m +"," + n +");");
  }

  static 
  {
    System.loadLibrary("javasci");
  }

  public static void main(String[] args) {
    JavaSciMatrix m = new JavaSciMatrix("A",4,4);
    m.rand();
    m.show();
    JavaSciMatrix n = new JavaSciMatrix("B",2,2, new double [] {1,2,3,4} );
    n.show();
    n.inv();
    n.show();
  }
}



