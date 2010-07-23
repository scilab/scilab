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

package org.scilab.modules.xcos.palette;

import java.awt.image.BufferedImage;
import java.io.File;
import java.io.IOException;
import java.lang.reflect.InvocationTargetException;
import java.util.Iterator;
import java.util.List;

import javax.imageio.ImageIO;
import javax.swing.SwingUtilities;
import javax.swing.tree.DefaultTreeModel;

import ncsa.hdf.hdf5lib.exceptions.HDF5Exception;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.scilab.modules.graph.utils.ScilabExported;
import org.scilab.modules.hdf5.read.H5Read;
import org.scilab.modules.jvm.utils.ScilabConstants;
import org.scilab.modules.localization.Messages;
import org.scilab.modules.types.scilabTypes.ScilabTList;
import org.scilab.modules.xcos.block.BasicBlock;
import org.scilab.modules.xcos.graph.XcosDiagram;
import org.scilab.modules.xcos.io.scicos.H5RWHandler;
import org.scilab.modules.xcos.io.scicos.ScicosFormatException;
import org.scilab.modules.xcos.palette.model.Category;
import org.scilab.modules.xcos.palette.model.PaletteBlock;
import org.scilab.modules.xcos.palette.model.PaletteNode;
import org.scilab.modules.xcos.palette.model.PreLoaded;
import org.scilab.modules.xcos.utils.BlockPositioning;
import org.scilab.modules.xcos.utils.FileUtils;
import org.scilab.modules.xcos.utils.XcosConstants;

import com.mxgraph.io.mxCodec;
import com.mxgraph.swing.mxGraphComponent;
import com.mxgraph.util.mxCellRenderer;
import com.mxgraph.util.mxRectangle;
import com.mxgraph.util.mxUtils;
import com.mxgraph.view.mxStylesheet;

/**
 * Utility class which is the entry point from Scilab for palette related
 * functions.
 */
public final class Palette {
	private static final String NAME = "name";

	private static final String WRONG_INPUT_ARGUMENT_S_INVALID_TREE_PATH = Messages.gettext("Wrong input argument \"%s\": invalid tree path.\n");
	private static final String WRONG_INPUT_ARGUMENT_S_INVALID_NODE = Messages.gettext("Wrong input argument \"%s\": invalid node, use 'xcosPalDisable' instead.\n");
	private static final String UNABLE_TO_IMPORT = Messages.gettext("Unable to import %s .\n");

	private static final Log LOG = LogFactory.getLog(Palette.class);

	/**
	 * Default hidden constructor
	 */
	private Palette() {
	}

	/**
	 * Get the {@link PaletteNode} of the path.
	 * 
	 * @param path
	 *            the path
	 * @param create
	 *            if <code>true</code> the Category path will be created,
	 *            otherwise it will not.
	 * @return the selected node
	 */
	private static PaletteNode getPathNode(final String[] path,
			final boolean create) {

		if (!SwingUtilities.isEventDispatchThread()) {
			throw new RuntimeException(
					"Unable to manipulate palette outside the EDT thread.");
		}

		Category node = PaletteManager.getInstance().getRoot();

		if (path == null) {
			return node;
		}

		for (int categoryCounter = 0; categoryCounter < path.length; categoryCounter++) {

			for (Iterator<PaletteNode> iter = node.getNode().iterator(); iter
					.hasNext();) {
				PaletteNode next = iter.next();

				if (next.toString().equals(path[categoryCounter])
						&& next instanceof Category) {
					node = (Category) next;
					break;
				} else if (next.toString().equals(path[categoryCounter])
						&& (categoryCounter == path.length - 1)) {
					return next; // found the terminal Palette instance
				}
			}

			if (!node.toString().equals(path[categoryCounter])) {
				if (create) {
					Category cat = new Category();
					cat.setName(path[categoryCounter]);
					cat.setEnable(create);

					cat.setParent(node);
					node.getNode().add(cat);

					node = cat;
				} else {
					return null;
				}
			}
		}
		return node;
	}

	/**
	 * Load an xcos palette into the palette manager
	 * 
	 * @param path
	 *            full path to the scilab exported palette
	 * @param category
	 *            TreePath of the palette
	 * @throws Exception
	 *             in case of error
	 */
	@ScilabExported(module = "xcos", filename = "Palette.giws.xml")
	public static void loadPal(final String path, final String[] category)
			throws Exception {
		/*
		 * Import the palette
		 */
		final ScilabTList data = new ScilabTList();
		final String file = new File(path).getAbsolutePath();
		try {
			int fileId = H5Read.openFile(file);
			H5Read.readDataFromFile(fileId, data);
			H5Read.closeFile(fileId);
		} catch (HDF5Exception e) {
			throw new Exception(String.format(UNABLE_TO_IMPORT, file), e);
		}

		/*
		 * handle shared data on the EDT thread
		 */
		SwingUtilities.invokeAndWait(new Runnable() {
			@Override
			public void run() {
				/*
				 * Decode the style part of the palette
				 */
				mxStylesheet styleSheet = new mxStylesheet();
				try {
					FileUtils.decodeStyle(styleSheet);
					new StyleElement().decode(data, styleSheet);
					mxUtils.writeFile(mxUtils.getXml(new mxCodec()
							.encode(styleSheet)), ScilabConstants.SCIHOME
							.getAbsolutePath()
							+ '/' + FileUtils.STYLE_FILENAME);
				} catch (IOException e) {
					throw new RuntimeException(e);
				} catch (ScicosFormatException e) {
					throw new RuntimeException(e);
				}

				PaletteNode node = getPathNode(category, true);
				if (!(node instanceof Category)) {
					throw new RuntimeException(String.format(
							WRONG_INPUT_ARGUMENT_S_INVALID_TREE_PATH,
							"category"));
				}
				Category cat = (Category) node;

				/*
				 * Adding the palette tree part of the palette
				 */
				try {
					cat.getNode()
							.add(new PreLoadedElement().decode(data, null));
				} catch (ScicosFormatException e) {
					throw new RuntimeException(e);
				}

				PaletteManager.getInstance().saveConfig();
				if (PaletteManager.getInstance().getView() != null) {
					final DefaultTreeModel model = (DefaultTreeModel) PaletteManager
							.getInstance().getView().getTree().getModel();
					model.reload(cat);
				}
			}
		});
	}

	/**
	 * Load an xcos palette into the palette manager at the root category.
	 * 
	 * @param path
	 *            full path to the scilab exported palette
	 * @throws Exception
	 *             in case of error
	 */
	@ScilabExported(module = "xcos", filename = "Palette.giws.xml")
	public static void loadPal(final String path) throws Exception {
		loadPal(path, null);
	}

	/**
	 * Add a category into the palette manager
	 * 
	 * @param name
	 *            TreePath of the palette
	 * @param visible
	 *            default visibility of the palette
	 * @throws Exception
	 *             in case of error
	 */
	@ScilabExported(module = "xcos", filename = "Palette.giws.xml")
	public static void addCategory(final String[] name, final boolean visible)
			throws Exception {
		try {
			SwingUtilities.invokeAndWait(new Runnable() {
				@Override
				public void run() {
					PaletteNode node = getPathNode(name, true);
					if (node instanceof Category) {
						node.setEnable(visible);
					} else {
						throw new RuntimeException(String.format(
								WRONG_INPUT_ARGUMENT_S_INVALID_TREE_PATH, NAME));
					}

					PaletteManager.getInstance().saveConfig();
					if (PaletteManager.getInstance().getView() != null) {
						final DefaultTreeModel model = (DefaultTreeModel) PaletteManager
								.getInstance().getView().getTree().getModel();
						model.reload(node);
					}
				}
			});
		} catch (InvocationTargetException exception) {
			throw (Exception) exception.getTargetException();
		}
	}

	/**
	 * Remove a palette or a category of the palette manager
	 * 
	 * @param name
	 *            TreePath of the palette
	 * @throws Exception
	 *             in case of error
	 */
	@ScilabExported(module = "xcos", filename = "Palette.giws.xml")
	public static void remove(final String[] name) throws Exception {
		try {
			SwingUtilities.invokeAndWait(new Runnable() {
				@Override
				public void run() {
					PaletteNode node = getPathNode(name, false);
					checkRemoving(node);
					remove(node);
					PaletteManager.getInstance().saveConfig();
				}
			});
		} catch (InvocationTargetException exception) {
			throw (Exception) exception.getTargetException();
		}
	}

	/**
	 * Remove the dynamic {@link PaletteNode} palette
	 * @param node the palette
	 */
	private static void remove(PaletteNode node) {
		if (node == null) {
			throw new RuntimeException(String.format(
					WRONG_INPUT_ARGUMENT_S_INVALID_TREE_PATH, NAME));
		} else if (node instanceof PreLoaded) {
			throw new RuntimeException(String.format(
					WRONG_INPUT_ARGUMENT_S_INVALID_NODE, NAME));
		} else if (node instanceof Category) {
			// Iterate over all nodes
			for (PaletteNode n : ((Category) node).getNode()) {
				remove(n);
			}
		} else { // others
			final Category toBeReloaded = node.getParent();
			if (toBeReloaded != null) {
				toBeReloaded.getNode().remove(node);
			}
			node.setParent(null);

			if (PaletteManager.getInstance().getView() != null) {
				final DefaultTreeModel model = (DefaultTreeModel) PaletteManager
						.getInstance().getView().getTree().getModel();
				if (toBeReloaded != null) {
					model.reload(toBeReloaded);
				} else {
					model.reload();
				}
			}
		}
	}
	
	/**
	 * Check that the node can be removed (throw exceptions).
	 * 
	 * @param node the node to check
	 */
	private static void checkRemoving(PaletteNode node) {
		if (node == null) {
			throw new RuntimeException(String.format(
					WRONG_INPUT_ARGUMENT_S_INVALID_TREE_PATH, NAME));
		} else if (node instanceof PreLoaded) {
			throw new RuntimeException(String.format(
					WRONG_INPUT_ARGUMENT_S_INVALID_NODE, NAME));
		} else if (node instanceof Category) {
			// Iterate over all nodes
			for (PaletteNode n : ((Category) node).getNode()) {
				checkRemoving(n);
			}
		}
		
		/*
		 * others can be removed safely.
		 */
	}
	
	/**
	 * Remove a palette or a category of the palette manager
	 * 
	 * @param name
	 *            TreePath of the palette or category
	 * @param status
	 *            True to set the palette visible, false otherwise
	 * @throws Exception
	 *             in case of error
	 */
	@ScilabExported(module = "xcos", filename = "Palette.giws.xml")
	public static void enable(final String[] name, final boolean status)
			throws Exception {
		try {
			SwingUtilities.invokeAndWait(new Runnable() {
				@Override
				public void run() {
					PaletteNode node = getPathNode(name, false);
					if (node == null) {
						throw new RuntimeException(String.format(
								WRONG_INPUT_ARGUMENT_S_INVALID_TREE_PATH, NAME));
					}

					node.setEnable(status);

					PaletteManager.getInstance().saveConfig();
					if (PaletteManager.getInstance().getView() != null) {
						final DefaultTreeModel model = (DefaultTreeModel) PaletteManager
								.getInstance().getView().getTree().getModel();
						model.reload(node);
					}
				}
			});
		} catch (InvocationTargetException exception) {
			throw (Exception) exception.getTargetException();
		}
	}

	/**
	 * Move a palette or a category of the palette manager
	 * 
	 * @param source
	 *            TreePath of the palette or category
	 * @param target
	 *            TreePath of the category
	 * @link TreePath} of the destination
	 * @throws Exception
	 *             in case of error
	 */
	@ScilabExported(module = "xcos", filename = "Palette.giws.xml")
	public static void move(final String[] source, final String[] target)
			throws Exception {
		try {
			SwingUtilities.invokeAndWait(new Runnable() {
				@Override
				public void run() {

					PaletteNode src = getPathNode(source, false);
					if (src == null) {
						throw new RuntimeException(String.format(
								WRONG_INPUT_ARGUMENT_S_INVALID_TREE_PATH,
								"source"));
					}

					PaletteNode trg = getPathNode(target, true);
					if (trg == null || !(trg instanceof Category)) {
						throw new RuntimeException(String.format(
								WRONG_INPUT_ARGUMENT_S_INVALID_TREE_PATH,
								"target"));
					}
					Category destination = (Category) trg;

					final Category[] toBeReloaded = new Category[] {
							src.getParent(), destination };

					if (toBeReloaded[0] != null) {
						toBeReloaded[0].getNode().remove(src);
					}
					destination.getNode().add(src);
					src.setParent(destination);

					PaletteManager.getInstance().saveConfig();
					if (PaletteManager.getInstance().getView() != null) {
						final DefaultTreeModel model = (DefaultTreeModel) PaletteManager
								.getInstance().getView().getTree().getModel();
						if (toBeReloaded[0] != null) {
							model.reload(toBeReloaded[0]);
							model.reload(toBeReloaded[1]);
						} else {
							model.reload();
						}
					}
				}
			});
		} catch (InvocationTargetException exception) {
			throw (Exception) exception.getTargetException();
		}
	}

	/**
	 * Generate a palette block image from a block saved instance (need a valid
	 * style).
	 * 
	 * @param blockPath
	 *            the HDF5 block instance
	 * @param iconPath
	 *            the output file path use to save the palette block.
	 * @throws IOException
	 *             in case of write errors
	 */
	@ScilabExported(module = "xcos", filename = "Palette.giws.xml")
	public static void generatePaletteIcon(String blockPath, String iconPath)
			throws IOException {
		final BasicBlock block = new H5RWHandler(blockPath).readBlock();

		if (block == null || block.getGeometry() == null) {
			return;
		}
		block.getGeometry().setX(XcosConstants.PALETTE_BLOCK_WIDTH);
		block.getGeometry().setY(XcosConstants.PALETTE_BLOCK_HEIGHT);

		XcosDiagram graph = new XcosDiagram();
		graph.installListeners();
		graph.getModel().beginUpdate();
		graph.addCell(block);
		BlockPositioning.updateBlockView(block);
		graph.getModel().endUpdate();

		/*
		 * Render
		 */
		mxGraphComponent graphComponent = graph.getAsComponent();
		graphComponent.refresh();

		final mxRectangle bounds = graph.getPaintBounds(new Object[] {block});
		final double width = bounds.getWidth();
		final double height = bounds.getHeight();

		final double scale;
		if (width > XcosConstants.PALETTE_BLOCK_WIDTH
				|| height > XcosConstants.PALETTE_BLOCK_HEIGHT) {
			scale = Math.min(XcosConstants.PALETTE_BLOCK_WIDTH / width,
					XcosConstants.PALETTE_BLOCK_HEIGHT / height)
					/ XcosConstants.PALETTE_BLOCK_ICON_RATIO;
		} else {
			scale = 1.0;
		}

		BufferedImage image = mxCellRenderer.createBufferedImage(graph, null,
				scale, null, graphComponent.isAntiAlias(), null, graphComponent
						.getCanvas());

		final String extension = iconPath
				.substring(iconPath.lastIndexOf('.') + 1);
		ImageIO.write(image, extension, new File(iconPath));

		if (LOG.isTraceEnabled()) {
			LOG.trace(iconPath + " updated.");
		}
	}

	/**
	 * Helper function used to regenerate palette block images from block
	 * instance.
	 * 
	 * This method is not export to scilab but is needed for internal purpose
	 * (palette block image generation)
	 */
	public static void generateAllPaletteImages() {
		try {
			SwingUtilities.invokeAndWait(new Runnable() {
				public void run() {
					final PaletteManager current = PaletteManager.getInstance();
					iterate(current.getRoot().getNode());
					current.saveConfig();
				}

				private void iterate(List<PaletteNode> children) {
					for (Iterator<PaletteNode> iter = children.iterator(); iter
							.hasNext();) {
						PaletteNode next = iter.next();

						if (next instanceof Category) {
							iterate(((Category) next).getNode());
						} else if (next instanceof PreLoaded) {
							final PreLoaded current = (PreLoaded) next;

							List<PaletteBlock> blocks = current.getBlock();
							for (Iterator<PaletteBlock> iterator = blocks
									.iterator(); iterator.hasNext();) {
								final PaletteBlock paletteBlock = iterator
										.next();
								final String file = paletteBlock.getIcon()
										.getEvaluatedPath();

								try {
									generatePaletteIcon(paletteBlock.getData()
											.getEvaluatedPath(), paletteBlock
											.getIcon().getEvaluatedPath());
								} catch (IOException e) {
									LOG.error(file);
									LOG.error(e);
								}
							}
						}
					}
				}
			});
		} catch (InterruptedException e) {
			LOG.error(e);
		} catch (InvocationTargetException e) {
			LOG.error(e);
		}

		LOG.trace("All images has been generated.");
	}
}
