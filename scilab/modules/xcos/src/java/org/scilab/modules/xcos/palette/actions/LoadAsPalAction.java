package org.scilab.modules.xcos.palette.actions;

import java.awt.Toolkit;
import java.awt.event.KeyEvent;

import javax.swing.JFileChooser;
import javax.swing.KeyStroke;

import org.scilab.modules.graph.ScilabGraph;
import org.scilab.modules.graph.actions.DefaultAction;
import org.scilab.modules.gui.bridge.filechooser.SwingScilabFileChooser;
import org.scilab.modules.gui.filechooser.FileChooser;
import org.scilab.modules.gui.filechooser.ScilabFileChooser;
import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.gui.pushbutton.PushButton;
import org.scilab.modules.gui.utils.SciFileFilter;
import org.scilab.modules.xcos.palette.XcosPaletteManager;
import org.scilab.modules.xcos.utils.XcosFileType;
import org.scilab.modules.xcos.utils.XcosMessages;

public class LoadAsPalAction extends DefaultAction {

    private static final long serialVersionUID = 6292720188130217522L;

    /**
     * Constructor
     * @param scilabGraph associated Scilab Graph
     */
    private LoadAsPalAction(ScilabGraph scilabGraph) {
	super(XcosMessages.OPEN, scilabGraph);
    }

    /**
     * Create a menu to add in Scilab Graph menu bar
     * @param scilabGraph associated Scilab Graph
     * @return the menu
     */
    public static MenuItem createMenu(ScilabGraph scilabGraph) {
	return createMenu(XcosMessages.LOAD_AS_PAL, null, new LoadAsPalAction(scilabGraph),
		KeyStroke.getKeyStroke(KeyEvent.VK_O, Toolkit.getDefaultToolkit().getMenuShortcutKeyMask()));
    }

    /**
     * Create a button to add in Scilab Graph tool bar
     * @param scilabGraph associated Scilab Graph
     * @return the button
     */
    public static PushButton createButton(ScilabGraph scilabGraph) {
	return createButton(XcosMessages.OPEN, "document-open.png", new LoadAsPalAction(scilabGraph));
    }

    /**
     * Open file action
     * @see org.scilab.modules.graph.actions.DefaultAction#doAction()
     */
    public void doAction() {
	SwingScilabFileChooser fc = ((SwingScilabFileChooser) ScilabFileChooser.createFileChooser().getAsSimpleFileChooser());

	/* Standard files */
	fc.setTitle(XcosMessages.OPEN);
	fc.setUiDialogType(JFileChooser.OPEN_DIALOG);
	fc.setMultipleSelection(false);


	SciFileFilter xcosFilter = new SciFileFilter("*.xcos", null, 0);
	SciFileFilter cosFilter = new SciFileFilter("*.cos*", null, 1);
	SciFileFilter allFilter = new SciFileFilter("*.*", null, 2);
	fc.addChoosableFileFilter(xcosFilter);
	fc.addChoosableFileFilter(cosFilter);
	fc.addChoosableFileFilter(allFilter);
	fc.setFileFilter(xcosFilter);

	fc.setAcceptAllFileFilterUsed(false);
	fc.displayAndWait();

	if (fc.getSelection() == null || fc.getSelection().length == 0 || fc.getSelection()[0].equals("")) {
	    return;
	}

	XcosPaletteManager.loadUserPalette(fc.getSelection()[0]);
    }
}