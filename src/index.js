const catalog = [
    {text: "管子", child: [
        {text: "管径计算（流速）", url: "dim_v"},
        {text: "管径计算（压降）", url: "dim_dP"},
        {text: "管子重量", url: "pipeWgt_thk"}
    ]},
    {text: "水蒸气", child: [
        {text: "物性", url: "waterProp"}
    ]}
];

const unit = {
    l: "m",

    f: "m3/h",
    ve: "m/s",

    dP: "kPa/100m",
    dim: "mm",
    don: "mm",
    thk: "mm",
    wgt: "kg/m",
    wgtt: "kg",

    p: "MPa(a)",
    t: "C",
    h: "kJ/kg",
    s: "kJ/kg",
    rho: "kg/m3",
    nu: "m2/s"
}

const propName = {
    l: "长度",

    f: "流量",
    ve: "流速",

    dP: "允许压降",
    dim: "管子内径",
    don: "管子外径",
    thk: "管子壁厚",
    wgt: "管子单重",
    wgtt: "管子总重",

    p: "压力",
    t: "温度",
    h: "焓值",
    s: "熵",
    x: "干度",
    rho: "密度",
    nu: "运动粘度"
}
function createInput(data) {
    var input = document.createElement("input");
    input.name = data.name || '';
    input.id = data.id || '';
    input.type = data.type || "text";
    input.readOnly = data.readOnly || false;
    input.required = data.required || false;
    input.placeholder = data.placeholder || '';
    input.value = data.value || '';

    return input;
}

function createSelect(data, callback) {
    var select = document.createElement("select");
    select.name = data.name;
    select.id = data.id;
    select.required = data.required;
    data.option.forEach((el) => {
        var option = document.createElement("option");
        option.innerText = option[0];
        option.value = option[1];
        select.appendChild(option);
    });

    return select;
}

function createButton(text, callback) {
    var btn = document.createElement("button");
    btn.innerText = text;
    btn.addEventListener("click", callback);
    return btn;
}

function createForm(data, callback) {
    var inForm = document.createElement("div");
    data.forEach((el) => {
        var p = document.createElement("p");
        var label = document.createElement("label");
        label.innerText = (propName[el] || '') + "\n" + (unit[el] || '');
        p.appendChild(label);
        p.appendChild(createInput({name: el}));
        inForm.appendChild(p);
    });
    inForm.appendChild(createButton("计算",callback))
    return inForm;
}

function createRes(res) {
    var resEl = document.createElement("div");
    res.forEach(el => {
        var p = document.createElement("p");
        var label = document.createElement("label");
        label.innerText = (propName[el.name] || '') + "\n" + (unit[el.name] || '');
        p.appendChild(label);
        var value = document.createElement("span");
        value.innerText = el.value; 
        p.appendChild(value);
        resEl.appendChild(p);
    });

    return resEl;
}

function dim_v_run() {
    var f = Number(document.getElementsByName("f")[0].value);
    var v = Number(document.getElementsByName("ve")[0].value);

    var dim = dim_v(f, v).toFixed(2);
    document.querySelector("#tab-panel").appendChild(createRes([{name: "dim", value: dim}]))
}

function dim_dP_run() {
    var f = Number(document.getElementsByName("f")[0].value);
    var dP = Number(document.getElementsByName("dP")[0].value);
    var rho = Number(document.getElementsByName("rho")[0].value);
    var nu = Number(document.getElementsByName("nu")[0].value);

    var dim = dim_dP(f, dP, rho, nu).toFixed(2);
    document.querySelector("#tab-panel").appendChild(createRes([{name: "dim", value: dim}]))
}

function pipeWgt_thk_run() {
    var don = Number(document.getElementsByName("don")[0].value);
    var thk = Number(document.getElementsByName("thk")[0].value);
    var l = Number(document.getElementsByName("l")[0].value);

    var wgt = pipeWgt_thk(don, thk).toFixed(2);
    var wgtt = l * Number(wgt);
    document.querySelector("#tab-panel").appendChild(createRes([{name: "wgt", value: wgt},{name: "wgtt", value: wgtt}]))
}
function initCatalog(data) {
    var ul = document.createElement("ul");
    
    data.forEach(plist => {
       
        var ol = document.createElement("ol");
        plist.child.forEach(clist => {
            var a = document.createElement("a");
            a.textContent = clist.text;
            a.href = '#' + clist.url;
            var li = document.createElement("li");
            li.appendChild(a);
            ol.appendChild(li);
        });
        
        var li = document.createElement("li");
        li.innerText = plist.text;
        li.appendChild(ol);
        ul.appendChild(li);
    });
    
    return ul;
}

function home() {
    document.getElementById("catalog").innerHTML = '';
    document.getElementById("catalog").appendChild(initCatalog(catalog));
};

home();

window.Router.route("/", function () {
    home();
});

window.Router.route("dim_v",function () {
    document.querySelector("#tab-title").innerHTML = "管径计算（流速）"
    document.querySelector("#tab-panel").innerHTML = '';
    document.querySelector("#tab-panel").appendChild(createForm(["f","ve"], dim_v_run));
});

window.Router.route("dim_dP",function () {
    document.querySelector("#tab-title").innerHTML = "管径计算（压降）"
    document.querySelector("#tab-panel").innerHTML = '';
    document.querySelector("#tab-panel").appendChild(createForm(["f","dP","rho","nu"], dim_dP_run));
});
window.Router.route("pipeWgt_thk",function () {
    document.querySelector("#tab-title").innerHTML = "管子重量"
    document.querySelector("#tab-panel").innerHTML = '';
    document.querySelector("#tab-panel").appendChild(createForm(["don","thk","l"], pipeWgt_thk_run));
});
window.Router.route("test", function () {

});