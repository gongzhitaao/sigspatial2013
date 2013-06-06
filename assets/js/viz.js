---
---
{% include config %}

(function($, d3, Server, undefined){

    var svg;
    var width;
    var height;

    var g_polys = {};
    var g_points = {};

    var ipoly = '10', ipt = '500';

    var xmin, xmax;
    var ymin, ymax;

    var x;
    var y;

    var scaleX, scaleY;

    function init() {

        width = $('div#svg').width();
        height = 600;

        svg = d3.select('div#svg')
            .append('svg:svg')
            .attr('width', width)
            .attr('height', height);

        $('#foo1').click(foo1);
        $('#foo2').click(foo2);

        $('input[name=polyid]').attr('value', 1);
        $('input[name=polyseq]').attr('value', 1);
        $('input[name=ptid]').attr('value', 0);
        $('input[name=ptseq]').attr('value', 501);

        var p1 = {}, p2 = {};
        init_poly(p1, '{{ BASE_PATH }}/data/poly10.txt');
        init_poly(p2, '{{ BASE_PATH }}/data/poly15.txt');
        g_polys = {'10': p1, '15': p2 };

        var pt1 = {}, pt2 = {};
        init_pt(pt1, '{{ BASE_PATH }}/data/points500.txt');
        init_pt(pt2, '{{ BASE_PATH }}/data/points1000.txt');
        g_points = {'500': pt1, '1000': pt2};

        $('#polyfile').change(function(){
            ipoly = $("#polyfile option:selected").val();
        });

        $('#ptfile').change(function(){
            ipt = $('#ptfile option:selected').val();
        });
    }

    function foo1() {

        var polyid = $('input[name=polyid]').val();
        var polyseq = $('input[name=polyseq]').val();

        var ptid = $('input[name=ptid]').val().toString();
        var ptseq = $('input[name=ptseq]').val().toString();

        var xml = $($.parseXML(g_points[ipt][ptid][ptseq]));

        var xy = xml.text().split(/,/).map(parseFloat);
        x = xy[0];
        y = xy[1];

        xml = $($.parseXML(g_polys[ipoly][polyid][polyseq]));

        foo(xml);
    }

    function foo2() {

        x = parseFloat($('input[name=x]').val());
        y = parseFloat($('input[name=y]').val());

        console.log(x, y);

        var xml = $($.parseXML($('#polygml').val()));

        foo(xml);
    }

    function foo(xml) {
        xmin = xmax = x;
        ymin = ymax = y;

        var polys = [];
        xml.find('gml\\:coordinates,coordinates').each(function() {

            var coords = $(this).text().split(/\s+/);
            var poly = [];
            for (var i = 0; i < coords.length-1; ++i) {
                var xy = coords[i].split(',').map(parseFloat);

                if (xy[0] < xmin) xmin = xy[0];
                if (xy[0] > xmax) xmax = xy[0];
                if (xy[1] < ymin) ymin = xy[1];
                if (xy[1] > ymax) ymax = xy[1];

                poly.push({x: xy[0], y: xy[1]});
            }
            polys.push(poly);
        });

        if (xmin > x - 1000) xmin = x - 1000;
        if (xmax < x + 1000) xmax = x + 1000;
        if (ymin > y - 1000) ymin = y - 1000;
        if (ymax < y + 1000) ymax = y + 1000;

        xmin -= 100;
        xmax += 100;
        ymin -= 100;
        ymax += 100;

        var r1 = (xmax-xmin)/(ymax-ymin);
        var r0 = width/height;

        var x0, x1, y0, y1, w, h, m;
        if (r1 > r0) {
            x0 = 0, x1 = width;
            h = width/(xmax-xmin)*(ymax-ymin);
            m = (height-h)/2;
            y0 = h-m, y1 = m;
        } else if (r1 < r0) {
            y0 = height, y1 = 0;
            w = height/(ymax-ymin)*(xmax-xmin);
            m = (width-w)/2;
            x0 = m, x1 = w+m;
        } else {
            x0 = 0, x1 = width;
            y0 = height, y1 = 0;
        }

        scaleX = d3.scale.linear()
            .domain([xmin, xmax]).range([x0, x1]);
        scaleY = d3.scale.linear()
            .domain([ymin, ymax]).range([y0, y1]);

        svg.selectAll('polygon').remove();
        svg.selectAll('circle').remove();

        svg.selectAll('polygon').data(polys)
            .enter().append('polygon')
            .attr('points', function(d) {
                return d.map(function(d){
                    return [scaleX(d.x), scaleY(d.y)].join(',');
                }).join(' ');
            })
            .attr('stroke-width', 1)
            .attr('stroke', 'blue')
            .attr('fill', '#eee');

        console.log(scaleX(xmin), scaleX(xmax));

        svg.append('circle')
            .attr('r', scaleX(1000+x)-scaleX(x))
            .attr('cx', scaleX(x))
            .attr('cy', scaleY(y))
            .attr('fill', '#aaa')
            .attr('opacity', '0.5');

        svg.append('circle')
            .attr('r', 2)
            .attr('cx', scaleX(x))
            .attr('cy', scaleY(y));

    }

    function init_pt(p, f) {

        $.ajax({
            url: f,
            dataType: 'text',

            success: function(d) {

                $.each(d.split(/\n/).filter(Boolean), function(i, v) {
                    var num = /:[0-9]+/g;

                    var id = num.exec(v)[0].substring(1);
                    var seq = num.exec(v)[0].substring(1);

                    if (!(id in p)) p[id] = {};
                    p[id][seq] = v.substring(num.lastIndex+1);

                });
            },

            error: function() {
                console.log("init point error");
            }

        });

    }

    function init_poly(p, f) {

        $.ajax({
            url: f,
            dataType: 'text',

            success: function (d) {

                $.each(d.split(/\n/).filter(Boolean), function(i, v) {
                    var num = /:[0-9]+/g;

                    var id = num.exec(v)[0].substring(1);
                    var seq = num.exec(v)[0].substring(1);

                    if (!(id in p)) p[id] = {};
                    p[id][seq] = v.substring(num.lastIndex+1);

                });
            },

            error: function() {
                console.log("init poly error");
            }

        });

    }

    function coord(gml) {

        $.ajax({
            url: gml,
            dataType: 'text',

            success: function(d) {

                var xmin = x, xmax = x,
                    ymin = y, ymax = y;

                var polys = [];
                var xml = $('Polygon', $.parseXML(d));

                xml.find('coordinates').each(function() {

                    var coords = $(this).text().split(/\s+/);
                    var poly = [];
                    for (var i = 0; i < coords.length-1; ++i) {
                        var xy = coords[i].split(',').map(parseFloat);

                        if (xy[0] < xmin) xmin = xy[0];
                        if (xy[0] > xmax) xmax = xy[0];
                        if (xy[1] < ymin) ymin = xy[1];
                        if (xy[1] > ymax) ymax = xy[1];

                        poly.push({x: xy[0], y: xy[1]});
                    }
                    polys.push(poly);
                });

                if (xmin > x - 1000) xmin = x - 1000;
                if (xmax < x + 1000) xmax = x + 1000;
                if (ymin > y - 1000) ymin = y - 1000;
                if (ymax < y + 1000) ymax = y + 1000;

                xmin -= 100;
                xmax += 100;
                ymin -= 100;
                ymax += 100;

                var scaleX = d3.scale.linear()
                        .domain([xmin, xmax]).range([0, width]);
                var scaleY = d3.scale.linear()
                        .domain([ymin, ymax]).range([height, 0]);

                svg.selectAll('polygon').data(polys)
                    .enter().append('polygon')
                    .attr('points', function(d) {
                        return d.map(function(d){
                            return [scaleX(d.x), scaleY(d.y)].join(',');
                        }).join(' ');
                    })
                    .attr('stroke-width', 1)
                    .attr('stroke', 'blue')
                    .attr('fill', '#eee');

                svg.append('circle')
                    .attr('r', scaleX(1000)-scaleX(0))
                    .attr('cx', scaleX(x))
                    .attr('cy', scaleY(y))
                    .attr('fill', '#aaa')
                    .attr('opacity', '0.5');

                svg.append('circle')
                    .attr('r', 2)
                    .attr('cx', scaleX(x))
                    .attr('cy', scaleY(y));

            },

            error: function(d) {
                alert('error');
            }
        });
    }

    $(document).ready(function(){

        init();

        var server = {
            _conf: {
                method: 'GET',
                remote: "http://localhost:8080/sigspatial2013/rpc"
            }
        };

        InstallFunction(server, 'point');
        server.point(0, 501, function(d){
            console.log(d);
        });

    });

}(jQuery, d3));
