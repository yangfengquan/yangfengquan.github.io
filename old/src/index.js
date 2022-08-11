const Digits = 4;
const catalog = [
    {text: "管径[流速]", url: "dim_v"},
    {text: "管径[压降]", url: "dim_dP"},
    {text: "管道阻力", url: "pipePressureDrop"},
    {text: "管子重量", url: "pipeWeight_thk"},
    {text: "钢管尺寸", url: "pipeSeries_SHT3405"},
    {text: "管道绝热[1]", url: "pipeInsultion"},
    {text: "管道绝热[2]", url: "pipeInsultion2"},
    {text: "水蒸汽管道", url: "waterPipe"},
];

const unit = {
    l: "m",

    f: "m3/h",
    f_m: "kg/h",
    ve: "m/s",

    dP: "kPa",
    dp0: "kPa",
    dp1: "kPa",
    dps: "kPa",
    dim: "mm",
    don: "mm",
    thk: "mm",
    wgt: "kg/m",
    wgtt: "kg",

    ithk: "mm",
    Vi: "m3",
    Si:"m2",

    t0: "C",
    ta: "C",
    ts: "C",
    t_A: "C",
    p_A: "MPa(a)",
    t_B: "C",
    p_B: "MPa(a)",
    d1: "mm",
    wv: "m/s",
    qs: "W/m2",
    q: "W/m",
    qf: "W",
    qt: "W",
	rho: "kg/m3",
	mu: "Pa·s",
	nu: "m2/s"
}

const propName = {
    l: "长度",

    f: "流量",
    f_m: "流量",
    ve: "流速",

    dP: "允许压降",
    dp0: "直管阻力降",
    dp1: "局部阻力降",
    dps: "总阻力降",
    dim: "管子内径",
    don: "管子外径",
    thk: "管子壁厚",
    wgt: "管子单重",
    wgtt: "管子总重",

    ithk: "保温厚度",
    Vi: "保温体积",
    Si: "保温外表面积",

    t0: "管道外表面温度",
    ta: "环境温度",
    ts: "保温外表面温度",
    t_A: "始端温度",
    p_A: "始端压力",
    t_B: "末端温度",
    p_B: "末端压力",
    d1: "保温外径",
    wv: "风速",
    qs: "单位表面积热损失",
    q: "单位长度热损失",
    qf: "单个管托热损失",
    qt: "总热损失",
	rho: "密度",
	mu: "动力粘度",
	nu: "运动粘度"
}

function initCatalog(data) {
    
        var ol = document.createElement("ol");
        data.forEach(link => {
            var a = document.createElement("a");
            a.textContent = link.text;
            a.href = '#' + link.url;
            var li = document.createElement("li");
            li.appendChild(a);
            ol.appendChild(li);
        });

    return ol;
}

(function () {
    document.getElementById("catalog").appendChild(initCatalog(catalog));
})();

function createInput(data) {
    var p = document.createElement("p");
    
    var label = document.createElement("label");
    label.innerText = (data.label || propName[data.name] || '') + "\n" + (data.unit || unit[data.name] || '');
    p.appendChild(label);

    var input = document.createElement("input");
    input.name = data.name || '';
    input.type = data.type || "text";
    input.readOnly = data.readOnly || false;
    input.required = data.required || false;
    input.placeholder = data.placeholder || '';
    input.value = data.value || '';
    input.autocomplete = data.autocomplete || "on";
    p.appendChild(input);

    return p;
}

function createSelect(data, callback) {
    var p = document.createElement("p");
    
    var label = document.createElement("label");
    label.innerText = (data.label || propName[data.name] || '') + "\n" + (data.unit || unit[data.name] || '');
    p.appendChild(label);

    var select = document.createElement("select");
    select.name = data.name || '';
    select.required = data.required || false;
    data.option.forEach((el, index) => {
        var option = document.createElement("option");
        option.innerText = Array.isArray(el) ? el[0] : el;
        option.value = Array.isArray(el) ? el[1] : index;
        select.appendChild(option);
    });
    select.addEventListener("change", callback);
    p.appendChild(select);

    return p;
}

function createButton(text, callback) {
    var btn = document.createElement("button");
    btn.innerText = text;
    btn.addEventListener("click", callback);
    return btn;
}

function createRes(res) {
    var p = document.createElement("p");
    var label = document.createElement("label");
    label.innerText = (propName[res.name] || '') + "\n" + (unit[res.name] || '');
    p.appendChild(label);
    var value = document.createElement("span");
    value.innerText = res.value; 
    p.appendChild(value);
    return p;
}

window.Router.route("/", function () {
    //window.location.href = "#changelog";
});

window.Router.route("dim_v",function () {
    document.querySelector("#tab-title").innerHTML = catalog[0].text;
    document.querySelector("#tab-panel").innerHTML = '';

    var form = document.createElement("div");
    ["f","ve"].forEach(el => {
        form.appendChild(createInput({name: el}));
    });

    document.querySelector("#tab-panel").appendChild(form);
    document.querySelector("#tab-panel").appendChild(createButton("计算", function () {
        var f = Number(document.getElementsByName("f")[0].value);
        var v = Number(document.getElementsByName("ve")[0].value);
    
        var dim = dim_v(f, v).toFixed(Digits);

        var resEl = document.createElement("div");
        [{name: "dim", value: dim}].forEach(el => {
            resEl.appendChild(createRes(el));
        })
        document.querySelector("#tab-panel").appendChild(resEl);
    }));
});

window.Router.route("dim_dP",function () {
    document.querySelector("#tab-title").innerHTML =  catalog[1].text
    document.querySelector("#tab-panel").innerHTML = '';
    
    var formWrapper = document.createElement("div");
    formWrapper.className = "formWrapper";

    var form1 = document.createElement("div");
    ["f","l","dP"].forEach(el => {
        form1.appendChild(createInput({name: el}));
    });
    formWrapper.appendChild(form1);

    var form2 = document.createElement("div");
    ["rho","nu"].forEach(el => {
        form2.appendChild(createInput({name: el}));
    });
    formWrapper.appendChild(form2);
    
    document.querySelector("#tab-panel").appendChild(formWrapper);
    document.querySelector("#tab-panel").appendChild(createButton("计算", function () {
        var f = Number(document.getElementsByName("f")[0].value);
        var l = Number(document.getElementsByName("l")[0].value);
        var dP = Number(document.getElementsByName("dP")[0].value);
        var rho = Number(document.getElementsByName("rho")[0].value);
        var nu = Number(document.getElementsByName("nu")[0].value);

        var dim = dim_dP(f, l, dP, rho, nu).toFixed(Digits);

        var resEl = document.createElement("div");
        [{name: "dim", value: dim}].forEach(el => {
            resEl.appendChild(createRes(el));
        })
        document.querySelector("#tab-panel").appendChild(resEl);
    }));
});

window.Router.route("pipePressureDrop",function () {
    document.querySelector("#tab-title").innerHTML =  catalog[2].text
    document.querySelector("#tab-panel").innerHTML = '';

    var formWrapper = document.createElement("div");
    formWrapper.className = "formWrapper";

    var form1 = document.createElement("div");
    form1.appendChild(createSelect({name: "e", label: "管子类别", option: Roughness}));
    ["dim","l","f","rho","mu"].forEach(el => {
        form1.appendChild(createInput({name: el}));
    });
    formWrapper.appendChild(form1);

    var form2 = document.createElement("div");
    Object.keys(LocalResistace).slice(0,6).forEach(el => {
        var label;
        if (PipeFitting.hasOwnProperty(el)) {
            label = PipeFitting[el];
        } else {
            label = Gate[el];
        }
        form2.appendChild(createInput({name: el, label: label, unit: "个"}));
    });
    formWrapper.appendChild(form2);

    var form3 = document.createElement("div");
    Object.keys(LocalResistace).slice(6).forEach(el => {
        var label;
        if (PipeFitting.hasOwnProperty(el)) {
            label = PipeFitting[el];
        } else {
            label = Gate[el];
        }
        form3.appendChild(createInput({name: el, label: label, unit: "个"}));
    });
    formWrapper.appendChild(form3);

    document.querySelector("#tab-panel").appendChild(formWrapper);
    document.querySelector("#tab-panel").appendChild(createButton("计算", function () {
        var e = Number(document.getElementsByName("e")[0].value) / 1000;
        var dim = Number(document.getElementsByName("dim")[0].value) / 1000;
        var f = Number(document.getElementsByName("f")[0].value);
        var l = Number(document.getElementsByName("l")[0].value);
        var rho = Number(document.getElementsByName("rho")[0].value);
        var mu = Number(document.getElementsByName("mu")[0].value);
        var ksum = 0;

        Object.keys(LocalResistace).forEach(el => {
            if (LocalResistace.hasOwnProperty(el)) {
                ksum += Number(document.getElementsByName(el)[0].value) * LocalResistace[el];
            }
        });
        
        var ve = f / 3600 / (Pi * (dim / 2) ** 2);
        var dp0 = pipeDp0(f, e, dim, l, rho, mu);
        var dp1 = pipeDp1(ksum, ve, rho);

        var resEl = document.createElement("div");
        [
            {name: "ve", value: ve},
            {name: "dp0", value: dp0},
            {name: "dp1", value: dp1},
            {name: "dps", value: (dp0 + dp1)}
        ].forEach(el => {
            resEl.appendChild(createRes(el));
        })
        document.querySelector("#tab-panel").appendChild(resEl);
    }));
});

window.Router.route("pipeWeight_thk",function () {
    document.querySelector("#tab-title").innerHTML =  catalog[3].text;
    document.querySelector("#tab-panel").innerHTML = '';

    var form = document.createElement("div");
    ["don","thk","l"].forEach(el => {
        form.appendChild(createInput({name: el}));
    });

    document.querySelector("#tab-panel").appendChild(form);
    document.querySelector("#tab-panel").appendChild(createButton("计算", function () {
        var don = Number(document.getElementsByName("don")[0].value);
        var thk = Number(document.getElementsByName("thk")[0].value);
        var l = Number(document.getElementsByName("l")[0].value);

        var wgt = pipeWgt_thk(don, thk).toFixed(Digits);
        var wgtt = l * Number(wgt);

        var resEl = document.createElement("div");
        [{name: "wgt", value: wgt},{name: "wgtt", value: wgtt}].forEach(el => {
            resEl.appendChild(createRes(el));
        })
        document.querySelector("#tab-panel").appendChild(resEl);
    }));
});

window.Router.route("pipeSeries_SHT3405",function () {
    document.querySelector("#tab-title").innerHTML =  catalog[4].text;
    document.querySelector("#tab-panel").innerHTML = '';

    var form = document.createElement("div");
    form.appendChild(createSelect({name: "dn", label: "公称直径", option: Object.keys(Do)}));
    form.appendChild(createSelect({name: "sch", label: "SCH", option: Sch}));

    document.querySelector("#tab-panel").appendChild(form);
    document.querySelector("#tab-panel").appendChild(createButton("查询", function () {
        var schIndex = document.getElementsByName("sch")[0].selectedIndex;
        var dn= document.getElementsByName("dn")[0].options[document.getElementsByName("dn")[0].selectedIndex].text;
        var thk = SHT3405[dn][schIndex] === undefined ? "不存在该系列" : SHT3405[dn][schIndex];
        
        var resEl = document.createElement("div");
        [{name: "don", value: Do[dn]},{name: "thk", value: thk}].forEach(el => {
            resEl.appendChild(createRes(el));
        })
        document.querySelector("#tab-panel").appendChild(resEl);
    }));
});

window.Router.route("pipeInsultion",function () {
    document.querySelector("#tab-title").innerHTML =  catalog[5].text
    document.querySelector("#tab-panel").innerHTML = '';

    var form = document.createElement("div");
    ["don","ithk","l"].forEach(el => {
        form.appendChild(createInput({name: el}));
    });

    document.querySelector("#tab-panel").appendChild(form);
    document.querySelector("#tab-panel").appendChild(createButton("计算",function name(params) {
        var don = Number(document.getElementsByName("don")[0].value);
        var ithk = Number(document.getElementsByName("ithk")[0].value);
        var l = Number(document.getElementsByName("l")[0].value);

        var vi = VInsultion(don, ithk, l).toFixed(Digits+2);
        var si = SInsultion(don, ithk, l).toFixed(Digits);

        var resEl = document.createElement("div");
        [{name: "Vi", value: vi},{name: "Si", value: si}].forEach(el => {
            resEl.appendChild(createRes(el));
        })
        document.querySelector("#tab-panel").appendChild(resEl);
    }));
});

window.Router.route("pipeInsultion2",function () {
    document.querySelector("#tab-title").innerHTML =  catalog[6].text
    document.querySelector("#tab-panel").innerHTML = '';

    var formWrapper = document.createElement("div");
    formWrapper.className = "formWrapper";

    var form1 = document.createElement("div");
    ["don","d1","l"].forEach(el => {
        form1.appendChild(createInput({name: el}));
    });
    form1.appendChild(createSelect({name: "e",label: "外表面材料", option: BlackDegree}));
    formWrapper.appendChild(form1);

    var form2 = document.createElement("div");
    ["t0","ta","wv"].forEach(el => {
        form2.appendChild(createInput({name: el}));
    });
    var p = document.createElement("p");
    p.innerText = "保温材料导热系数按照硅酸铝及其制品计算。"
    form2.appendChild(p);
    formWrapper.appendChild(form2);
    
    document.querySelector("#tab-panel").appendChild(formWrapper);
    document.querySelector("#tab-panel").appendChild(createButton("计算",function name() {
        var t0 = Number(document.getElementsByName("t0")[0].value);
        var don = Number(document.getElementsByName("don")[0].value) / 1000;
        var d1 = Number(document.getElementsByName("d1")[0].value) / 1000;
        var l = Number(document.getElementsByName("l")[0].value);
        var ta = Number(document.getElementsByName("ta")[0].value);
        var wv = Number(document.getElementsByName("wv")[0].value);
        var e = Number(document.getElementsByName("e")[0].value);

        var ts = ts_delta(t0, ta, don, d1, e, wv);
        var k = getLambda((t0 + ts) / 2);
        var ar = getAlphar(e, ta, ts);
        var ac = getAlphac(wv, d1, ta, ts);
        var a = getAlphas(ar, ac);
        var qs = Qs(t0, ta, don, d1, k, a);
        var q = q_Q(qs, d1);
        var qt = q * l;

        var resEl = document.createElement("div");
        [
            {name: "ts", value: ts.toFixed(Digits)},
			{name: "qs", value: qs.toFixed(Digits)},
            {name: "q", value: q.toFixed(Digits)},
            {name: "qt", value: qt.toFixed(Digits)}
        ].forEach(el => {
            resEl.appendChild(createRes(el));
        })
        document.querySelector("#tab-panel").appendChild(resEl);
    }));
});

window.Router.route("waterPipe",function () {
    document.querySelector("#tab-title").innerHTML =  catalog[7].text
    document.querySelector("#tab-panel").innerHTML = '';

    var formWrapper = document.createElement("div");
    formWrapper.className = "formWrapper";

    var form1 = document.createElement("div");
    form1.appendChild(createSelect({name: "e", label: "管子类别", option: Roughness}));
    ["don","dim","d1","l","hf","lf","n"].forEach(el => {
        if (el === "hf") {
            form1.appendChild(createInput({name: el, label: "管托高度", unit: "mm"}));
        } 
        else if (el === "lf") {
            form1.appendChild(createInput({name: el, label: "管托长度", unit: "mm"}));
        }
        else if (el === "n") {
            form1.appendChild(createInput({name: el, label: "管托数量", unit: "个"}));
        }
        else {
            form1.appendChild(createInput({name: el}));
        }
        
    });
    formWrapper.appendChild(form1);

    var form2 = document.createElement("div");
    ["f_m","t_A","p_A","ta","wv"].forEach(el => {
        form2.appendChild(createInput({name: el}));
    });
    form2.appendChild(createSelect({name: "epsilon",label: "外表面材料", option: BlackDegree}));
    formWrapper.appendChild(form2);

    var form3 = document.createElement("div");
    Object.keys(LocalResistace).slice(0,6).forEach(el => {
        var label;
        if (PipeFitting.hasOwnProperty(el)) {
            label = PipeFitting[el];
        } else {
            label = Gate[el];
        }
        form3.appendChild(createInput({name: el, label: label, unit: "个"}));
    });
    formWrapper.appendChild(form3);

    var form4 = document.createElement("div");
    Object.keys(LocalResistace).slice(6).forEach(el => {
        var label;
        if (PipeFitting.hasOwnProperty(el)) {
            label = PipeFitting[el];
        } else {
            label = Gate[el];
        }
        form4.appendChild(createInput({name: el, label: label, unit: "个"}));
    });
    formWrapper.appendChild(form4);

    document.querySelector("#tab-panel").appendChild(formWrapper);
    document.querySelector("#tab-panel").appendChild(createButton("计算", function () {
        var e = Number(document.getElementsByName("e")[0].value) / 1000;
        var don = Number(document.getElementsByName("don")[0].value) / 1000;
        var dim = Number(document.getElementsByName("dim")[0].value) / 1000;
        var d1 = Number(document.getElementsByName("d1")[0].value) / 1000;
        var l = Number(document.getElementsByName("l")[0].value);
        var f_m = Number(document.getElementsByName("f_m")[0].value);  
        var t_A = Number(document.getElementsByName("t_A")[0].value);
        var p_A = Number(document.getElementsByName("p_A")[0].value);
        var ta = Number(document.getElementsByName("ta")[0].value);
        var wv = Number(document.getElementsByName("wv")[0].value);
        var epsilon = Number(document.getElementsByName("epsilon")[0].value);
        var hf = Number(document.getElementsByName("hf")[0].value) / 1000;
        var lf = Number(document.getElementsByName("lf")[0].value) / 1000;
        var n = Number(document.getElementsByName("epsilon")[0].value);
        var ksum = 0;

        Object.keys(LocalResistace).forEach(el => {
            if (LocalResistace.hasOwnProperty(el)) {
                ksum += Number(document.getElementsByName(el)[0].value) * LocalResistace[el];
            }
        });
        
        
        var w = new IAPWS97();
        var ww = w.solve({t: t_A + 273.15, p: p_A});
        var ve = f_m / ww.rho / 3600 / (Pi * (dim / 2) ** 2);
        //var dp0 = pipeDp0(f_m / ww.rho, e, dim, l, ww.rho, ww.mu);
        //var dp1 = pipeDp1(ksum, ve, ww.rho);
        //console.log((dp0 + dp1) / 1000);
        //var p_B = p_A - (dp0 + dp1) / 1000;
        var re = reynolds(dim, ve, ww.rho, ww.mu);
        //console.log("re",re);
        var res = resistace(re, dim, e);
        //console.log("res",res);
        ksum = kt0(res, l, dim) + ksum;
        var pd1 = pd(ve, ww.v);
        var p_B = p2(pd1,p_A * 1e6,ksum) / 1e6;
        //console.log("pB",p_B);

        var ts = ts_delta(t_A, ta, don, d1, epsilon, wv);
        var t0 = getT0(t_A, ww.h, p_B, ta, don, d1, epsilon, wv, f_m / 3600, l);
        var tss = ts_delta(t0, ta, don, d1, epsilon, wv);
        var k = getLambda((t0 + tss) / 2);
        var ar = getAlphar(epsilon, ta, tss);
        var ac = getAlphac(wv, d1, ta, tss);
        var a = getAlphas(ar, ac);
        var qs = Qs(t0, ta, don, d1, k, a);
        var q = q_Q(qs, d1);
        var qf = qFin(hf, lf, t0, ta, (d1 - don) / 2);
        //console.log(qf);
        var qt = q * l + qf * n;
        //console.log("qt",qt);
        var h_B = getHB(ww.h, f_m / 3600, qt);
        //console.log("hb",h_B);
        var t_B = w.solve({p: p_B, h: h_B}).t - 273.15;


        var resEl = document.createElement("div");
        [
            {name: "ve", value: ve.toFixed(Digits)},
            //{name: "dp0", value: dp0.toFixed(Digits)},
            //{name: "dp1", value: dp1.toFixed(Digits)},
            //{name: "dps", value: (dp0 + dp1).toFixed(Digits)},
            {name: "ts", value: ts.toFixed(Digits)},
            {name: "p_B", value: p_B.toFixed(Digits)},
            {name: "t_B", value: t_B.toFixed(Digits)},
            {name: "qs", value: qs.toFixed(Digits)},
            {name: "q", value: q.toFixed(Digits)},
            {name: "qf", value: qf.toFixed(Digits)},
            {name: "qt", value: qt.toFixed(Digits)}
        ].forEach(el => {
            resEl.appendChild(createRes(el));
        })
        document.querySelector("#tab-panel").appendChild(resEl);
    }));
});

window.Router.route("contact", function () {
    document.querySelector("#tab-title").innerHTML = "联系";
    document.querySelector("#tab-panel").innerHTML = '';

    [
        {name: "主页", content: '<a href="http://yangshu.gitee.io">http://yangshu.gitee.io</a>'},
        {name: "邮箱", content: "yfq000@126.com"},
        {name: "微信公众号", content: "jisuanhao"},
        {name: "问题反馈", content: '<a href="https://gitee.com/yangshu/yangshu/issues">https://gitee.com/yangshu/yangshu/issues</a>'}
    ].forEach(el => {
        var label = document.createElement("label");
        label.innerText = el.name;
        var span = document.createElement("span");
        span.innerHTML = el.content;
        var p = document.createElement("p");
        p.appendChild(label);
        p.appendChild(span);
        document.querySelector("#tab-panel").appendChild(p);
    })
});

window.Router.route("changelog", function () {
    document.querySelector("#tab-title").innerHTML = "更新日志";
    document.querySelector("#tab-panel").innerHTML = '';

    [
        {date: "2021-7-21", content: "水蒸气管道终端压力及方法按照DL/T5054修改。"},
        {date: "2021-7-20", content: "水蒸气计算功能增加管托热损失计算。"},
        {date: "2021-7-19", content: "增加水蒸气计算功能。"},
        {date: "2021-7-18", content: "增加管道热损失计算功能。\n修改输入表单布局"},
        {date: "2021-7-16", content: "修改水蒸气物性计算数值的显示精度。\n修改样式。\n增加管道阻力降功能。"},
        {date: "2021-7-16", content: "测试更新日志功能。"}
    ].forEach(el => {
        var h4 = document.createElement("h4");
        h4.innerText = el.date;
        var p = document.createElement("p");
        p.innerText = el.content;
        document.querySelector("#tab-panel").appendChild(h4);
        document.querySelector("#tab-panel").appendChild(p);
    })
});


window.Router.route("test", function () {

});
