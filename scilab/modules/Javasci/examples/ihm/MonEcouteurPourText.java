import java.awt.event.*;

public class MonEcouteurPourText implements TextListener
{
  private MonPanneau appli;
  public MonEcouteurPourText(MonPanneau qui)
  {
    appli=qui;	
  }
  public void textValueChanged(TextEvent e)
  {
    appli.recupTexte();	
    System.out.println(e);
  }
}
