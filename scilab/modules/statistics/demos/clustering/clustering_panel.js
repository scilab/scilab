// Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2026 - Dassault Systèmes S.E. - Adeline CARNIS
//
// This file is released under the 3-clause BSD license. See COPYING-BSD.

$(document).ready(() => {
    $("#dbscan-help").click(() => {
        toScilab('help|dbscan');
    });

    $("#meanshift-help").click(() => {
        toScilab('help|meanshift');
    });

    $("#kmeans-help").click(() => {
        toScilab('help|kmeans');
    });

    $("#n-pts").on("input", function() {
        $("#n-val").text(parseInt($("#n-pts").val()));
        regenerate();
    });

    // dbscan - eps
    $("#eps").on("input", function() {
        $("#eps-val").text(parseFloat($("#eps").val()).toFixed(2));
        computeDbscan();
    });

    // dbscan - minPts
    $("#minpts").on("input", function() {
        $("#minpts-val").text(parseInt($("#minpts").val()));
        computeDbscan();
    });

    // dbscan - metric
    $("#metric").change(function() {
        computeDbscan();
    })

    // meanshift - bw
    $("#bandwidth").on("input", function() {
        $("#bw-val").text(parseFloat($("#bandwidth").val()).toFixed(2));
        computeMeanshift();
    });

    // kmeans - k
    $("#k").on("input", function() {
        $("#k-val").text(parseInt($("#k").val()));
        computeKmeans();
    });
});

function regenerate() {
  var n = parseInt($("#n-pts").val());
  var eps = parseFloat($("#eps").val());
  var minpts = parseInt($("#minpts").val());
  var metric = $("#metric").val();
  var bw     = parseFloat($("#bandwidth").val());
  var k      = parseInt($("#k").val());
  $("#spinner").show();
  toScilab('regen|' + n + '|' + eps + '|' + minpts + '|' + metric + '|' + bw + '|' + k, function() { 
       $("#spinner").hide(); 
      });
}

function computeDbscan() {
  var eps    = parseFloat($("#eps").val());
  var minpts = parseInt($("#minpts").val());
  var metric = $("#metric").val();
  $("#spinner").show();
  toScilab('dbscan|' + eps + '|' + minpts + '|' + metric,
    function() { $("#spinner").hide(); });
}

function computeMeanshift() {
  var bw = parseFloat($("#bandwidth").val());
  $("#spinner").show();
  toScilab('meanshift|' + bw,
    function() { $("#spinner").hide(); });
}

function computeKmeans() {
  var k = parseInt($("#k").val());
  $("#spinner").show();
  toScilab('kmeans|' + k,
    function() { $("#spinner").hide(); });
}
