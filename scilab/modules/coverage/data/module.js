function order(id, col, ascent, bid) {
    var link = document.getElementById(bid);
    if (link.className == 'buttonOrderEnable') {
	return;
    }
    var parent = link.parentNode;
    var tok = bid.split("_");
    var n = tok[1] == 1 ? 2 : 1;
    var butid = tok[2];
    link.setAttribute('class', 'buttonOrderEnable');
    link = document.getElementById("but_" + n + "_" + butid);
    link.setAttribute('class', 'buttonOrderDisable');

    var gbs = parent.parentNode.parentNode.getElementsByClassName("groupButton");
    for (var i = 0, l = gbs.length; i < l; ++i) {
	if (gbs[i] != parent) {
	    console.log(gbs[i].children[0]);
	    gbs[i].children[0].setAttribute('class', 'buttonOrderDisable');
	    gbs[i].children[1].setAttribute('class', 'buttonOrderDisable');
	}
    }

    var table = document.getElementById(id);
    var rows = table.getElementsByTagName("tr");
    rows = Array.prototype.slice.call(rows, 1);
    var parent = rows[0].parentNode;
    for (var i = 0, l = rows.length; i < l; ++i) {
	parent.removeChild(rows[i]);
    }
    rows.sort(function (a,b) {
	var x = a.getElementsByTagName("td")[col].getElementsByClassName("cmp")[0].textContent;  
	var y = b.getElementsByTagName("td")[col].getElementsByClassName("cmp")[0].textContent;
	var _x = parseFloat(x);
	var _y = parseFloat(y);
	if (!isNaN(_x) && !isNaN(_y)) {
	    x = _x;
	    y = _y;
	}
	if (x == y)
	    return 0;
	if (x < y) {
	    return ascent ? -1 : 1;
	}
	return ascent ? 1 : -1;
    });
    var bool = true;
    for (var i = 0, l = rows.length; i < l; ++i) {
	rows[i].setAttribute("class", bool ? "altern2" : "altern1");
	parent.appendChild(rows[i]);
	bool = !bool;
    }
}
