import java.awt.*;
import java.awt.event.*;

public class MaFenetre extends Frame
{
  private MonPanneau p;
	
  public MaFenetre()
  {
    super("Mon Titre"); // invocation du constructeur de Frame
    p=new MonPanneau();
    this.setLayout(new FlowLayout());
    this.add(p);
    this.pack();
    this.addWindowListener(new WCloser());
  }
}
