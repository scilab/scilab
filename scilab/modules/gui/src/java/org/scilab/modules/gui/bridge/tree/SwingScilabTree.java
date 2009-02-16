package org.scilab.modules.gui.bridge.tree;

import java.awt.BorderLayout;
import java.awt.Component;
import java.awt.event.MouseListener;
import java.util.ArrayList;
import java.util.Vector;

import javax.swing.JFrame;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.JTree;
import javax.swing.tree.DefaultMutableTreeNode;
import javax.swing.tree.DefaultTreeModel;
import javax.swing.tree.TreeNode;

import org.scilab.modules.gui.events.callback.CallBack;
import org.scilab.modules.gui.tree.ScilabTree;
import org.scilab.modules.gui.tree.SimpleTree;
import org.scilab.modules.gui.tree.Tree;
import org.scilab.modules.gui.utils.Position;
import org.scilab.modules.gui.utils.PositionConverter;
import org.scilab.modules.gui.utils.Size;


public class SwingScilabTree extends DefaultMutableTreeNode implements SimpleTree {

	private static final long serialVersionUID = 1L;
	
	private CallBack callback;
	
	/**
	 * Constructor
	 */
	public SwingScilabTree() {
	
	}
	
	public SwingScilabTree(Tree tree) {
		super(tree.getLabel());
		Vector<Tree> children = tree.getChildren();
		for (int i = 0 ; i < children.size() ; ++i)
		{
			SwingScilabTree child = new SwingScilabTree(children.get(i));
			this.add(child);
		}
	}
	
	public static void  showTree(Tree tree) {
		
		JTree root = new JTree();	
			
		SwingScilabTree main = new SwingScilabTree(tree);
		DefaultTreeModel model = new DefaultTreeModel(main);
		root.setModel(model);
		
		JFrame frame = new JFrame();
		frame.setTitle( "Tree example" );
		frame.setSize( 300, 200 );
		frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		
		JPanel panel = new JPanel();
		panel.setLayout(new BorderLayout()) ;
		frame.add(panel);
		
		// Add the listbox to a scrolling pane
		JScrollPane scrollPane = new JScrollPane();
		scrollPane.getViewport().add(root);
		//panel.add(scrollPane, BorderLayout.CENTER);
		panel.add(root);
		
		frame.setVisible(true);
		
		
	}

}
