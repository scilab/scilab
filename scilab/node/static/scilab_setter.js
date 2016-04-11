function setParent(uid, parent) {
    var $child = getJElementById(uid);
    var $parent = getJElementById(parent);
    
    if($child.hasClass("GO_AXES")) {
        $child.width($parent.width());
        $child.height($parent.height());

        Plotly.newPlot($child.prop('id'), [{}], {}, {displayModeBar:false, showLink:false, scrollZoom:true});
    }

    if($parent.hasClass("GO_UI_FRAME")) {
        $parent = getJElementById(parent, "_body");
    }
    
    $parent.append($child);
}

function setFigureSize(uid, val) {
    var child = getElementById(uid);
    child.style.width = val[0] + 'px';
    child.style.height = val[1] + 'px';
}

function setFigureName(uid, val) {
    var $fig = getJElementById(uid, '_btn');
    $fig.text(val);
}

function setUIPosition(uid, val, units) {
    var u = 'px';
    if(units === 'normalized') {
        u = '%';
        val[0] *= 100;
        val[1] *= 100;
        val[2] *= 100;
        val[3] *= 100;
    }
    
    var child = getElementById(uid);
    child.style.left = val[0] + u;
    child.style.bottom = val[1] + u;
    child.style.width = val[2] + u;
    child.style.height = val[3] + u;
}

function setUIString(uid, val, parent) {
    var child = getElementById(uid);
    
    var $child = getJElementById(uid);
    
    if($child.hasClass('GO_UI_CHECKBOX') || $child.hasClass('GO_UI_RADIOBUTTON')) {
        var label = getElementById(uid + '_span');
        label.textContent = val[0];
        return;
    }

    if($child.hasClass('GO_UI_POPUPMENU') || $child.hasClass('GO_UI_LISTBOX')) {
        //remove previous values
        while (child.length) {child.remove(0);}

       var size = val.length;
        for(var i = 0 ; i < size ; ++i) {
            child.add(new Option(val[i]));
        }

        if(child.className === 'GO_UI_LISTBOX') {
            //switch to listbox instead of combobox
           child.size = 2;
        }
        
        return;
    }
    
    if($child.hasClass('GO_UI_FRAME')) {
       var $parent = getJElementById(parent);
        if($parent && $parent.hasClass('GO_UI_TAB')) {
           var tab = getElementById(uid + '_tab');
            tab.textContent = val[0];
        }
        
        return;
    }

    if($child.hasClass('GO_UI_TAB')) {
        return;
    }

    if(isInputType(uid)) {
        child.value = val[0];
    } else {
        child.textContent = val[0];
    }
}

function setVisible(uid, val) {
    var child = getElementById(uid);
    if(!child) {
        console.log("setVisible : " + uid);
        return;
    }
    child.style.visibility = val;
}

function setUIEnable(uid, val) {
    var child = getJElementById(uid);
    if(val) {
        child.removeClass("disabled");
    } else {
        child.addClass("disabled");
    }
}

function setUIForegroundColor(uid, val) {
    var child = getElementById(uid);
    child.style.color = val;
}

function setUIBackgroundColor(uid, val) {
    var child = getElementById(uid);
    child.style.backgroundColor = val;
}

function setUIFontAngle(uid, val) {
    var child = getElementById(uid);
    child.style.fontStyle = val;
}

function setUIFontName(uid, val) {
    var child = getElementById(uid);
    child.style.fontFamily = val;
}

function setUIFontSize(uid, val) {
    var child = getElementById(uid);
    child.style.fontSize = val;
}

function setUIFontWeight(uid, val) {
    var child = getElementById(uid);
    child.style.fontWeight = val;
}

function setUIHorizontalAlignment(uid, val) {
    var $child = getJElementById(uid);
    if($child.hasClass('GO_UI_TEXT')) {
        switch(val) {
            default:
            case 'left' :
                val = 'flex-start';
                break;
            case 'center' :
                break;
            case 'right' :
                val = 'flex-end';
                break;
        }
        
        $child.css("justify-content", val);
    } else {
        $child.css("text-align", val);
    }
}

function setUIVerticalAlignment(uid, val) {
    var child = getElementById(uid);
    child.style.lineHeight = val;
}

function setUILayout(uid, val) {
    var child = getElementById(uid);

    switch(val) {
        case LayoutType.LAYOUT_BORDER :
            //add 'frame' in child to be more ... flexible ;)
            var layout = createElement('DIV');
            layout.id = getIdString(uid, '_layout');
            layout.style.display = 'flex';
            layout.style.flexDirection = 'column';
            layout.style.width = '100%';
            layout.style.height = 'inherit';

            //add top
            var header = createElement('HEADER');
            header.id = getIdString(uid, '_top');
            header.style.width = '100%';

            //add middle band
            var middle = createElement('DIV');
            middle.id = getIdString(uid, '_middle');
            middle.style.flex = '1';
            middle.style.display = 'flex';

            //add left
            var left = createElement('ASIDE');
            left.id = getIdString(uid, '_left');
            left.style.display = 'flex';

            //add center
            var center = createElement('ARTICLE');
            center.id = getIdString(uid, '_center');
            center.style.flex = '1';
            center.style.display = 'flex';
            center.style.width = '100%';

            //add right
            var right = createElement('ASIDE');
            right.id = getIdString(uid, '_right');
            right.style.display = 'flex';

            //add bottom
            var footer = createElement('FOOTER');
            footer.id = getIdString(uid, '_bottom');
            footer.style.width = '100%';

            //hierarchy
            middle.appendChild(left);
            middle.appendChild(center);
            middle.appendChild(right);

            layout.appendChild(header);
            layout.appendChild(middle);
            layout.appendChild(footer);

            child.appendChild(layout);
            break;

        case LayoutType.LAYOUT_GRID:
            break;

        case LayoutType.LAYOUT_GRIDBAG:
            //add empty table
           var table = createElement('TABLE');
            table.id = getIdString(uid, '_table');
            table.style.margin = '0';
            table.style.padding = '0';
            table.style.width = '100%';
            table.style.height = '100%';
            table.style.borderCollapse = 'collapse';

            //insert table in parent
            child.appendChild(table);
            break;

        default:
        case LayoutType.LAYOUT_NONE:
            break;
    }
}

function setUIMin(uid, val) {
    var child = getElementById(uid);
    child.min  = val;
}

function setUIMax(uid, val) {
    var child = getElementById(uid);
    child.max  = val;
}

function setUIStep(uid, val) {
    var child = getElementById(uid);
    child.step  = val;
}

function setUIValue(uid, val) {
    var child = getElementById(uid);
    if(!child) {
        console.log("setUIValue : " + uid);
        return;
    }
    switch(child.className) {
        case 'GO_UI_SPINNER':
        case 'GO_UI_SLIDER':
            child.value  = '' + val;
            break;
        case 'GO_UI_TAB':
            tabSelectHelper(child, val);
            break;
    }
}

function setUIBorder(uid, parent, pos, pad, size) {
    var child = getElementById(uid);
    var position;
    var padding;
    switch(pos) {
        case BorderLayoutType.BORDER_CENTER:
            position = '_center';
            child.style.width = 'inherit';
            child.style.height = 'inherit';
            padding = getPadding(0, 0, 0, 0);
            break;

        case BorderLayoutType.BORDER_BOTTOM:
            position = '_bottom';
            child.style.width = '100%';
            if(size[1] === -1) {
                child.style.height = 'inherit';
            } else {
                child.style.height = getSubPadding(size[1]);
            }

            padding = getPadding(pad[1], 0, 0, 0);
            break;

        case BorderLayoutType.BORDER_TOP:
            position = '_top';
            child.style.width = '100%';

            if (size[1] === -1) {
                child.style.height = 'inherit';
            } else {
                child.style.height = getSubPadding(size[1]);
            }

            padding = getPadding(0, 0, pad[1], 0);
            break;

        case BorderLayoutType.BORDER_LEFT:
            position = '_left';

            if (size[0] == -1){
                child.style.width = 'inherit';
            } else {
                child.style.width = getSubPadding(size[0]);
            }

            child.style.height = 'inherit';
            padding = getPadding(0, pad[0], 0, 0);
            break;

        case BorderLayoutType.BORDER_RIGHT:
            position = '_right';

            if (size[0] == -1) {
                child.style.width = 'inherit';
            } else {
                child.style.width = getSubPadding(size[0]);
            }

            child.style.height = 'inherit';
            padding = getPadding(0, 0, 0, pad[0]);
            break;
    }

    //move child in targeted cell
    var parentGrid = getElementById(parent, position);
    parentGrid.appendChild(child);
    parentGrid.style.padding = padding;

    //to well perform positionning, we must clear some default properties
    //position left, right, width, height,

    child.style.position = 'inherit';
    child.style.left = 'inherit';
    child.style.bottom = 'inherit';
}

function setUIGridBag(uid, parent, grid) {
    var child = getElementById(uid);
    if(!child) {
        console.log("setUIGridBag: " + uid);
        return;
    }
    child.style.position = 'inherit';
    child.style.left = 'inherit';
    child.style.bottom = 'inherit';
    child.style.width = '100%';
    child.style.height = '100%';

    //create a td with grid information and add it to the good cell ( or create if not exist ! )

    //build td child
    var td = createElement('TD');
    td.id = getIdString(parent, '_' + grid[0] + '_' + grid[1]);

    if (grid[2] !== '1') {
        td.colSpan = grid[2];
    }

    if (grid[3] !== '1') {
        td.rowSpan = grid[3];
    }

    td.appendChild(child);

    var table = getElementById(parent, '_table');

    //build or get tr
    var trName = '_' + grid[1];
    var tr = getElementById(parent, trName);

    if(tr == null) {
        tr = createElement('TR');
        tr.id = getIdString(parent, trName);
        gridbagHelperTR(table, tr, grid[1]);
    }

    gridbagHelperTD(tr,td, grid[0]);

    //force refresh of table, move it to another component and rollback
    var scilab = getScilab();
    scilab.appendChild(table);
    var elem = getElementById(parent);
    elem.appendChild(table);
}



function setUIFrameBorder(uid, title) {
    
    if(title.position === "") {
        return;
    }
    var border;
    var header = getElementById(uid, "_header");
    var footer = getElementById(uid, "_footer");

    if(title.position === "top") {
       header.style.display = 'block';
       footer.style.display = 'none';
       border = header;
    } else { //bottom
       footer.style.display = 'block';
       header.style.display = 'none';
       border = footer;
    }
    
    border.textContent = title.text;
    border.style.textAlign = title.alignment;
    border.style.fontName = title.fontName;
    border.style.fontStyle = title.fontStyle;
    border.style.fontSize = title.fontSize + 'px';
    border.style.fontWeight = title.fontWeight;
    border.style.color = title.fontColor;
}

function setUIIcon(uid, icon, val) {
    var child = getElementById(uid);

    //add span element in button to show image
    var span = createElement('SPAN');
    if(icon.substring(0, 5) === 'glyph') {
        span.className = "glyphicon " + icon;
        span.style.paddingRight = '4px';
    } else {
        span.style.background = '#f3f3f3 url(' + icon + ') no-repeat left center';
        span.style.paddingLeft = '20px';
    }

    var text = createElement('SPAN');
    text.textContent = val;
    text.style = child.style;

    child.innerHTML = "";
    child.appendChild(span);
    child.appendChild(text);
}

function setUIGroupName(uid, val) {
    var child = getElementById(uid, "_radio");
    if(child) {
        child.name  = val;
    }
}


function setCallback(uid) {
    var event;
    var func;

    var child = getElementById(uid);

    var $child = getJElementById(uid);

    if($child.hasClass('GO_UI_PUSHBUTTON')) {
        event = "click";
        func = onPushButton;
    } else if($child.hasClass('GO_UI_CHECKBOX')) {
        //change child to real checkbox
        child = getElementById(uid, "_checkbox");
        event = "click";
        func = onCheckBox;
    } else if($child.hasClass('GO_UI_RADIOBUTTON')) {
        //change child to real radio
        child = getElementById(uid, "_radio");
        event = "click";
        func = onRadioButton;
    } else if($child.hasClass('GO_UI_LISTBOX')) {
        event = "change";
        func = onListBox;
    } else if($child.hasClass('GO_UI_POPUPMENU')) {
        event = "change";
        func = onComboBox;
    } else if($child.hasClass('GO_UI_SLIDER')) {
        event = "input";
        func = onSlider;
    } else if($child.hasClass('GO_UI_EDIT')) {
        event = "input";
        func = onEditBox;
    } else if($child.hasClass('GO_UI_SPINNER')) {
        event = "input";
        func = onSpinner;
    }

    //add callback listener
    child.addEventListener(event, func);
}