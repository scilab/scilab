
/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Clément DAVID
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.xcos;

import java.beans.PropertyChangeEvent;
import java.beans.PropertyChangeListener;
import java.net.MalformedURLException;
import java.net.URL;
import java.util.List;

import javax.swing.ImageIcon;

import org.apache.commons.logging.LogFactory;
import org.scilab.modules.graph.ScilabGraph;
import org.scilab.modules.graph.actions.CopyAction;
import org.scilab.modules.graph.actions.CutAction;
import org.scilab.modules.graph.actions.DeleteAction;
import org.scilab.modules.graph.actions.InvertSelectionAction;
import org.scilab.modules.graph.actions.PasteAction;
import org.scilab.modules.graph.actions.RedoAction;
import org.scilab.modules.graph.actions.SelectAllAction;
import org.scilab.modules.graph.actions.UndoAction;
import org.scilab.modules.graph.actions.ZoomInAction;
import org.scilab.modules.graph.actions.ZoomOutAction;
import org.scilab.modules.graph.actions.base.DefaultAction;
import org.scilab.modules.graph.event.ArrowKeyListener;
import org.scilab.modules.gui.bridge.menu.SwingScilabMenu;
import org.scilab.modules.gui.bridge.tab.SwingScilabTab;
import org.scilab.modules.gui.checkboxmenuitem.CheckBoxMenuItem;
import org.scilab.modules.gui.menu.Menu;
import org.scilab.modules.gui.menu.ScilabMenu;
import org.scilab.modules.gui.menubar.MenuBar;
import org.scilab.modules.gui.menubar.ScilabMenuBar;
import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.gui.pushbutton.PushButton;
import org.scilab.modules.gui.tab.ScilabTab;
import org.scilab.modules.gui.tab.Tab;
import org.scilab.modules.gui.textbox.ScilabTextBox;
import org.scilab.modules.gui.toolbar.ScilabToolBar;
import org.scilab.modules.gui.toolbar.ToolBar;
import org.scilab.modules.gui.utils.BarUpdater;
import org.scilab.modules.gui.utils.Position;
import org.scilab.modules.gui.utils.Size;
import org.scilab.modules.gui.window.ScilabWindow;
import org.scilab.modules.gui.window.Window;
import org.scilab.modules.xcos.actions.AboutXcosAction;
import org.scilab.modules.xcos.actions.CloseAction;
import org.scilab.modules.xcos.actions.CloseViewportAction;
import org.scilab.modules.xcos.actions.CompileAction;
import org.scilab.modules.xcos.actions.DebugLevelAction;
import org.scilab.modules.xcos.actions.DiagramBackgroundAction;
import org.scilab.modules.xcos.actions.ExportAction;
import org.scilab.modules.xcos.actions.FitDiagramToViewAction;
import org.scilab.modules.xcos.actions.InitModelicaAction;
import org.scilab.modules.xcos.actions.NewDiagramAction;
import org.scilab.modules.xcos.actions.NormalViewAction;
import org.scilab.modules.xcos.actions.OpenAction;
import org.scilab.modules.xcos.actions.PrintAction;
import org.scilab.modules.xcos.actions.QuitAction;
import org.scilab.modules.xcos.actions.RecentFileAction;
import org.scilab.modules.xcos.actions.SaveAction;
import org.scilab.modules.xcos.actions.SaveAsAction;
import org.scilab.modules.xcos.actions.SetContextAction;
import org.scilab.modules.xcos.actions.SetupAction;
import org.scilab.modules.xcos.actions.ShowHideShadowAction;
import org.scilab.modules.xcos.actions.StartAction;
import org.scilab.modules.xcos.actions.StopAction;
import org.scilab.modules.xcos.actions.ViewDiagramBrowserAction;
import org.scilab.modules.xcos.actions.ViewGridAction;
import org.scilab.modules.xcos.actions.ViewViewportAction;
import org.scilab.modules.xcos.actions.XcosDemonstrationsAction;
import org.scilab.modules.xcos.actions.XcosDocumentationAction;
import org.scilab.modules.xcos.block.actions.BlockDocumentationAction;
import org.scilab.modules.xcos.block.actions.BlockParametersAction;
import org.scilab.modules.xcos.block.actions.BorderColorAction;
import org.scilab.modules.xcos.block.actions.CodeGenerationAction;
import org.scilab.modules.xcos.block.actions.FilledColorAction;
import org.scilab.modules.xcos.block.actions.FlipAction;
import org.scilab.modules.xcos.block.actions.MirrorAction;
import org.scilab.modules.xcos.block.actions.RegionToSuperblockAction;
import org.scilab.modules.xcos.block.actions.RotateAction;
import org.scilab.modules.xcos.block.actions.ViewDetailsAction;
import org.scilab.modules.xcos.block.actions.alignement.AlignBlockActionBottom;
import org.scilab.modules.xcos.block.actions.alignement.AlignBlockActionCenter;
import org.scilab.modules.xcos.block.actions.alignement.AlignBlockActionLeft;
import org.scilab.modules.xcos.block.actions.alignement.AlignBlockActionMiddle;
import org.scilab.modules.xcos.block.actions.alignement.AlignBlockActionRight;
import org.scilab.modules.xcos.block.actions.alignement.AlignBlockActionTop;
import org.scilab.modules.xcos.configuration.ConfigurationManager;
import org.scilab.modules.xcos.configuration.model.DocumentType;
import org.scilab.modules.xcos.configuration.model.PositionType;
import org.scilab.modules.xcos.configuration.utils.ConfigurationConstants;
import org.scilab.modules.xcos.graph.XcosDiagram;
import org.scilab.modules.xcos.link.actions.StyleHorizontalAction;
import org.scilab.modules.xcos.link.actions.StyleStraightAction;
import org.scilab.modules.xcos.link.actions.StyleVerticalAction;
import org.scilab.modules.xcos.palette.PaletteManager;
import org.scilab.modules.xcos.palette.actions.ViewPaletteBrowserAction;
import org.scilab.modules.xcos.utils.XcosMessages;

import com.mxgraph.swing.mxGraphOutline;

/**
 * Xcos tab operations
 * 
 * This class implement specific operation of an Xcos Tab.
 */
public class XcosTab extends ScilabTab {

	static {
		DefaultAction.addIconPath(System.getenv("SCI")
				+ "/modules/xcos/images/icons/");
	}

	/*
	 * Instance fields
	 */
	private MenuBar menuBar;
	private Menu fileMenu;
	private Menu recentsMenu;
	private Menu edit;
	private Menu view;
	private Menu simulate;
	private Menu format;
	private Menu alignMenu;
	private Menu linkStyle;
	private Menu tools;
	private Menu help;

	private PushButton openAction;
	private PushButton saveAction;
	private PushButton saveAsAction;
	private PushButton printAction;
	private PushButton newDiagramAction;
	private PushButton deleteAction;
	private PushButton undoAction;
	private PushButton redoAction;
	private PushButton fitDiagramToViewAction;
	private PushButton startAction;
	private PushButton stopAction;
	private PushButton zoomInAction;
	private PushButton zoomOutAction;
	private PushButton xcosDemonstrationAction;
	private PushButton xcosDocumentationAction;

	/**
	 * Default constructor
	 * 
	 * @param diagram
	 *            The associated diagram
	 */
	public XcosTab(final XcosDiagram diagram) {
		super(XcosMessages.XCOS);

		/** tab association */
		diagram.setParentTab(this);

		initComponents(diagram);

		((SwingScilabTab) getAsSimpleTab())
				.setWindowIcon(new ImageIcon(
						System.getenv("SCI")
								+ "/modules/gui/images/icons/32x32/apps/utilities-system-monitor.png")
						.getImage());

		// No SimpleTab.addMember(ScilabComponent ...) so perform a raw
		// association.
		((SwingScilabTab) getAsSimpleTab()).setContentPane(diagram
				.getAsComponent());

		// Get the palette window position and align on it.
		if (PaletteManager.isVisible()) {
			final Window win = PaletteManager.getInstance().getView()
					.getParentWindow();
			final Position palPosition = win.getPosition();
			final Size palSize = win.getDims();
			final Position mainPosition = new Position(palPosition.getX()
					+ palSize.getWidth(), palPosition.getY());
			getParentWindow().setPosition(mainPosition);
		}

		/*
		 * VIEW PORT
		 */
		XcosTab.createViewPort(diagram);

		setCallback(new CloseAction(diagram));
		diagram.getAsComponent().addKeyListener(new ArrowKeyListener());
		diagram.updateTabTitle();
	}

	/**
	 * Instantiate all the subcomponents of this Tab.
	 * 
	 * @param diagram
	 *            the diagram
	 */
	private void initComponents(final XcosDiagram diagram) {
		final Window window = ScilabWindow.createWindow();

		final ConfigurationManager manager = ConfigurationManager.getInstance();
		final PositionType p = manager.getSettings().getWindows().getDiagram();

		window.setDims(new Size(p.getWidth(), p.getHeight()));
		window.setPosition(new Position(p.getX(), p.getY()));

		/* Create the menu bar */
		menuBar = createMenuBar(diagram);
		addMenuBar(menuBar);

		/* Create the toolbar */
		final ToolBar toolBar = createToolBar(diagram);
		addToolBar(toolBar);

		/* Create the infoBar */
		addInfoBar(ScilabTextBox.createTextBox());

		window.addTab(this);
		BarUpdater.updateBars(getParentWindowId(), getMenuBar(), getToolBar(),
				getInfoBar(), getName());
	}

	/**
	 * Create the windows menu bar
	 * 
	 * @param diagram
	 *            the diagram
	 * @return the Xcos diagram menu bar
	 */
	private MenuBar createMenuBar(final XcosDiagram diagram) {

		menuBar = ScilabMenuBar.createMenuBar();

		/** FILE MENU */
		fileMenu = ScilabMenu.createMenu();
		fileMenu.setText(XcosMessages.FILE);
		fileMenu.setMnemonic('F');

		fileMenu.add(NewDiagramAction.createMenu(diagram));

		fileMenu.add(OpenAction.createMenu(diagram));
		fileMenu.addSeparator();
		fileMenu.add(SaveAction.createMenu(diagram));
		fileMenu.add(SaveAsAction.createMenu(diagram));
		fileMenu.add(ExportAction.createMenu(diagram));

		recentsMenu = createRecentMenu();

		fileMenu.add(recentsMenu);

		fileMenu.add(PrintAction.createMenu(diagram));
		fileMenu.addSeparator();
		fileMenu.add(CloseAction.createMenu(diagram));
		fileMenu.addSeparator();
		fileMenu.add(QuitAction.createMenu(diagram));

		menuBar.add(fileMenu);

		/** Edit menu */
		edit = ScilabMenu.createMenu();
		edit.setText(XcosMessages.EDIT);
		edit.setMnemonic('E');
		menuBar.add(edit);

		edit.add(UndoAction.undoMenu(diagram));
		edit.add(RedoAction.redoMenu(diagram));
		edit.addSeparator();
		edit.add(CutAction.cutMenu(diagram));
		edit.add(CopyAction.copyMenu(diagram));
		edit.add(PasteAction.pasteMenu(diagram));
		edit.add(DeleteAction.createMenu(diagram));
		edit.addSeparator();
		edit.add(SelectAllAction.createMenu(diagram));
		edit.add(InvertSelectionAction.createMenu(diagram));
		edit.addSeparator();
		edit.add(BlockParametersAction.createMenu(diagram));
		edit.addSeparator();
		edit.add(RegionToSuperblockAction.createMenu(diagram));

		/** View menu */
		view = ScilabMenu.createMenu();
		view.setText(XcosMessages.VIEW);
		view.setMnemonic('V');
		menuBar.add(view);

		view.add(ZoomInAction.zoominMenu(diagram));
		view.add(ZoomOutAction.zoomoutMenu(diagram));
		view.add(FitDiagramToViewAction.createMenu(diagram));
		view.add(NormalViewAction.createMenu(diagram));
		view.addSeparator();
		view.add(ViewPaletteBrowserAction.createCheckBoxMenu(diagram));
		view.add(ViewDiagramBrowserAction.createMenu(diagram));
		final CheckBoxMenuItem menu = ViewViewportAction
				.createCheckBoxMenu(diagram);
		view.add(menu);
		(diagram).setViewPortMenuItem(menu);
		view.add(ViewDetailsAction.createMenu(diagram));

		/** Simulation menu */
		simulate = ScilabMenu.createMenu();
		simulate.setText(XcosMessages.SIMULATION);
		simulate.setMnemonic('S');
		menuBar.add(simulate);

		final MenuItem startMenu = StartAction.createMenu(diagram);
		final MenuItem stopMenu = StopAction.createMenu(diagram);

		simulate.add(SetupAction.createMenu(diagram));
		simulate.add(DebugLevelAction.createMenu(diagram));
		simulate.add(SetContextAction.createMenu(diagram));
		simulate.add(CompileAction.createMenu(diagram));
		simulate.add(InitModelicaAction.createMenu(diagram));
		simulate.add(startMenu);
		simulate.add(stopMenu);

		/** Format menu */
		format = ScilabMenu.createMenu();
		format.setText(XcosMessages.FORMAT);
		format.setMnemonic('O');
		menuBar.add(format);

		format.add(RotateAction.createMenu(diagram));
		format.add(FlipAction.createMenu(diagram));
		format.add(MirrorAction.createMenu(diagram));
		format.add(ShowHideShadowAction.createMenu(diagram));

		format.addSeparator();
		alignMenu = ScilabMenu.createMenu();
		alignMenu.setText(XcosMessages.ALIGN_BLOCKS);
		alignMenu.add(AlignBlockActionLeft.createMenu(diagram));
		alignMenu.add(AlignBlockActionCenter.createMenu(diagram));
		alignMenu.add(AlignBlockActionRight.createMenu(diagram));
		alignMenu.addSeparator();
		alignMenu.add(AlignBlockActionTop.createMenu(diagram));
		alignMenu.add(AlignBlockActionMiddle.createMenu(diagram));
		alignMenu.add(AlignBlockActionBottom.createMenu(diagram));
		format.add(alignMenu);
		format.addSeparator();

		format.add(BorderColorAction.createMenu(diagram));
		format.add(FilledColorAction.createMenu(diagram));
		format.addSeparator();

		linkStyle = ScilabMenu.createMenu();
		linkStyle.setText(XcosMessages.LINK_STYLE);
		linkStyle.add(StyleHorizontalAction.createMenu(diagram));
		linkStyle.add(StyleStraightAction.createMenu(diagram));
		linkStyle.add(StyleVerticalAction.createMenu(diagram));
		format.add(linkStyle);
		format.addSeparator();

		format.add(DiagramBackgroundAction.createMenu(diagram));
		final CheckBoxMenuItem gridMenu = ViewGridAction
				.createCheckBoxMenu(diagram);
		format.add(gridMenu);

		/** Tools menu */
		tools = ScilabMenu.createMenu();
		tools.setText(XcosMessages.TOOLS);
		tools.setMnemonic('T');
		menuBar.add(tools);

		tools.add(CodeGenerationAction.createMenu(diagram));

		/** Help menu */
		help = ScilabMenu.createMenu();
		help.setText(XcosMessages.HELP);
		help.setMnemonic('?');
		menuBar.add(help);

		help.add(XcosDocumentationAction.createMenu(diagram));
		help.add(BlockDocumentationAction.createMenu(diagram));
		help.addSeparator();
		help.add(XcosDemonstrationsAction.createMenu(diagram));
		help.addSeparator();
		help.add(AboutXcosAction.createMenu(diagram));

		return menuBar;
	}

	/**
	 * Create the recent menu from the previously opened files
	 * @return the recent menu
	 */
	private Menu createRecentMenu() {
		final Menu recent;

		recent = ScilabMenu.createMenu();
		recent.setText(XcosMessages.RECENT_FILES);

		final ConfigurationManager manager = ConfigurationManager.getInstance();
		final List<DocumentType> recentFiles = manager.getSettings()
				.getRecentFiles().getDocument();
		for (int i = 0; i < recentFiles.size(); i++) {
			URL url;
			try {
				url = new URL(recentFiles.get(i).getUrl());
			} catch (final MalformedURLException e) {
				LogFactory.getLog(XcosTab.class).error(e);
				break;
			}
			recent.add(RecentFileAction.createMenu(url));
		}

		ConfigurationManager.getInstance().addPropertyChangeListener(
				ConfigurationConstants.RECENT_FILES_CHANGED,
				new PropertyChangeListener() {
					@Override
					public void propertyChange(final PropertyChangeEvent evt) {
						assert evt.getPropertyName().equals(
								ConfigurationConstants.RECENT_FILES_CHANGED);

						/*
						 * We only handle menu creation there. Return when this
						 * is not the case.
						 */
						if (evt.getOldValue() != null) {
							return;
						}

						URL url;
						try {
							url = new URL(((DocumentType) evt.getNewValue())
									.getUrl());
						} catch (final MalformedURLException e) {
							LogFactory.getLog(XcosTab.class).error(e);
							return;
						}

						((SwingScilabMenu) recent.getAsSimpleMenu()).add(
								(SwingScilabMenu) RecentFileAction.createMenu(
										url).getAsSimpleMenu(), 0);
					}
				});

		return recent;
	}

	/**
	 * Create the Tab toolbar
	 * 
	 * @param diagram
	 *            the associated diagram
	 * @return tool bar
	 */
	private ToolBar createToolBar(final XcosDiagram diagram) {
		final ToolBar toolBar = ScilabToolBar.createToolBar();

		newDiagramAction = NewDiagramAction.createButton(diagram);
		toolBar.add(newDiagramAction);

		openAction = OpenAction.createButton(diagram);
		toolBar.add(openAction);

		toolBar.addSeparator();

		saveAction = SaveAction.createButton(diagram);
		toolBar.add(saveAction);
		saveAsAction = SaveAsAction.createButton(diagram);
		toolBar.add(saveAsAction);

		toolBar.addSeparator();

		printAction = PrintAction.createButton(diagram);
		toolBar.add(printAction);

		toolBar.addSeparator();

		deleteAction = DeleteAction.createButton(diagram);
		toolBar.add(deleteAction);

		toolBar.addSeparator();

		// UNDO / REDO
		undoAction = UndoAction.undoButton(diagram);
		redoAction = RedoAction.redoButton(diagram);
		toolBar.add(undoAction);
		toolBar.add(redoAction);

		toolBar.addSeparator();

		fitDiagramToViewAction = FitDiagramToViewAction.createButton(diagram);
		toolBar.add(fitDiagramToViewAction);

		toolBar.addSeparator();

		// START / STOP
		startAction = StartAction.createButton(diagram);
		stopAction = StopAction.createButton(diagram);

		toolBar.add(startAction);
		toolBar.add(stopAction);

		toolBar.addSeparator();

		// ZOOMIN / ZOOMOUT
		zoomInAction = ZoomInAction.zoominButton(diagram);
		toolBar.add(zoomInAction);
		zoomOutAction = ZoomOutAction.zoomoutButton(diagram);
		toolBar.add(zoomOutAction);

		toolBar.addSeparator();

		xcosDemonstrationAction = XcosDemonstrationsAction
				.createButton(diagram);
		toolBar.add(xcosDemonstrationAction);
		xcosDocumentationAction = XcosDocumentationAction.createButton(diagram);
		toolBar.add(xcosDocumentationAction);

		return toolBar;
	}

	/**
	 * @param xcosDiagramm
	 *            diagram
	 */
	private static void createViewPort(final ScilabGraph xcosDiagramm) {
		final Window outline = ScilabWindow.createWindow();
		final Tab outlineTab = ScilabTab.createTab(XcosMessages.VIEWPORT);

		outlineTab.setCallback(new CloseViewportAction(xcosDiagramm));

		final MenuBar vpMenuBar = ScilabMenuBar.createMenuBar();
		outlineTab.addMenuBar(vpMenuBar);

		final Menu vpMenu = ScilabMenu.createMenu();
		vpMenu.setText(XcosMessages.VIEWPORT);
		vpMenu.setMnemonic('V');
		vpMenuBar.add(vpMenu);

		vpMenu.add(CloseViewportAction.createMenu(xcosDiagramm));

		outlineTab.getAsSimpleTab().setInfoBar(ScilabTextBox.createTextBox());

		((XcosDiagram) xcosDiagramm).setViewPort(outlineTab);

		// Creates the graph outline component
		final mxGraphOutline graphOutline = new mxGraphOutline(xcosDiagramm
				.getAsComponent());

		graphOutline.setDrawLabels(true);

		((SwingScilabTab) outlineTab.getAsSimpleTab())
				.setContentPane(graphOutline);
		outline.addTab(outlineTab);
		outline.setVisible(false);
		outlineTab.setVisible(false);
	}

	/**
	 * Set the current tab and the associated window visible when a unique Tab
	 * is docked.
	 * 
	 * @param newVisibleState
	 * @see org.scilab.modules.gui.tab.ScilabTab#setVisible(boolean)
	 */
	@Override
	public void setVisible(final boolean newVisibleState) {
		if (getParentWindow().getNbDockedObjects() == 1) {
			getParentWindow().setVisible(newVisibleState);
		}

		super.setVisible(newVisibleState);
	}
}
