import java.awt.event.*;
import javasci.*;
import java.awt.*;

public class MonEcouteurPourBouton implements ActionListener
{
  private MonPanneau appli;
  public MonEcouteurPourBouton(MonPanneau qui)
  {
    appli=qui;	
  }
  public void actionPerformed(ActionEvent e)
  {
    int i=0;
    appli.YA_EU_UN_Click();
    System.out.println(e);
  }
}



	
