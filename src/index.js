const Digits = 2;
const catalog = [
    {text: "管子", child: [
        {text: "管径计算（流速）", url: "dim_v"},
        {text: "管径计算（压降）", url: "dim_dP"},
        {text: "管子重量", url: "pipeWgt_thk"},
        {text: "钢管尺寸系列SH/T3405", url: "pipeSeries_SHT3405"},
        {text: "管子保温", url: "pipeInsultion"},
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

    ithk: "mm",
    Vi: "m3",
    Si:"m2",

    m: "kg/mol",
    p: "MPa(a)",
    t: "C",
    h: "kJ/kg",
    s: "kJ/kg",
    v: "m3/kg",
    rho: "kg/m3",
    u: "kJ/kg",
    cp: "kJ/(kg·K)",
    cv: "kJ/(kg·K)",
    mu: "Pa·s",
    nu: "m2/s",
    k: "W/(m·K)",
    st: "N/m",
    alfav: "1/K",
    kt: "1/MPa",
    w: "m/s"
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

    ithk: "保温厚度",
    Vi: "保温体积",
    Si:"保温外表面积",

    m: "摩尔质量",
    phase: "相态",
    region: "区间",
    p: "压力",
    t: "温度",
    h: "比焓",
    s: "比熵",
    x: "干度",
    v: "比体积",
    rho: "密度",
    u: "内能",
    cp: "定压比热容",
    cv: "定容比热容",
    mu: "动力粘度",
    nu: "运动粘度",
    k: "导热系数",
    st: "表面张力",
    ie: "等熵指数",
    alfav: "膨胀系数",
    kt: "等温压缩性",
    w: "声速",
    epsilon: "介电常数",
    kw: "电离常数"
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
    document.querySelector("#tab-panel").appendChild(createForm(["f","ve"], function () {
        var f = Number(document.getElementsByName("f")[0].value);
        var v = Number(document.getElementsByName("ve")[0].value);
    
        var dim = dim_v(f, v).toFixed(Digits);
        document.querySelector("#tab-panel").appendChild(createRes([{name: "dim", value: dim}]));
    }));
});

window.Router.route("dim_dP",function () {
    document.querySelector("#tab-title").innerHTML = "管径计算（压降）"
    document.querySelector("#tab-panel").innerHTML = '';
    document.querySelector("#tab-panel").appendChild(createForm(["f","dP","rho","nu"], function () {
        var f = Number(document.getElementsByName("f")[0].value);
        var dP = Number(document.getElementsByName("dP")[0].value);
        var rho = Number(document.getElementsByName("rho")[0].value);
        var nu = Number(document.getElementsByName("nu")[0].value);

        var dim = dim_dP(f, dP, rho, nu).toFixed(Digits);
        document.querySelector("#tab-panel").appendChild(createRes([{name: "dim", value: dim}]));
    }));
});

window.Router.route("pipeWgt_thk",function () {
    document.querySelector("#tab-title").innerHTML = "管子重量"
    document.querySelector("#tab-panel").innerHTML = '';
    document.querySelector("#tab-panel").appendChild(createForm(["don","thk","l"], function () {
        var don = Number(document.getElementsByName("don")[0].value);
        var thk = Number(document.getElementsByName("thk")[0].value);
        var l = Number(document.getElementsByName("l")[0].value);

        var wgt = pipeWgt_thk(don, thk).toFixed(Digits);
        var wgtt = l * Number(wgt);
        document.querySelector("#tab-panel").appendChild(createRes([{name: "wgt", value: wgt},{name: "wgtt", value: wgtt}]));
    }));
});

window.Router.route("pipeSeries_SHT3405",function () {
    document.querySelector("#tab-title").innerHTML = "钢管尺寸系列SH/T3405"
    document.querySelector("#tab-panel").innerHTML = '';
    var form = document.createElement("div");
    var p = document.createElement("p");
    var label = document.createElement("label");
    label.innerText = "公称直径";
    p.appendChild(label);
    p.appendChild(createSelect({name: "dn", option: Object.keys(Do)}));
    form.appendChild(p);
    var p = document.createElement("p");
    var label = document.createElement("label");
    label.innerText = "SCH";
    p.appendChild(label);
    p.appendChild(createSelect({name: "sch", option: Sch}));
    form.appendChild(p);
    form.appendChild(createButton("计算", function () {
        //var dn = Number(document.getElementsByName("dn")[0].value);
        var schIndex = document.getElementsByName("sch")[0].selectedIndex;
        var dn= document.getElementsByName("dn")[0].options[document.getElementsByName("dn")[0].selectedIndex].text;
        var thk = SHT3405[dn][schIndex] === undefined ? "不存在该系列" : SHT3405[dn][schIndex];
        document.querySelector("#tab-panel").appendChild(createRes([{name: "don", value: Do[dn]},{name: "thk", value: thk}]));
    }));
    document.querySelector("#tab-panel").appendChild(form);
});

window.Router.route("pipeInsultion",function () {
    document.querySelector("#tab-title").innerHTML = "管子保温"
    document.querySelector("#tab-panel").innerHTML = '';
    document.querySelector("#tab-panel").appendChild(createForm(["don","ithk","l"], function name(params) {
        var don = Number(document.getElementsByName("don")[0].value);
        var ithk = Number(document.getElementsByName("ithk")[0].value);
        var l = Number(document.getElementsByName("l")[0].value);

        var vi = VInsultion(don, ithk, l).toFixed(Digits+2);
        var si = SInsultion(don, ithk, l).toFixed(Digits);
        document.querySelector("#tab-panel").appendChild(createRes([{name: "Vi", value: vi},{name: "Si", value: si}]));
    }));
});

window.Router.route("waterProp",function () {
    document.querySelector("#tab-title").innerHTML = "水蒸气物性"
    document.querySelector("#tab-panel").innerHTML = '';
    var p = document.createElement("p");
    var label = document.createElement("label");
    label.innerText = "已知参数";
    p.appendChild(label)
    p.appendChild(createSelect({name: "mode", option: ["请选择","压力-温度","压力-饱和","温度-饱和","压力-比焓","压力-比熵","比焓-比熵","压力-干度","温度-干度"]},function (e) {
        
        var l = document.getElementById("tab-panel").children.length;
        if (l > 1) {
            for (let index = l - 1; index > 0; index--) {
                document.getElementById("tab-panel").removeChild(document.getElementById("tab-panel").children[index])
            }
        }

        var modes = [
            ['p','t'],
            ['p'],
            ['t'],
            ['p','h'],
            ['p','s'],
            ['h','s'],
            ['p','x'],
            ['t','x']
        ];
        var mode = modes[e.target.selectedIndex - 1];
        document.querySelector("#tab-panel").appendChild(createForm(mode,function (e) {
            var l = document.getElementById("tab-panel").children.length;
            if (l > 2) {
                for (let index = l - 1; index > 1; index--) {
                    document.getElementById("tab-panel").removeChild(document.getElementById("tab-panel").children[index])
                }
            }
        
            var length = mode.length, arg0, arg1;
            
            if (length === 1) {
                arg0 = Number(document.getElementsByName(mode[0])[0].value);
                if (mode[0] === "t") arg0 += 273.15;
                var args = {};
                args[mode[0]] = arg0;
            }else{
                arg0 = Number(document.getElementsByName(mode[0])[0].value);
                arg1 = Number(document.getElementsByName(mode[1])[0].value);
                var w = new IAPWS97();
                if (mode[0] === "t") arg0 += 273.15;
                if (mode[1] === "t") arg1 += 273.15;
                var args = {};
                args[mode[0]] = arg0;
                args[mode[1]] = arg1;
            }
            var w = new IAPWS97();
            var s = w.solve(args);
            var r = [];
            for (const key in propName) {
                if (Object.hasOwnProperty.call(s, key)) {
                    if (s[key] != null) {
                        r.push({name: key, value: s[key]});
                    }
                }
            }
            document.querySelector("#tab-panel").appendChild(createRes(r));
        }));
    }));
    document.querySelector("#tab-panel").appendChild(p);
});

window.Router.route("test", function () {

});

function createInput(data) {
    var input = document.createElement("input");
    input.name = data.name || '';
    //input.id = data.id || '';
    input.type = data.type || "text";
    input.readOnly = data.readOnly || false;
    input.required = data.required || false;
    input.placeholder = data.placeholder || '';
    input.value = data.value || '';

    return input;
}

function createSelect(data, callback) {
    var select = document.createElement("select");
    select.name = data.name || '';
    //select.id = data.id;
    select.required = data.required || false;
    data.option.forEach((el, index) => {
        var option = document.createElement("option");
        option.innerText = Array.isArray(el) ? el[0] : el;
        option.value = Array.isArray(el) ? el[0] : index;
        select.appendChild(option);
    });
    select.addEventListener("change", callback);

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