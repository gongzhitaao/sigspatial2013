(function ($, d3) {

    function pip() {
        var vis = d3.select("#pip").append("svg").attr("width", 600).attr("height", 600);
        var scalex = d3.scale.linear().domain([-30, 30]).range([0, 600]);
        var scaley = d3.scale.linear().domain([-30, 30]).range([600, 0]),

        poly = [{ "x": 0, "y": 0 },
                { "x": 10, "y": -5 },
                { "x": 18, "y": 7 },
                { "x": 8, "y": 20 },
                { "x": -10, "y": 12 },
                { "x": -12, "y": 2 },
                { "x": -3, "y": -8 },
        ];

        point = [{ "x": -23, "y": 20 },
                 { "x": 0, "y": 0 },
                 { "x": 5, "y": 10 }
        ];

        vis.append("svg:polygon").data([poly])
            .attr("points", function (d) {
                return d.map(function (d) {
                    return [scalex(d.x), scaley(d.y)].join(",");
                }).join(" ");
            })
            .attr("stroke", "white")
            .attr("stroke-width", 2)
            .attr("fill-opacity", 0);

        vis.selectAll("circle").data(point)
            .enter().append("svg:circle")
            .attr("cx", function (d) { return scalex(d.x); })
            .attr("cy", function (d) { return scaley(d.y); })
            .attr("r", 8)
            .attr("fill", "yellow");
        ;
    }

    function pip_sol() {
        var vis = d3.select("#pip-sol").append("svg")
            .attr("width", 500).attr("height", 500);
        var scalex = d3.scale.linear().domain([-30, 30]).range([0, 500]);
        var scaley = d3.scale.linear().domain([-30, 30]).range([500, 0]);

        var line = [{ x1: 0, y1: 5, x2: 10, y2: -10, type: "-2" },
                    { x1: 18, y1: -6, x2: 25, y2: 0, type: "1" },
                    { x1: -10, y1: -8, x2: 0, y2: 5, type: "2" },
                    { x1: 10, y1: -10, x2: 18, y2: -6, type: "0" },
                    { x1: 25, y1: 0, x2: 8, y2: 20, type: "1" },
                    { x1: 8, y1: 20, x2: -10, y2: 12, type: "0" },
                    { x1: -10, y1: 12, x2: -20, y2: 0, type: "-1" },
                    { x1: -20, y1: 0, x2: -10, y2: -8, type: "-1" }];

        var point = [{ "x": 0, "y": 0 }];

        var axes = [{ "x1": -28, "y1": 0, "x2": 28, "y2": 0 },
                { "x1": 0, "y1": -28, "x2": 0, "y2": 28 }
        ];

        vis.append("svg:defs").append("svg:marker")
            .attr("id", "arrow")
            .attr("viewBox", "0 0 10 10")
            .attr("refX", 10).attr("refY", 5)
            .attr("markerWidth", 8)
            .attr("markerHeight", 6)
            .attr("orient", "auto")
            .attr("position", "20%")
            .append("path")
            .attr("d", "M 0 0 L 10 5 L 0 10 z")
            .attr("fill", "white");

        vis.selectAll("line").data(axes)
            .enter().append("svg:line")
            .attr("x1", function (d) { return scalex(d.x1); })
            .attr("y1", function (d) { return scaley(d.y1); })
            .attr("x2", function (d) { return scalex(d.x2); })
            .attr("y2", function (d) { return scaley(d.y2); })
            .attr("stroke", "white").attr("stroke-width", 2)
            .attr("marker-end", "url(#arrow)");

        var g = vis.append("svg:g");

        var c = d3.scale.linear()
            .domain([-2, -1, 0, 1, 2])
            .range(["maroon", "red", "white", "lightgreen", "green"]);

        g.selectAll("line").data(line)
            .enter().append("svg:line")
            .attr("x1", function (d) { return scalex(d.x1); })
            .attr("y1", function (d) { return scaley(d.y1); })
            .attr("x2", function (d) { return scalex(d.x2); })
            .attr("y2", function (d) { return scaley(d.y2); })
            .attr("stroke", function (d) { return c(d.type); })
            .attr("stroke-width", 2)
            .attr("marker-end", "url(#arrow)");

        vis.append("svg:circle").data(point)
            .attr("cx", function (d) { return scalex(d.x); })
            .attr("cy", function (d) { return scaley(d.y); })
            .attr("r", 8)
            .attr("fill", "yellow");
    }

    function pnp() {
        var vis = d3.select("#pnp").append("svg")
                    .attr("width", 600).attr("height", 600);
        var scalex = d3.scale.linear().domain([-30, 30]).range([0, 600]);
        var scaley = d3.scale.linear().domain([-30, 30]).range([600, 0]);

        var rect = [{ x: 15, y: 15, w: 10, h: 7 },
                    { x: -20, y: 4, w: 14, h: 13 },
                    { x: -15, y: 20, w: 7, h: 12 },
        ];

        var circ = [{ x: 0, y: 0, r: 15, f: "none" },
                    { x: 0, y: 0, r: .5, f: "green" }
        ];

        vis.selectAll("rect").data(rect)
            .enter().append("svg:rect")
            .attr("x", function (d) { return scalex(d.x); })
            .attr("y", function (d) { return scaley(d.y); })
            .attr("width", function (d) { return scalex(d.w) - scalex(0) })
            .attr("height", function (d) { return scalex(d.h) - scaley(0) })
            .attr("stroke", "white").attr("fill", "none").attr("stroke-width", 2);

        vis.selectAll("circle").data(circ)
            .enter().append("svg:circle")
            .attr("cx", function (d) { return scalex(d.x); })
            .attr("cy", function (d) { return scaley(d.y); })
            .attr("r", function (d) { return scalex(d.r) - scalex(0); })
            .attr("stroke", "yellow").attr("stroke-width", 2)
            .attr("fill", function (d) { return d.f; });
    }

    function pnp_sol() {
        var vis = d3.select("#pnp-sol").append("svg")
                    .attr("width", 600).attr("height", 600);
        var scalex = d3.scale.linear().domain([-30, 30]).range([0, 600]);
        var scaley = d3.scale.linear().domain([-30, 30]).range([600, 0]);

        poly = [{ "x": 0, "y": 0 },
                { "x": 10, "y": -5 },
                { "x": 18, "y": 7 },
                { "x": 8, "y": 20 },
                { "x": -10, "y": 12 },
                { "x": -12, "y": 2 },
                { "x": -3, "y": -8 },
        ];

        var circ = [{ x: -15, y: 20, r: .5, f: "yellow" }, ];

        vis.selectAll("circle").data(circ)
            .enter().append("svg:circle")
            .attr("cx", function (d) { return scalex(d.x); })
            .attr("cy", function (d) { return scaley(d.y); })
            .attr("r", function (d) { return scalex(d.r) - scalex(0); })
            .attr("stroke", "yellow").attr("stroke-width", 2)
            .attr("fill", function (d) { return d.f; });

        vis.append("svg:polygon").data([poly])
            .attr("points", function (d) {
                return d.map(function (d) {
                    return [scalex(d.x), scaley(d.y)].join(",");
                }).join(" ");
            })
            .attr("stroke", "white")
            .attr("stroke-width", 2)
            .attr("fill-opacity", 0);
    }

    $(document).ready(function () {
        pip();
        pip_sol();
        pnp();
        pnp_sol();
    });
})(jQuery, d3);