//helper call by scilab to insert gridbad child in good order.
function gridbagHelperTD(parent, child, pos) {
    for(var i = 0 ; i < parent.cells.length ; ++i){
        var __tr__ = parent.cells[i];
        var idxstr = __tr__.id.indexOf('_');
        var idx = parseInt(__tr__.id.substring(idxstr+1));
        if(idx > pos) {
            parent.insertBefore(child, __tr__);
            return;
        }
    }

    //not found, insert @last pos
    parent.insertBefore(child, null);
}

function gridbagHelperTR(parent, child, pos) {
    for(var i = 0 ; i < parent.rows.length ; ++i){
        var __tr__ = parent.rows[i];
        var idxstr = __tr__.id.indexOf('_');
        var idx = parseInt(__tr__.id.substring(idxstr+1));
        if(idx > pos) {
            parent.insertBefore(child, __tr__);
            return;
        }
    }

    //not found, insert @last pos
    parent.insertBefore(child, null);
}

function tabSelectHelper(tab, index) {
    var __ul__ = document.getElementById(tab.id + '_ul');

    if(!__ul__) {
        return;
    }

    var __item__ = __ul__.children[index];
    if(!__item__) {
        return;
    }

    var __id__ = __item__.id.substr(0, __item__.id.indexOf('_'));

    //change tab label
    for (var i = 0; i < __ul__.children.length; ++i) {
        __ul__.children[i].className = '';
    }

    __item__.className = 'GO_UI_TAB_SELECTED';

    //change div visibility

    //all hide
    var __next__ = __ul__.nextElementSibling;
    while(__next__) {
        __next__.style.display = 'none';
        __next__ = __next__.nextElementSibling;
    }

    //show good one
    document.getElementById(__id__).style.display = 'flex';
}

function addTabHelper(uid, child) {
    var __child__ = getElementById(child);
    var __li__ = createElement('LI');
    __li__.id = getIdString(child, '_li');

    //create input button
    var __btn__ = createElement('INPUT');
    __btn__.id = getIdString(child, '_btn');
    __btn__.type = 'button';
    __btn__.addEventListener('click', onTab);

    //add button in il
    __li__.appendChild(__btn__);

    //add il as first child in ul
    var __ul__ = getElementById(uid, '_ul');
    __ul__.insertBefore(__li__, __ul__.firstChild);

    //update child properties
    __child__.style.position = 'inherit';
    __child__.style.left = 'inherit';
    __child__.style.bottom = 'inherit';
    __child__.style.width = '100%';
    __child__.style.height = 'inherit';
}