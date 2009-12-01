package org.scilab.modules.xcos.actions;

import java.awt.Toolkit;
import java.awt.event.KeyEvent;

import javax.swing.KeyStroke;

import org.scilab.modules.graph.ScilabGraph;
import org.scilab.modules.graph.actions.DefaultAction;
import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.xcos.XcosDiagram;
import org.scilab.modules.xcos.block.BasicBlock;
import org.scilab.modules.xcos.utils.XcosMessages;

public class MirrorAction extends DefaultAction {

    private static final long serialVersionUID = -6228416497044111945L;

    /**
     * Constructor
     * @param scilabGraph associated diagram
     */
    public MirrorAction(ScilabGraph scilabGraph) {
	super(XcosMessages.MIRROR, scilabGraph);
    }

    /**
     * Menu added to the menubar
     * @param scilabGraph associated diagram
     * @return the menu
     */
    public static MenuItem createMenu(ScilabGraph scilabGraph) {
	return createMenu(XcosMessages.MIRROR, null, new MirrorAction(scilabGraph),
		KeyStroke.getKeyStroke(KeyEvent.VK_M, Toolkit.getDefaultToolkit().getMenuShortcutKeyMask()));
    }

    /**
     * Action !!
     * @see org.scilab.modules.graph.actions.DefaultAction#doAction()
     */
    public void doAction() {
	if (((XcosDiagram) getGraph(null)).getSelectionCells().length != 0) {

	    Object[] allCells = ((XcosDiagram) getGraph(null)).getSelectionCells();

	    for (int i = 0 ; i < allCells.length ; ++i) {
		if (allCells[i] instanceof BasicBlock) {
		    ((BasicBlock) allCells[i]).toggleMirror();
		}
	    }
	}
    }

}