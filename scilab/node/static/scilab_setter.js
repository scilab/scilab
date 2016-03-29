function setParent(uid, parent) {
    var __child__ = getElementById(uid);
    var __parent__ = getElementById(parent);
    __parent__.appendChild(__child__);
}

function setFigureSize(uid, val) {
    var __child__ = getElementById(uid);
    __child__.style.width = val[0] + 'px';
    __child__.style.height = val[1] + 'px';
}

function setUIPosition(uid, val) {
    var __child__ = getElementById(uid);
    __child__.style.left = val[0] + 'px';
    __child__.style.bottom = val[1] + 'px';
    __child__.style.width = val[2] + 'px';
    __child__.style.height = val[3] + 'px';
}

function setUIString(uid, val, parent) {
    var __child__ = getElementById(uid);
    switch(__child__.className) {
        case 'GO_UI_CHECKBOX':
        case 'GO_UI_RADIOBUTTON':
           var __label__ = getElementById(uid + '_label');
            __label__.innerHTML = val[0];
            break;

        case 'GO_UI_POPUPMENU' :
        case 'GO_UI_LISTBOX':
            //remove previous values
            while (__child__.length) {__child__.remove(0);}

           var size = val.length;
            for(var i = 0 ; i < size ; ++i) {
                __child__.add(new Option(val[i]));
            }

            if(__child__.className === 'GO_UI_LISTBOX') {
                //switch to listbox instead of combobox
               __child__.size = 2;
            }

            break;

        case 'GO_UI_TAB':
            //nothing to do
           break;

       case 'GO_UI_FRAME':
           var __parent__ = getElementById(parent);
            if(__parent__ && __parent__.className === 'GO_UI_TAB') {
               var __btn__ = getElementById(uid + '_btn');
                __btn__.value = val[0];
            }
            break;

        default:
            if(isInputType(uid)) {
                __child__.value = val[0];
            } else {
                __child__.innerHTML = val[0];
            }
            break;
    }
}

function setVisible(uid, val) {
    var __child__ = getElementById(uid);
    __child__.style.visibility = val;
}

function setUIEnable(uid, val) {
    var __child__ = $('#' + getIdString(uid));
    if(val) {
        __child__.removeClass("disabled");
    } else {
        __child__.addClass("disabled");
    }
}

function setUIForegroundColor(uid, val) {
    var __child__ = getElementById(uid);
    __child__.style.color = val;
}

function setUIBackgroundColor(uid, val) {
    var __child__ = getElementById(uid);
    __child__.style.backgroundColor = val;
}

function setUIFontAngle(uid, val) {
    var __child__ = getElementById(uid);
    __child__.style.fontStyle = val;
}

function setUIFontName(uid, val) {
    var __child__ = getElementById(uid);
    __child__.style.fontFamily = val;
}

function setUIFontSize(uid, val) {
    var __child__ = getElementById(uid);
    __child__.style.fontSize = val;
}

function setUIFontWeight(uid, val) {
    var __child__ = getElementById(uid);
    __child__.style.fontWeight = val;
}

function setUIHorizontalAlignment(uid, val) {
    var __child__ = getElementById(uid);
    __child__.style.textAlign = val;
}

function setUIVerticalAlignment(uid, val) {
    var __child__ = getElementById(uid);
    __child__.style.lineHeight = val;
}

function setUILayout(uid, val) {
    var __child__ = getElementById(uid);

    switch(val) {
        case LayoutType.LAYOUT_BORDER :
            //add 'frame' in child to be more ... flexible ;)
           var __layout__ = createElement('DIV');
            __layout__.id = getIdString(uid, '_layout');
            __layout__.style.display = 'flex';
            __layout__.style.flexDirection = 'column';
            __layout__.style.width = '100%';
            __layout__.style.height = 'inherit';

            //add top
           var __top__ = createElement('HEADER');
            __top__.id = getIdString(uid, '_top');
            __top__.style.width = '100%';

            //add middle band
           var __middle__ = createElement('DIV');
            __middle__.id = getIdString(uid, '_middle');
            __middle__.style.flex = '1';
            __middle__.style.display = 'flex';

            //add left
           var __left__ = createElement('ASIDE');
            __left__.id = getIdString(uid, '_left');
            __left__.style.display = 'flex';

            //add center
           var __center__ = createElement('ARTICLE');
            __center__.id = getIdString(uid, '_center');
            __center__.style.flex = '1';
            __center__.style.display = 'flex';
            __center__.style.width = '100%';

            //add right
           var __right__ = createElement('ASIDE');
            __right__.id = getIdString(uid, '_right');
            __right__.style.display = 'flex';

            //add bottom
           var __bottom__ = createElement('FOOTER');
            __bottom__.id = getIdString(uid, '_bottom');
            __bottom__.style.width = '100%';

            //hierarchy
            __middle__.appendChild(__left__);
            __middle__.appendChild(__center__);
            __middle__.appendChild(__right__);

            __layout__.appendChild(__top__);
            __layout__.appendChild(__middle__);
            __layout__.appendChild(__bottom__);

            __child__.appendChild(__layout__);
            break;

        case LayoutType.LAYOUT_GRID:
            break;

        case LayoutType.LAYOUT_GRIDBAG:
            //add empty table
           var __table__ = createElement('TABLE');
            __table__.id = getIdString(uid, '_table');
            __table__.style.margin = '0';
            __table__.style.padding = '0';
            __table__.style.width = '100%';
            __table__.style.height = '100%';
            __table__.style.borderCollapse = 'collapse';

            //insert table in parent
            __child__.appendChild(__table__);
            break;

        default:
        case LayoutType.LAYOUT_NONE:
            break;
    }
}

function setUIMin(uid, val) {
    var __child__ = getElementById(uid);
    __child__.min  = val;
}

function setUIMax(uid, val) {
    var __child__ = getElementById(uid);
    __child__.max  = val;
}

function setUIStep(uid, val) {
    var __child__ = getElementById(uid);
    __child__.step  = val;
}

function setUIValue(uid, val) {
    var __child__ = getElementById(uid);
    switch(__child__.className) {
        case 'GO_UI_SPINNER':
        case 'GO_UI_SLIDER':
            __child__.value  = '' + val;
            break;
        case 'GO_UI_TAB':
            tabSelectHelper(__child__, val);
            break;
    }
}

function setUIBorder(uid, parent, pos, pad, size) {
    var __child__ = getElementById(uid);
    var position;
    var padding;
    switch(pos) {
        case BorderLayoutType.BORDER_CENTER:
            position = '_center';
            __child__.style.width = 'inherit';
            __child__.style.height = 'inherit';
            padding = getPadding(0, 0, 0, 0);
            break;

        case BorderLayoutType.BORDER_BOTTOM:
            position = '_bottom';
            __child__.style.width = '100%';
            if(size[1] === -1) {
                __child__.style.height = 'inherit';
            } else {
                __child__.style.height = getSubPadding(size[1]);
            }

            padding = getPadding(pad[1], 0, 0, 0);
            break;

        case BorderLayoutType.BORDER_TOP:
            position = '_top';
            __child__.style.width = '100%';

            if (size[1] === -1) {
                __child__.style.height = 'inherit';
            } else {
                __child__.style.height = getSubPadding(size[1]);
            }

            padding = getPadding(0, 0, pad[1], 0);
            break;

        case BorderLayoutType.BORDER_LEFT:
            position = '_left';

            if (size[0] == -1){
                __child__.style.width = 'inherit';
            } else {
                __child__.style.width = getSubPadding(size[0]);
            }

            __child__.style.height = 'inherit';
            padding = getPadding(0, pad[0], 0, 0);
            break;

        case BorderLayoutType.BORDER_RIGHT:
            position = '_right';

            if (size[0] == -1) {
                __child__.style.width = 'inherit';
            } else {
                __child__.style.width = getSubPadding(size[0]);
            }

            __child__.style.height = 'inherit';
            padding = getPadding(0, 0, 0, pad[0]);
            break;
    }

    //move child in targeted cell
    var __parent__ = getElementById(parent, position);
    __parent__.appendChild(__child__);
    __parent__.style.padding = padding;

    //to well perform positionning, we must clear some default properties
    //position left, right, width, height,

    __child__.style.position = 'inherit';
    __child__.style.left = 'inherit';
    __child__.style.bottom = 'inherit';
}

function setUIGridBag(uid, parent, grid) {
    var __child__ = getElementById(uid);
    __child__.style.position = 'inherit';
    __child__.style.left = 'inherit';
    __child__.style.bottom = 'inherit';
    __child__.style.width = '100%';
    __child__.style.height = '100%';

    //create a td with grid information and add it to the good cell ( or create if not exist ! )

    //build td child
    var __td__ = createElement('TD');
    __td__.id = getIdString(parent, '_' + grid[0] + '_' + grid[1]);

    if (grid[2] !== '1') {
        __td__.colSpan = grid[2];
    }

    if (grid[3] !== '1') {
        __td__.rowSpan = grid[3];
    }

    __td__.appendChild(__child__);

    var __table__ = getElementById(parent, '_table');

    //build or get tr
    var trName = '_' + grid[1];
    var __tr__ = getElementById(parent, trName);

    if(__tr__ == null) {
        __tr__ = createElement('TR');
        __tr__.id = getIdString(parent, trName);
        gridbagHelperTR(__table__, __tr__, grid[1]);
    }

    gridbagHelperTD(__tr__,__td__, grid[0]);

    //force refresh of table, move it to another component and rollback
    var __scilab__ = document.getElementById('scilab');
    __scilab__.appendChild(__table__);
    var __parent__ = getElementById(parent);
    __parent__.appendChild(__table__);
}



function setUIFrameBorder(uid, border, parent, title) {
    var __child__ = getElementById(uid);
    var __border__ = createCommonIUControl(border, 'DIV', 'GO_UI_FRAME_BORDER');

    var __fieldset__ = createElement('FIELDSET');
   __fieldset__.id = getIdString(uid, '_fieldset');
    var __legend__ = createElement('LEGEND');
    __legend__.id = getIdString(uid, '_legend');
    __legend__.innerHTML = title;

    __fieldset__.appendChild(__legend__);
    __fieldset__.appendChild(__child__);
    __border__.appendChild(__fieldset__);
    var __parent__ = getElementById(parent);
    __parent__.appendChild(__border__);
}

function setUIIcon(uid, icon, val) {
    var __child__ = getElementById(uid);

    //add span element in button to show image
    var __icon__ = createElement('SPAN');
    __icon__.style.background = '#f3f3f3 url(' + icon + ') no-repeat left center';
    __icon__.style.paddingLeft = '20px';
    __child__.appendChild(__icon__);
    //__child__.innerHTML += val;
}

function setCallback(uid) {
    var event;
    var func;

    var __child__ = getElementById(uid);

    switch(__child__.className) {
        case 'btn' :
            event = "click";
            func = onPushButton;
            break;
        case 'GO_UI_CHECKBOX' :
            event = "click";
            func = onCheckBox;
            break;
        case 'GO_UI_RADIOBUTTON' :
            event = "click";
            func = onRadioButton;
            break;
        case 'GO_UI_LISTBOX' :
            event = "change";
            func = onListBox;
            break;
        case 'GO_UI_POPUPMENU' :
            event = "change";
            func = onComboBox;
            break;
        case 'GO_UI_SLIDER' :
            event = "input";
            func = onSlider;
            break;
        case 'GO_UI_EDIT' :
            event = "input";
            func = onEditBox;
            break;
        case 'GO_UI_SPINNER' :
            event = "input";
            func = onSpinner;
            break;
        default :
            return;
    }

    //add callback listener
    __child__.addEventListener(event, func);
}