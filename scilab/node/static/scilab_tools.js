function getSubPadding(val) {
    if (val)
    {
        return val + 'px ';
    }

    return '0 ';
}
function getPadding(t, r, b, l) {
    var ret = '';

    ret += getSubPadding(t);
    ret += getSubPadding(r);
    ret += getSubPadding(b);
    ret += getSubPadding(l);
    return ret;
}

function isInputType(uid) {
    
    var $child = getJElementById(uid);
    if($child.hasClass('GO_UI_EDIT') || 
        $child.hasClass('GO_UI_SLIDER') ||
        $child.hasClass('GO_UI_SPINNER')) {
        return true;
    }

    return false;    
}

function getUID(id) {
    return parseInt(id.substring(3));
}

function createElement(type) {
    return document.createElement(type);
}

function getIdString(uid, suffix) {
    suffix = typeof suffix !== 'undefined' ? suffix : '';
    return 'uid' + uid + suffix;
}

function getScilab() {
    return document.getElementById('scilab');
}

function getElementById(uid, suffix) {
    return document.getElementById(getIdString(uid, suffix));
}

function getJElementById(uid, suffix) {
    return $("#" + getIdString(uid, suffix));
}

function createCommonIUControl(uid, htmlType, cssClass) {
    var temp = createElement(htmlType);
    temp.id = getIdString(uid);
    temp.className = cssClass;

    //add item temporary in main scilabview div waiting __GO_PARENT__ update
    var $parent = $('#scilab');
    $parent.append(temp);
    return temp;
}
