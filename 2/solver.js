import { Pipe } from "./pipe.js";
function Solver() {
    this.methods = {};
}

Solver.prototype.method = function (url, callback) {
    this.methods[url] = callback || function () { };
};

export var solver = new Solver();

solver.method ("pipe_diameter_velocity", function () {
    let pipe = new Pipe();
    pipe.fluid.flowrate_volume = parseFloat(document.getElementsByName("flowrate_volume")[0].value);
    console.log(pipe.fluid.flowrate_volume);
});

solver.method ("pipe_diameter_drop_pressure", function () {
    alert("未选择功能。");
});

solver.method ("pipe_drop_pressure", function () {
    alert("未选择功能。");
});

solver.method ("pipe_weight", function () {
    alert("未选择功能。");
});

solver.method ("insul_pipe_weight", function () {
    alert("未选择功能。");
});

solver.method ("anticorrosion_material", function () {
    alert("未选择功能。");
});

solver.method ("insulation_material", function () {
    alert("未选择功能。");
});

solver.method ("property", function () {
    alert("未选择功能。");
});