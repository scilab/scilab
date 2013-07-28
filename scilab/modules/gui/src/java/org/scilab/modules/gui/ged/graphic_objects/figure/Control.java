/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 2013 - Marcos CARDINOT
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
package org.scilab.modules.gui.ged.graphic_objects.figure;

import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.FocusAdapter;
import java.awt.event.FocusEvent;

import javax.swing.JComboBox;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JSeparator;
import javax.swing.JTextField;
import javax.swing.JToggleButton;

import org.scilab.modules.graphic_objects.graphicController.GraphicController;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties;
import org.scilab.modules.gui.ged.ContentLayout;

import org.scilab.modules.gui.ged.MessagesGED;
import org.scilab.modules.gui.ged.graphic_objects.SimpleSection;

/**
* Construction and startup of all components of the section: Control.
* @author Marcos CARDINOT <mcardinot@gmail.com>
*/
public class Control extends Figure implements SimpleSection {
    private String currentFigure;
    private ContentLayout layout = new ContentLayout();

    private static JToggleButton bControl;
    private JLabel lControl;
    private JSeparator sControl;
    private static JPanel pControl;
    private JToggleButton bAxesSize;
    private JLabel lAxesSize;
    private JTextField cAxesSize;
    private JPanel pAxesSize;
    private JLabel lAxesSizeX;
    private JTextField cAxesSizeX;
    private JLabel lAxesSizeY;
    private JTextField cAxesSizeY;
    private JLabel lCloseRequestFcn;
    private JTextField cCloseRequestFcn;
    private JLabel lEventHandler;
    private JComboBox cEventHandler;
    private JToggleButton bFigurePosition;
    private JLabel lFigurePosition;
    private JTextField cFigurePosition;
    private JPanel pFigurePosition;
    private JLabel lFigurePositionX;
    private JTextField cFigurePositionX;
    private JLabel lFigurePositionY;
    private JTextField cFigurePositionY;
    private JToggleButton bFigureSize;
    private JLabel lFigureSize;
    private JTextField cFigureSize;
    private JPanel pFigureSize;
    private JLabel lFigureSizeX;
    private JTextField cFigureSizeX;
    private JLabel lFigureSizeY;
    private JTextField cFigureSizeY;
    private JLabel lResizeFcn;
    private JTextField cResizeFcn;
    private JToggleButton bViewport;
    private JLabel lViewport;
    private JTextField cViewport;
    private JLabel lViewportX;
    private JTextField cViewportX;
    private JLabel lViewportY;
    private JTextField cViewportY;
    private JPanel pViewport;

    /**
    * Initializes the properties and the icons of the buttons.
    * @param objectID Enters the identification of figure.
    */
    public Control(String objectID) {
        constructComponents();
        initMainPanel();
        initComponents();
        loadProperties(objectID);
    }

    /**
    * Construct the Components.
    */
    @Override
    public final void constructComponents() {
        bControl = new JToggleButton();
        lControl = new JLabel();
        sControl = new JSeparator();
        pControl = new JPanel();

        bAxesSize = new JToggleButton();
        lAxesSize = new JLabel();
        cAxesSize = new JTextField();
        pAxesSize = new JPanel();
        lAxesSizeX = new JLabel();
        cAxesSizeX = new JTextField();
        lAxesSizeY = new JLabel();
        cAxesSizeY = new JTextField();
        lCloseRequestFcn = new JLabel();
        cCloseRequestFcn = new JTextField();
        cEventHandler = new JComboBox();
        lEventHandler = new JLabel();
        bFigurePosition = new JToggleButton();
        lFigurePosition = new JLabel();
        cFigurePosition = new JTextField();
        pFigurePosition = new JPanel();
        lFigurePositionX = new JLabel();
        cFigurePositionX = new JTextField();
        lFigurePositionY = new JLabel();
        cFigurePositionY = new JTextField();
        bFigureSize = new JToggleButton();
        lFigureSize = new JLabel();
        cFigureSize = new JTextField();
        pFigureSize = new JPanel();
        lFigureSizeX = new JLabel();
        cFigureSizeX = new JTextField();
        lFigureSizeY = new JLabel();
        cFigureSizeY = new JTextField();
        lResizeFcn = new JLabel();
        cResizeFcn = new JTextField();
        bViewport = new JToggleButton();
        lViewport = new JLabel();
        cViewport = new JTextField();
        pViewport = new JPanel();
        lViewportX = new JLabel();
        cViewportX = new JTextField();
        lViewportY = new JLabel();
        cViewportY = new JTextField();
    }

    /**
    * Insert show/hide button, title and main JPanel of section.
    */
    @Override
    public final void initMainPanel() {
        String SECTIONNAME = MessagesGED.control;
        this.setName(SECTIONNAME);

        layout.addHeader(this, pControl, bControl, lControl, sControl, SECTIONNAME);
        bControl.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent evt) {
                pControl.setVisible(!bControl.isSelected());
                HideFigure.checkAllButtons();
            }
        });
    }

    /**
    * Initialize the Components.
    */
    @Override
    public final void initComponents() {
        int ROW = 0;
        int LEFTMARGIN = 0; //to components
        int COLUMN = 1; //first column
        int LEFTMARGINIP = 0; //left margin - inner panel
        int COLUMNIP = 0; //left column - inner panel

        //Components of the property: Axes Size.
        ROW = layout.addInnerPanel(pControl, pAxesSize, bAxesSize, lAxesSize, cAxesSize, MessagesGED.axes_size, ROW);
        bAxesSize.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent evt) {
                pAxesSize.setVisible(!bAxesSize.isSelected());
                HideFigure.checkAllButtons();
            }
        });
        int rowAxesSize = 0;
        //Axes Size - X
        layout.addLabelTextField(pAxesSize, lAxesSizeX, "X", cAxesSizeX, true, LEFTMARGINIP, COLUMNIP, rowAxesSize++);
        cAxesSizeX.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent evt) {
                updateAxesSize();
            }
        });
        cAxesSizeX.addFocusListener(new FocusAdapter() {
            @Override
            public void focusLost(FocusEvent evt) {
                updateAxesSize();
            }
        });
        //Axes Size - Y
        layout.addLabelTextField(pAxesSize, lAxesSizeY, "Y", cAxesSizeY, true, LEFTMARGINIP, COLUMNIP, rowAxesSize++);
        cAxesSizeY.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent evt) {
                updateAxesSize();
            }
        });
        cAxesSizeY.addFocusListener(new FocusAdapter() {
            @Override
            public void focusLost(FocusEvent evt) {
                updateAxesSize();
            }
        });

        //Components of the property: CloseRequestFcn.
        layout.addLabelTextField(pControl, lCloseRequestFcn, MessagesGED.close_request_fcn,
                                 cCloseRequestFcn, true, LEFTMARGIN, COLUMN, ROW++);
        cCloseRequestFcn.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent evt) {
                updateCloseRequestFcn();
            }
        });
        cCloseRequestFcn.addFocusListener(new FocusAdapter() {
            @Override
            public void focusLost(FocusEvent evt) {
                updateCloseRequestFcn();
            }
        });

        //Components of the property: Event Handler.
        layout.addLabelComboBox(pControl, lEventHandler, MessagesGED.event_handler,
                                 cEventHandler, new String[] {MessagesGED.off, MessagesGED.function},
                                 LEFTMARGIN, COLUMN, ROW++);
        cEventHandler.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent evt) {
                //not implemented yet
            }
        });

        //Components of the property: Figure Position.
        ROW = layout.addInnerPanel(pControl, pFigurePosition, bFigurePosition,
                                   lFigurePosition, cFigurePosition, MessagesGED.figure_position, ROW);
        bFigurePosition.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent evt) {
                pFigurePosition.setVisible(!bFigurePosition.isSelected());
                HideFigure.checkAllButtons();
            }
        });

        int rowFigurePosition = 0;
        //Figure Position - X
        layout.addLabelTextField(pFigurePosition, lFigurePositionX, "X",
                                 cFigurePositionX, true, LEFTMARGINIP, COLUMNIP, rowFigurePosition++);
        cFigurePositionX.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent evt) {
                updateFigurePosition();
            }
        });
        cFigurePositionX.addFocusListener(new FocusAdapter() {
            @Override
            public void focusLost(FocusEvent evt) {
                updateFigurePosition();
            }
        });
        //Figure Position - Y
        layout.addLabelTextField(pFigurePosition, lFigurePositionY, "Y",
                                 cFigurePositionY, true, LEFTMARGINIP, COLUMNIP, rowFigurePosition++);
        cFigurePositionY.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent evt) {
                updateFigurePosition();
            }
        });
        cFigurePositionY.addFocusListener(new FocusAdapter() {
            @Override
            public void focusLost(FocusEvent evt) {
                updateFigurePosition();
            }
        });

        //Components of the property: Figure Size.
        ROW = layout.addInnerPanel(pControl, pFigureSize, bFigureSize,
                                   lFigureSize, cFigureSize, MessagesGED.figure_size, ROW);
        bFigureSize.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent evt) {
                pFigureSize.setVisible(!bFigureSize.isSelected());
                HideFigure.checkAllButtons();
            }
        });
        int rowFigureSize = 0;
        //Figure Size - X
        layout.addLabelTextField(pFigureSize, lFigureSizeX, "X",
                                 cFigureSizeX, true, LEFTMARGINIP, COLUMNIP, rowFigureSize++);
        cFigureSizeX.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent evt) {
                updateFigureSize();
            }
        });
        cFigureSizeX.addFocusListener(new FocusAdapter() {
            @Override
            public void focusLost(FocusEvent evt) {
                updateFigureSize();
            }
        });
        //Figure Size - Y
        layout.addLabelTextField(pFigureSize, lFigureSizeY, "Y",
                                 cFigureSizeY, true, LEFTMARGINIP, COLUMNIP, rowFigureSize++);
        cFigureSizeY.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent evt) {
                updateFigureSize();
            }
        });
        cFigureSizeY.addFocusListener(new FocusAdapter() {
            @Override
            public void focusLost(FocusEvent evt) {
                updateFigureSize();
            }
        });

        //Components of the property: ResizeFcn.
        layout.addLabelTextField(pControl, lResizeFcn, MessagesGED.resize_fcn,
                                 cResizeFcn, true, LEFTMARGIN, COLUMN, ROW++);
        cResizeFcn.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent evt) {
                updateResizeFcn();
            }
        });
        cResizeFcn.addFocusListener(new FocusAdapter() {
            @Override
            public void focusLost(FocusEvent evt) {
                updateResizeFcn();
            }
        });

        //Components of the property: Viewport.
        ROW = layout.addInnerPanel(pControl, pViewport, bViewport, lViewport, cViewport, MessagesGED.viewport, ROW);
        bViewport.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent evt) {
                pViewport.setVisible(!bViewport.isSelected());
                HideFigure.checkAllButtons();
            }
        });
        int rowViewport = 0;
        //Viewport - X
        layout.addLabelTextField(pViewport, lViewportX, "X",
                                 cViewportX, true, LEFTMARGINIP, COLUMNIP, rowViewport++);
        cViewportX.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent evt) {
                updateViewport();
            }
        });
        cViewportX.addFocusListener(new FocusAdapter() {
            @Override
            public void focusLost(FocusEvent evt) {
                updateViewport();
            }
        });
        //Viewport - Y
        layout.addLabelTextField(pViewport, lViewportY, "Y",
                                 cViewportY, true, LEFTMARGINIP, COLUMNIP, rowViewport++);
        cViewportY.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent evt) {
                updateViewport();
            }
        });
        cViewportY.addFocusListener(new FocusAdapter() {
            @Override
            public void focusLost(FocusEvent evt) {
                updateViewport();
            }
        });
   }

    /**
    * Loads the current properties of the section.
    * @param objectID Enters the identification of Figure.
    */
    @Override
    public final void loadProperties(String objectID) {
        if (objectID != null) {
            currentFigure = objectID;

            //Get the current status of the property: Axes Size
            Integer[] currentAxesSize = (Integer []) GraphicController.getController()
                    .getProperty(currentFigure, GraphicObjectProperties.__GO_AXES_SIZE__);
            cAxesSizeX.setText(currentAxesSize[0].toString());
            cAxesSizeY.setText(currentAxesSize[1].toString());
            titleAxesSize();

            //Get the current status of the property: CloseRequestFcn
            cCloseRequestFcn.setText((String) GraphicController.getController()
                                .getProperty(currentFigure, GraphicObjectProperties.__GO_CLOSEREQUESTFCN__));

            //Get the current status of the property: Event Handle
            //not implemented yet.

            //Get the current status of the property: Figure Position
            Integer[] currentFigurePosition = (Integer []) GraphicController.getController()
                    .getProperty(currentFigure, GraphicObjectProperties.__GO_POSITION__);
            cFigurePositionX.setText(currentFigurePosition[0].toString());
            cFigurePositionY.setText(currentFigurePosition[1].toString());
            titleFigurePosition();

            //Get the current status of the property: Figure Size
            Integer[] currentFigureSize = (Integer []) GraphicController.getController()
                    .getProperty(currentFigure, GraphicObjectProperties.__GO_SIZE__);
            cFigureSizeX.setText(currentFigureSize[0].toString());
            cFigureSizeY.setText(currentFigureSize[1].toString());
            titleFigureSize();

            //Get the current status of the property: ResizeFcn
            cResizeFcn.setText((String) GraphicController.getController()
                                .getProperty(currentFigure, GraphicObjectProperties.__GO_RESIZEFCN__));

            //Get the current status of the property: Viewport
            Integer[] currentViewport = (Integer []) GraphicController.getController()
                    .getProperty(currentFigure, GraphicObjectProperties.__GO_VIEWPORT__);
            cViewportX.setText(currentViewport[0].toString());
            cViewportY.setText(currentViewport[1].toString());
            titleViewport();
        }
    }

    /**
    * Updates the property: Axes Size.
    */
    private void updateAxesSize() {
        Integer[] setPosition = new Integer[] {
            Integer.parseInt(cAxesSizeX.getText()),
            Integer.parseInt(cAxesSizeY.getText())
        };
        GraphicController.getController()
                .setProperty(currentFigure, GraphicObjectProperties.__GO_AXES_SIZE__, setPosition);
        titleAxesSize();
    }

    /**
    * Inserts the current situation of the axes size.
    */
    public void titleAxesSize() {
        cAxesSize.setText(" [" + cAxesSizeX.getText() + " , " + cAxesSizeY.getText() + "]");
    }

    /**
    * Updates the property: CloseRequestFcn.
    */
    private void updateCloseRequestFcn() {
        GraphicController.getController().setProperty(
                currentFigure, GraphicObjectProperties.__GO_CLOSEREQUESTFCN__,
                cCloseRequestFcn.getText());
    }

    /**
    * Updates the property: Figure Position.
    */
    private void updateFigurePosition() {
        Integer[] setPosition = new Integer[] {
            Integer.parseInt(cFigurePositionX.getText()),
            Integer.parseInt(cFigurePositionY.getText())
        };
        GraphicController.getController()
                .setProperty(currentFigure, GraphicObjectProperties.__GO_POSITION__, setPosition);
        titleFigurePosition();
    }

    /**
    * Inserts the current situation of the Figure Position.
    */
    public void titleFigurePosition() {
        cFigurePosition.setText(" [" + cFigurePositionX.getText() + " , " + cFigurePositionY.getText() + "]");
    }

    /**
    * Updates the property: Figure Size.
    */
    private void updateFigureSize() {
        Integer[] setPosition = new Integer[] {
            Integer.parseInt(cFigureSizeX.getText()),
            Integer.parseInt(cFigureSizeY.getText())
        };
        GraphicController.getController()
                .setProperty(currentFigure, GraphicObjectProperties.__GO_SIZE__, setPosition);
        titleFigureSize();
    }

    /**
    * Inserts the current situation of the Figure Size.
    */
    public final void titleFigureSize() {
        cFigureSize.setText(" [" + cFigureSizeX.getText() + " , " + cFigureSizeY.getText() + "]");
    }

    /**
    * Updates the property: ResizeFcn.
    */
    private void updateResizeFcn() {
        GraphicController.getController().setProperty(
                currentFigure, GraphicObjectProperties.__GO_RESIZEFCN__,
                cResizeFcn.getText());
    }

    /**
    * Updates the property: Viewport.
    */
    private void updateViewport() {
        Integer[] setPosition = new Integer[] {
            Integer.parseInt(cViewportX.getText()),
            Integer.parseInt(cViewportY.getText())
        };
        GraphicController.getController()
                .setProperty(currentFigure, GraphicObjectProperties.__GO_VIEWPORT__, setPosition);
        titleViewport();
    }

    /**
    * Inserts the current situation of the viewport.
    */
    public final void titleViewport() {
        cViewport.setText(" [" + cViewportX.getText() + " , " + cViewportY.getText() + "]");
    }

    /**
    * Get Status of Main Jpanel.
    * @return visibility
    */
    public static boolean getStatus() {
        return pControl.isVisible();
    }

    /**
    * Set Visibility of Property Group.
    * @param visible boolean
    */
    public static void setVisibility(boolean visible) {
        pControl.setVisible(visible);
        bControl.setSelected(!visible);
    }
}