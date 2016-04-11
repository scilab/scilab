function createFigure(uid) {
    var parent = getScilab();
    var temp = createElement("DIV");
    temp.id = getIdString(uid);
    temp.className = 'GO_FIGURE';
    parent.appendChild(temp);
    addWindow(temp);
    return temp;
}

function createAxes(uid) {
    var temp = createCommonIUControl(uid, 'DIV', 'GO_AXES');
    return temp;
}

function createPushButton(uid) {
    var temp = createCommonIUControl(uid, 'BUTTON', 'GO_UI_PUSHBUTTON');
    //add bootstrap class
    var $temp = $("#" + temp.id);
    $temp.addClass("btn");
    
    return temp;
}

function createFrame(uid) {
    var temp = createCommonIUControl(uid, 'DIV', 'GO_UI_FRAME');

    //add bootstrap class
    var $temp = $("#" + temp.id);
    $temp.addClass("panel");
    $temp.addClass("panel-default");
    $temp.css("margin", "0");
    
    var header = createCommonIUControl(uid, 'DIV', 'panel-heading');
    header.id = getIdString(uid, "_header");
    header.style.display = 'none';
    
    var footer = createCommonIUControl(uid, 'DIV', 'panel-footer');
    footer.id = getIdString(uid, "_footer");
    footer.style.display = 'none';
    
    var child = createCommonIUControl(uid, 'DIV', 'panel-body');
    child.id = getIdString(uid, "_body");
    
    temp.appendChild(header);
    temp.appendChild(child);
    temp.appendChild(footer);
    return temp;
}

function createText(uid) {
    var temp = createCommonIUControl(uid, 'LABEL', 'GO_UI_TEXT');
//    var $temp = $("#" + temp.id);
//    $temp.addClass("label");
//    $temp.addClass("label-default");
    return temp;
}

function createEdit(uid) {
    var temp = createCommonIUControl(uid, 'INPUT', 'GO_UI_EDIT');
    temp.type = 'text';

    //add bootstrap class
    var $temp = $("#" + temp.id);
    $temp.addClass("form-control");
    return temp;
}

function createCheckbox(uid) {
    //for checkbox we need to create 4 elements.

    //a div to enclose others
    var main = createCommonIUControl(uid, 'DIV', 'GO_UI_CHECKBOX');

    //add bootstrap class
    var $main = $("#" + main.id);
    $main.addClass("checkbox");
    main.style.position = 'absolute';
    main.style.margin = '0';
    main.style.display = 'flex';
    main.style.alignItems = 'center';
    
    //the label of the checkbox
    var label = createCommonIUControl(uid, 'LABEL', 'GO_UI_CHECKBOX_LABEL');
    label.id = getIdString(uid, '_label');
    main.appendChild(label);

     //the checkbox itself
    var temp = createCommonIUControl(uid, 'INPUT', 'GO_UI_CHECKBOX_RADIO');
    temp.type = 'checkbox';
    temp.id = getIdString(uid, '_checkbox');
    temp.style.marginLeft = '-15px';
    label.appendChild(temp);
    
    //and a span to esay update text value
    var span = createCommonIUControl(uid, 'SPAN', '');
    span.id = getIdString(uid, '_span');
    label.appendChild(span);

    return main;
}

function createRadio(uid) {
    //for checkbox we need to create 4 elements.

    //a div to enclose others
    var main = createCommonIUControl(uid, 'DIV', 'GO_UI_RADIOBUTTON');

    //add bootstrap class
    var $main = $("#" + main.id);
    $main.addClass("radio");
    main.style.position = 'absolute';
    main.style.margin = '0';
    main.style.display = 'flex';
    main.style.alignItems = 'center';
 
    //the label of the radio
    var label = createCommonIUControl(uid, 'LABEL', 'GO_UI_RADIOBUTTON_LABEL');
    label.id = getIdString(uid, '_label');
    main.appendChild(label);

     //the radio itself
   var temp = createCommonIUControl(uid, 'INPUT', 'GO_UI_RADIOBUTTON_RADIO');
    temp.type = 'radio';
    temp.id = getIdString(uid, '_radio');
    temp.style.marginLeft = '4px';
    temp.style.marginBottom = '12px';
    label.appendChild(temp);
    
    //and a span to esay update text value
    var span = createCommonIUControl(uid, 'SPAN', '');
    span.id = getIdString(uid, '_span');
    label.appendChild(span);

    return main;
}

function createSlider(uid) {
    var temp = createCommonIUControl(uid, 'INPUT', 'GO_UI_SLIDER');
    temp.type = 'range';
    return temp;
}

function createListbox(uid) {
    var temp = createCommonIUControl(uid, 'SELECT', 'GO_UI_LISTBOX');
    return temp;
}

function createCombobox(uid) {
    var temp = createCommonIUControl(uid, 'SELECT', 'GO_UI_POPUPMENU');
    temp.size = 1;
    var $temp = $("#" + temp.id);
    $temp.addClass("form-control");
    return temp;
}

function createSpinner(uid) {
    var temp = createCommonIUControl(uid, 'INPUT', 'GO_UI_SPINNER');
    temp.type = 'number';
    //$("#" + getIdString(uid)).TouchSpin({verticalbuttons: true});
    return temp;
}

function createTab(uid) {
    var temp = createCommonIUControl(uid, 'DIV', 'GO_UI_TAB');

    //create a list for tab headers
    var ul = createElement('UL');
    ul.id = getIdString(uid, '_ul');
    temp.appendChild(ul);

    var $ul = $("#" + ul.id);
    $ul.addClass("nav");
    $ul.addClass("nav-tabs");

    var content = createElement('DIV');
    content.id = getIdString(uid, '_tabs');
    temp.appendChild(content);

    var $content = $("#" + content.id);
    $content.addClass("tab-content");

    return temp;
}

function deleteObject(uid) {
    $child = getJElementById(uid);
    if($child) {
        if($child.hasClass('GO_FIGURE')) {
            delWindow(uid);
        } else {
            $child.remove();
        }
    }
}