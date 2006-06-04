import org.eclipse.swt.SWT;
import org.eclipse.swt.events.FocusEvent;
import org.eclipse.swt.events.KeyEvent;
import org.eclipse.swt.events.KeyListener;
import org.eclipse.swt.events.FocusListener;
import org.eclipse.swt.graphics.Image;
import org.eclipse.swt.widgets.*;


class SciGUIConsole
{  

	
  public static void main(String[] args) 
  {
 	 Display d;
   Shell s;

  	
    d = new Display( );
    s = new Shell(d);
    s.setSize(250,250);
    s.setText("Scilab 5.0");
    final Text text1 = new Text(s, SWT.MULTI | SWT.V_SCROLL |   SWT.H_SCROLL | SWT.WRAP | SWT.BORDER);
    text1.setBounds(10,10,200,200);

//
//	text1.addKeyListener(new KeyListener() {
//      String selectedItem = "";
//
//      public void keyPressed(KeyEvent e) 
//      {
//      }
//    });	

    

    s.open( );
    while(!s.isDisposed( ))
    {
     if(!d.readAndDispatch( )) d.sleep( );
    }
    d.dispose( );
  }
}