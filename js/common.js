"use strict";
var __extends = (this && this.__extends) || (function () {
    var extendStatics = function (d, b) {
        extendStatics = Object.setPrototypeOf ||
            ({ __proto__: [] } instanceof Array && function (d, b) { d.__proto__ = b; }) ||
            function (d, b) { for (var p in b) if (Object.prototype.hasOwnProperty.call(b, p)) d[p] = b[p]; };
        return extendStatics(d, b);
    };
    return function (d, b) {
        extendStatics(d, b);
        function __() { this.constructor = d; }
        d.prototype = b === null ? Object.create(b) : (__.prototype = b.prototype, new __());
    };
})();
var Pi = 3.1415926;
var mlist = ["getDimByV", "getDimByDropP", "getDimBySCH", "getWeight", "getInsultionVandS", "water"];
var mnlist = ["管径初选-选定流速", "管径初选-选定压降", "管道尺寸查询-SH/T3406", "管材重量", "管道保温", "水和水蒸汽物性"];
var Yang = /** @class */ (function () {
    function Yang(initValue) {
        this.properties = {};
        this.inputs = [];
    }
    return Yang;
}());
var Pipe = /** @class */ (function (_super) {
    __extends(Pipe, _super);
    function Pipe(mm, initValue) {
        if (initValue === void 0) { initValue = {}; }
        var _this = _super.call(this, initValue) || this;
        _this.dn = ''; //公称直径
        _this.do = 0; //管道外径
        _this.dim = 0; //管道内径
        _this.f = 0; //流量
        _this.v = 0; //流速
        _this.rho = 0; //密度
        _this.visc = 0; //运动粘度
        _this.dropp = 0; //压降
        _this.sch = ''; //SCH                                                 
        _this.thk = 0; //管道壁厚
        _this.pw = 0; //管道单重
        _this.wgt = 0; //管道重
        _this.ithk = 0; //保温厚
        _this.iv = 0; //保温体积
        _this.is = 0; //保温外表面积
        _this.do1 = 0; //伴热管外径
        _this.l = 0; //长度
        _this.properties = {}; //参数特性
        _this.inputs = []; //输入参数
        _this.mm = ''; //模式
        _this._sch = ["SCH5", "SCH10", "SCH20", "SCH30", "SCH40", "SCH60", "SCH80", "SCH100", "SCH120", "SCH140", "SCH160", "STD", "XS", "XXS", "SCH5S", "SCH10S", "SCH40S", "SCH80S"];
        _this._do = {
            DN6: 10.3,
            DN8: 13.7,
            DN10: 17.1,
            DN15: 21.3,
            DN20: 26.7,
            DN25: 33.4,
            DN32: 42.2,
            DN40: 48.3,
            DN50: 60.3,
            DN65: 73,
            DN80: 88.9,
            DN90: 101.6,
            DN100: 114.3,
            DN125: 141.3,
            DN150: 168.3,
            DN200: 219.1,
            DN250: 273.1,
            DN300: 323.9,
            DN350: 355.6,
            DN400: 406.4,
            DN450: 457,
            DN500: 508,
            DN550: 559,
            DN600: 610,
        };
        _this._thk = {
            //["SCH5","SCH10","SCH20","SCH30","SCH40","SCH60","SCH80","SCH100","SCH120","SCH140","SCH160","STD","XS","XXS","SCH5S","SCH10S","SCH40S","SCH80S"]
            DN6: [, 1.24, , 1.45, 1.73, , 2.41, , , , , 1.73, 2.41, , , 1.24, 1.73, 2.41],
            DN8: [, 1.65, , 1.85, 2.24, , 3.02, , , , , 2.24, 3.02, , , 1.65, 2.24, 3.02],
            DN10: [, 1.65, , 1.85, 2.31, , 3.2, , , , , 2.31, 3.2, , , 1.65, 2.31, 3.2],
            DN15: [1.65, 2.11, , 2.41, 2.77, , 3.73, , , , 4.78, 2.77, 3.73, 7.47, 1.65, 2.11, 2.77, 3.73],
            DN20: [1.65, 2.11, , 2.41, 2.87, , 3.91, , , , 5.56, , , 7.82, , , 2.87, 3.91],
            DN25: [1.65, 2.77, , 2.91, 3.38, , 4.55, , , , 6.35, 3.38, 4.55, 9.09, 1.65, 2.77, 3.38, 4.55],
            DN32: [1.65, 2.77, , 2.97, 3.56, , 4.85, , , , 6.35, 3.56, 4.85, 9.7, 1.65, 2.77, 3.56, 4.85],
            DN40: [1.65, 2.77, , 3.18, 3.68, , 5.08, , , , 7.14, 3.68, 5.08, 10.15, 1.65, 2.77, 3.68, 5.08],
            DN50: [1.65, 2.77, , 3.18, 3.91, , 5.54, , , , 8.74, 3.91, 5.54, 11.07, 1.65, 2.77, 3.91, 5.54],
            DN65: [2.11, 3.05, , 4.78, 5.16, , 7.01, , , , 9.53, 5.16, 7.01, 14.02, 2.11, 3.05, 5.16, 7.01],
            DN80: [2.11, 3.05, , 4.78, 5.49, , 7.62, , , , 11.13, 5.49, 7.62, 15.24, 2.11, 3.05, 5.49, 7.62],
            DN90: [2.11, 3.05, , 4.78, 5.74, , 8.08, , , , , , , , 2.11, 3.05, 5.74, 8.08],
            DN100: [2.11, 3.05, , 4.78, 6.02, , 8.56, , 11.13, , 13.49, 6.02, 8.56, 17.12, 2.11, 3.05, 6.02, 8.56],
            DN125: [2.77, 3.4, , , 6.55, , 9.53, , 12.7, , 15.88, 6.55, 9.53, 19.05, 2.77, 3.4, 6.55, 9.53],
            DN150: [2.77, 3.4, , , 7.11, , 10.97, , 12.7, , 18.26, 7.11, 10.97, 19.05, 2.77, 3.4, 7.11, 10.97],
            DN200: [2.77, 3.76, 6.35, 7.04, 8.18, 10.13, 12.7, 15.09, 18.26, 20.62, 23.01, 8.18, 12.7, 22.23, 2.77, 3.76, 8.18, 12.7],
            DN250: [3.4, 4.19, 6.35, 7.8, 9.27, 12.7, 15.09, 18.26, 21.44, 25.4, 28.58, 9.27, 12.7, 25.4, 3.4, 4.19, 9.27, 12.7],
            DN300: [3.96, 4.57, 6.35, 8.38, 10.31, 14.27, 17.48, 21.44, 25.4, 28.58, 33.32, 9.53, 12.7, 25.4, 3.96, 4.78, 9.53, 12.7],
            DN350: [3.96, 6.35, 7.92, 9.53, 11.13, 15.09, 19.05, 23.83, 27.79, 31.75, 35.71, 9.53, 12.7, , 3.96, 4.78, 9.53, 12.7],
            DN400: [4.19, 6.35, 7.92, 9.53, 12.7, 16.66, 21.44, 26.19, 30.96, 36.53, 40.49, 9.53, 12.7, , 4.19, 4.78, 9.53, 12.7],
            DN450: [4.19, 6.35, 7.92, 11.13, 14.27, 19.05, 23.83, 29.36, 34.93, 39.67, 45.24, 9.53, 12.7, , 4.19, 4.78, 9.53, 12.7],
            DN500: [4.78, 6.35, 9.53, 12.7, 15.09, 20.62, 26.19, 32.54, 38.1, 44.45, 50.01, 9.53, 12.7, , 4.78, 5.54, 9.53, 12.7],
            DN550: [4.78, 6.35, 9.53, 12.7, , 22.23, 28.58, 34.93, 41.28, 47.63, 53.98, 9.53, 12.7, , 4.78, 5.54, 9.53, 12.7],
            DN600: [5.54, 6.35, 9.53, 14.27, 17.48, 24.61, 30.96, 38.89, 46.02, 52.37, 59.54, 9.53, 12.7, , 5.54, 6.35, 9.53, 12.7],
        };
        _this.mm = mm;
        switch (mm) {
            case "getDimByV":
                _this.inputs = ["f", "v"];
                _this.properties = {
                    f: { name: "流量", unit: "m3/h" },
                    v: { name: "流速", unit: "m/s" },
                    dim: { name: "管道内径", unit: "mm" }
                };
                break;
            case "getDimByDropP":
                _this.inputs = ["f", "dropp", "rho", "visc"];
                _this.properties = {
                    f: { name: "流量", unit: "m3/h" },
                    dropp: { name: "允许压降", unit: "kPa/100m" },
                    rho: { name: "介质密度", unit: "kg/m3" },
                    visc: { name: "运动粘度", unit: "mm2/s" },
                    dim: { name: "管道内径", unit: "mm" }
                };
                break;
            case "getDimBySCH":
                _this.inputs = ["dn", "sch", "l"];
                _this.properties = {
                    dn: { name: "公称直径", opt: {} },
                    sch: { name: "SCH", opt: {} },
                    l: { name: "长度", unit: "m" },
                    do: { name: "管道外径", unit: "mm" },
                    thk: { name: "壁厚", unit: "mm" },
                    pw: { name: "单重", unit: "kg/m" },
                    wgt: { name: "总重", unit: "kg" }
                };
                Object.keys(_this._thk).forEach(function (v) {
                    _this.properties["dn"].opt[v] = v;
                });
                _this._sch.forEach(function (v) {
                    _this.properties["sch"].opt[v] = v;
                });
                break;
            case "getWeight":
                _this.inputs = ["do", "thk", "l"];
                _this.properties = {
                    do: { name: "管道外径", unit: "mm" },
                    thk: { name: "壁厚", unit: "mm" },
                    l: { name: "长度", unit: "m" },
                    pw: { name: "单重", unit: "kg/m" },
                    wgt: { name: "总重", unit: "kg" }
                };
                break;
            case "getInsultionVandS": //保温材料体积及外表面积
                _this.inputs = ["do", "ithk", "l"];
                _this.properties = {
                    do: { name: "管道外径", unit: "mm" },
                    ithk: { name: "保温厚度", unit: "mm" },
                    l: { name: "长度", unit: "m" },
                    iv: { name: "保温材料体积", unit: "m3" },
                    is: { name: "保温外表面积", unit: "m2" }
                };
                break;
            default:
                ;
        }
        Object.keys(initValue).forEach(function (key) {
            _this[key] = initValue[key];
        });
        return _this;
    }
    Pipe.prototype.getDimByV = function () {
        this.dim = 18.8 * Math.sqrt(this.f / this.v);
    };
    Pipe.prototype.getDimByDropP = function () {
        this.dim = 11.4 * Math.pow(this.rho, 0.207) * Math.pow(this.visc, 0.033) * Math.pow(this.f, 0.38) * Math.pow(this.dropp, -0.207);
    };
    Pipe.prototype.getDimBySCH = function () {
        this.do = this._do[this.dn];
        this.thk = this._thk[this.dn][this._sch.indexOf(this.sch)];
        this.getWeight();
    };
    Pipe.prototype.getWeight = function (rho) {
        if (rho === void 0) { rho = 7.85; }
        this.pw = Pi * rho * (this.do - this.thk) * this.thk / 1000;
        this.wgt = this.pw * this.l;
        //console.log("do",this.do,"thk",this.thk,"pw",this.pw,"l",this.l);        
    };
    /**
     * @param {钢管外径 mm} do
     * @param {保温厚度 mm} ithk
     * @return 保温体积 m3
     */
    Pipe.prototype.getVOfInsultion = function () {
        this.iv = Pi * (this.do + 1.033 * this.ithk) * 1.033 * this.ithk / 1e6 * this.l;
    };
    /**
     * @param {钢管外径 mm} do
     * @param {保温厚度 mm} ithk
     * @return 保温表面积 m2
     */
    Pipe.prototype.getSOfInsultion = function () {
        this.is = Pi * (this.do + 2.1 * this.ithk + 8.2) / 1e3 * this.l;
    };
    /**
     * 单管伴热或双管伴热（管径相同，夹角小于90°）
     * @param {主管管外径 mm} do
     * @param {伴热管外径 mm} do1
     * @return 伴热管道综合外径 mm
     */
    Pipe.prototype.getDHasHeatingtube = function () {
        return this.do + this.do1 + 10;
    };
    /**
     * 单管伴热或双管伴热（管径相同，夹角大于90°）
     * @param {主管管外径 mm} do
     * @param {伴热管外径 mm} do1
     * @return 伴热管道综合外径 mm
     */
    Pipe.prototype.getdHasHeatingtube = function () {
        return this.do + 1.5 * this.do1 + 10;
    };
    return Pipe;
}(Yang));
var State2 = /** @class */ (function (_super) {
    __extends(State2, _super);
    function State2(initValue) {
        if (initValue === void 0) { initValue = {}; }
        var _this = _super.call(this, initValue) || this;
        _this.name = ''; //物料名称
        _this.m = 0; //摩尔质量，g/mole
        _this.phase = ''; //相态
        _this.t = 0; //温度，K
        _this.p = 0; //压力，Pa
        _this.x = 0; //干度
        _this.rho = 0; //密度，kg/m3
        _this.v = 0; //比体积，m3/kg
        _this.h = 0; //比焓，kJ/kg
        _this.s = 0; //比熵，kJ/kg.K
        _this.u = 0; //内能，kJ/kg
        _this.cp = 0; //定压比热容，kJ/kg.K
        _this.cv = 0; //定容比热容，kJ/kg.K
        _this.mu = 0; //动力粘度，Pa.s
        _this.nu = 0; //运动粘度，m2/s
        _this.k = 0; //导热系数，W/m2.K
        _this.st = 0; //表面张力，N/m(单位待核实)
        _this.ie = 0; //等熵指数
        _this.alfav = 0; //膨胀系数，1/K
        _this.kt = 0; //等温压缩性，1/MPa(单位待核实)
        _this.w = 0; //声速，m/s
        _this.epsilon = 0; //介电常数
        _this.ic = 0; //电离常数
        _this.region = 0; //区间
        _this.properties = {
            name: {
                name: "名称",
                ename: "name"
            },
            m: {
                name: "摩尔质量",
                ename: "Molar mass",
                unit: "g/mole"
            },
            phase: {
                name: "相态",
                ename: "Phase"
            },
            t: {
                name: "温度",
                ename: "Temperature",
                unit: "℃"
            },
            p: {
                name: "压力",
                ename: "Pressure",
                unit: "MPa"
            },
            x: {
                name: "干度",
                ename: "General quantity"
            },
            rho: {
                name: "密度",
                ename: "Density",
                unit: "kg/m3"
            },
            v: {
                name: "比体积",
                ename: "Specific Volume",
                unit: "m3/kg"
            },
            h: {
                name: "比焓",
                ename: "Specific Enthalpy",
                unit: "kJ/kg"
            },
            s: {
                name: "比熵",
                ename: "Specific Entropy",
                unit: "kJ/(kg·K)"
            },
            u: {
                name: "内能",
                ename: "Specific internal energy",
                unit: "kJ/kg"
            },
            cp: {
                name: "定压比热容",
                ename: "Specific Isobaric Heat Capacity",
                unit: "kJ/(kg·K)"
            },
            cv: {
                name: "定容比热容",
                ename: "Specific Isochoric Heat Capacity",
                unit: "kJ/(kg·K)"
            },
            mu: {
                name: "动力粘度",
                ename: "Dynamic Viscosity",
                unit: "Pa·s"
            },
            nu: {
                name: "运动粘度",
                ename: "Kinematic  Viscosity",
                unit: "m2/s"
            },
            k: {
                name: "导热系数",
                ename: "Thermal Conductivity",
                unit: "W/(m·K)"
            },
            st: {
                name: "表面张力",
                ename: "Surface Tension",
                unit: "N/m"
            },
            ie: {
                name: "等熵指数",
                ename: "Isoentropic exponent"
            },
            alfav: {
                name: "膨胀系数",
                ename: "Cubic expansion coefficient",
                unit: "1/K"
            },
            kt: {
                name: "等温压缩性",
                zname: "Isothermal compressibility",
                unit: "1/MPa"
            },
            w: {
                name: "声速",
                ename: "Speed of sound",
                unit: "m/s"
            },
            epsilon: {
                name: "介电常数",
                ename: "Dielectric Constant"
            },
            ic: {
                name: "电离常数",
                ename: "Ionisation constant"
            },
            region: {
                name: "区间",
                ename: "region"
            }
        };
        _this.inputs = ["p", "t", "h", "s", "x"];
        _this.mm = "";
        Object.keys(initValue).forEach(function (key) {
            _this[key] = initValue[key];
        });
        return _this;
    }
    return State2;
}(Yang));
var Mixer = /** @class */ (function (_super) {
    __extends(Mixer, _super);
    function Mixer() {
        var _this = _super !== null && _super.apply(this, arguments) || this;
        _this.im1 = 0; //进入介质1流量
        _this.ip1 = 0; //进入介质1压力
        _this.it1 = 0; //进入介质1温度
        _this.ih1 = 0; //进入介质1比焓
        _this.im2 = 0; //进入介质2流量
        _this.ip2 = 0; //进入介质2压力
        _this.it2 = 0; //进入介质2温度
        _this.ih2 = 0; //进入介质2比焓
        _this.om = 0; //流出介质流量
        _this.op = 0; //流出介质压力
        _this.ot = 0; //流出介质温度
        _this.oh = 0; //流出介质比焓
        _this.properties = {};
        _this.inputs = [];
        return _this;
    }
    return Mixer;
}(Yang));
var Yangw = /** @class */ (function (_super) {
    __extends(Yangw, _super);
    function Yangw() {
        var _this = _super !== null && _super.apply(this, arguments) || this;
        _this.dim = NaN; //管道直径
        _this.f = NaN; //流量
        _this.v = ''; //流速
        _this.w = '';
        _this.h = true;
        _this.g = '';
        _this.properties = {
            dim: { name: "公称直径", unit: "mm" },
            f: { name: "流量", unit: "m3/h" },
            v: { name: "流速", opt: { "gg": "dgggf", "dgggf": 2 }, unit: "m/s" },
            w: { name: "流速", opt: { "gg": "df", "df": 2, "deff": 2, "dfeff": 2 }, unit: "m/s" },
            h: { name: "是否" },
            g: { name: "对吗" }
        };
        _this.inputs = ["dim", "f", "g"];
        return _this;
    }
    return Yangw;
}(Yang));
function initSiderbar(mlist, mnlist) {
    var html = document.getElementById("sidebar");
    if (html == null) {
        alert("Sidebar元素获取失败");
    }
    else {
        //网站header
        var h1 = document.createElement("h1");
        var a = document.createElement("a");
        a.text = "Yang.shu";
        a.href = "./";
        h1.appendChild(a);
        html.appendChild(h1);
        // 网站目录
        var listTitle = document.createElement("h2");
        listTitle.textContent = "目录";
        html.appendChild(listTitle);
        var ol = document.createElement("ol");
        mlist.forEach(function (value, index, array) {
            var li = document.createElement("li");
            var link = document.createElement("a");
            link.addEventListener("click", function () { load(value); });
            link.text = mnlist[index];
            link.href = "javascript:void(0);";
            li.appendChild(link);
            ol.appendChild(li);
        });
        html.appendChild(ol);
        //网站 footer
        var footerTitle = document.createElement("h2");
        footerTitle.textContent = "其它";
        html.appendChild(footerTitle);
        var ul = document.createElement("ul");
        var mail = document.createElement("li");
        var mailLink = document.createElement("a");
        mailLink.text = "错误反馈";
        mailLink.href = "mailto: yangfengquan@126.com";
        mail.appendChild(mailLink);
        ul.appendChild(mail);
        var about = document.createElement("li");
        var aboutLink = document.createElement("a");
        aboutLink.text = "关于";
        aboutLink.href = "./about.html";
        about.appendChild(aboutLink);
        ul.appendChild(about);
        html.appendChild(ul);
    }
}
function initContent(m) {
    var html = document.getElementById("content");
    //var yang = new Yang();
    if (html == null) {
        //location.reload();
        alert("Content 元素获取失败");
    }
    else {
        html.innerHTML = '';
        //模块标题
        var mTitle = document.createElement("h1");
        mTitle.textContent = mnlist[mlist.indexOf(m.mm)];
        html.appendChild(mTitle);
        //创建运行按钮
        var navUl = document.createElement("ul");
        var navLi = document.createElement("li");
        var runBtn = document.createElement("input");
        runBtn.type = "submit";
        runBtn.value = "运行";
        runBtn.id = "runbtn";
        runBtn.disabled = true;
        runBtn.addEventListener("click", function () {
            // var mark: boolean = true;
            // (m.inputs as string[]).forEach(v => {
            //     if((<HTMLInputElement> document.getElementsByName(v)[0]).value == '') {
            //         alert("参数 [" + m.properties[v].name + "] 必须填写。");
            //         mark = false;
            //     }
            // });
            // if (mark) run(m.mm);
            run(m.mm);
        });
        navLi.appendChild(runBtn);
        navUl.appendChild(navLi);
        navLi = document.createElement("li");
        var resetBtn = document.createElement("input");
        resetBtn.type = "submit";
        resetBtn.value = "重置";
        resetBtn.id = "runbtn";
        resetBtn.addEventListener("click", function () {
            if (html != null) {
                html.innerHTML = '';
                load(m.mm);
            }
        });
        navLi.appendChild(resetBtn);
        navUl.appendChild(navLi);
        html.appendChild(navUl);
        for (var key in m.properties) {
            var p = document.createElement("p");
            if (m.properties[key].hasOwnProperty("name")) {
                var label = document.createElement("label");
                label.textContent = m.properties[key].name;
                label.className = "label-before";
                p.appendChild(label);
            }
            if (m.properties[key].hasOwnProperty("opt")) {
                if (Object.keys(m.properties[key].opt).length <= 3) {
                    for (var i in m.properties[key].opt) {
                        var rad = document.createElement("input");
                        rad.name = key;
                        rad.value = m.properties[key].opt[i];
                        rad.type = "radio";
                        if (m.inputs.indexOf(key) == -1) {
                            //rad.readOnly = true;
                            rad.disabled = true;
                        }
                        rad.addEventListener("change", function () { isRunable(m); });
                        var label = document.createElement("label");
                        label.textContent = i;
                        label.className = "label-after";
                        p.appendChild(rad);
                        p.appendChild(label);
                    }
                }
                else {
                    var select = document.createElement("select");
                    select.name = key;
                    if (m.inputs.indexOf(key) == -1) {
                        select.disabled = true;
                    }
                    for (var j in m.properties[key].opt) {
                        var opt = document.createElement("option");
                        opt.value = m.properties[key].opt[j];
                        opt.text = j;
                        select.options.add(opt);
                    }
                    select.addEventListener("change", function () {
                        isRunable(m);
                        this.style.border = "#000 1px solid";
                    });
                    p.appendChild(select);
                }
            }
            else {
                var type = '';
                switch (typeof m[key]) {
                    case "boolean":
                        type = "checkbox";
                        break;
                    case "number":
                        type = "number";
                        break;
                    case "string":
                        type = "text";
                        break;
                }
                var input = document.createElement("input");
                input.type = type;
                input.name = key;
                //input.addEventListener("change", function(){isRunable(m)});
                input.addEventListener("input", function () {
                    isRunable(m);
                    if (this.value != '') {
                        this.style.border = "black 1px solid";
                    }
                    else {
                        this.style.border = "red 1px solid";
                    }
                });
                if (type == "checkbox") {
                    p.innerHTML = '';
                    if (m.inputs.indexOf(key) == -1) {
                        input.disabled = true;
                    }
                    p.appendChild(input);
                    if (m.properties[key].hasOwnProperty("name")) {
                        var label = document.createElement("label");
                        label.textContent = m.properties[key].name;
                        label.className = "label-after";
                        p.appendChild(label);
                    }
                }
                else {
                    if (m.inputs.indexOf(key) == -1) {
                        input.readOnly = true;
                    }
                    p.appendChild(input);
                    if (m.properties[key].hasOwnProperty("unit")) {
                        var label2 = document.createElement("label");
                        label2.textContent = m.properties[key].unit;
                        label2.className = "label-after";
                        p.appendChild(label2);
                    }
                }
            }
            html.appendChild(p);
        }
    }
}
function load(m) {
    switch (true) {
        case m == "getDimByV" || m == "getDimByDropP" || m == "getDimBySCH" || m == "getWeight" || m == "getInsultionVandS":
            var pipe = new Pipe(m);
            initContent(pipe);
            break;
        case m == "water":
            var w = new State2();
            w.mm = m;
            initContent(w);
            break;
        default:
            alert("未找到计算模块");
    }
}
function run(m) {
    console.log("run:", m);
    switch (m) {
        case "getDimByV":
            var f = Number(document.getElementsByName("f")[0].value);
            var v = Number(document.getElementsByName("v")[0].value);
            var pipe = new Pipe("", { f: f, v: v });
            pipe.getDimByV();
            document.getElementsByName("dim")[0].value = pipe.dim.toString();
            break;
        case "getDimByDropP":
            var f = Number(document.getElementsByName("f")[0].value);
            var dropp = Number(document.getElementsByName("dropp")[0].value);
            var rho = Number(document.getElementsByName("rho")[0].value);
            var visc = Number(document.getElementsByName("visc")[0].value);
            var pipe = new Pipe("", { f: f, dropp: dropp, rho: rho, visc: visc });
            pipe.getDimByDropP();
            document.getElementsByName("dim")[0].value = pipe.dim.toString();
            break;
        case "getDimBySCH":
            var dn = document.getElementsByName("dn")[0].value;
            var sch = document.getElementsByName("sch")[0].value;
            var l = Number(document.getElementsByName("l")[0].value);
            var pipe = new Pipe("", { dn: dn, sch: sch, l: l });
            pipe.getDimBySCH();
            document.getElementsByName("do")[0].value = pipe.do.toString();
            if (pipe.thk != undefined) {
                document.getElementsByName("thk")[0].value = pipe.thk.toString();
                document.getElementsByName("pw")[0].value = pipe.pw.toString();
                document.getElementsByName("wgt")[0].value = pipe.wgt.toString();
            }
            else {
                document.getElementsByName("thk")[0].value = "0";
            }
            break;
        case "getWeight":
            var dom = Number(document.getElementsByName("do")[0].value);
            var thk = Number(document.getElementsByName("thk")[0].value);
            var l = Number(document.getElementsByName("l")[0].value);
            var pipe = new Pipe("", { do: dom, thk: thk, l: l });
            pipe.getWeight();
            document.getElementsByName("pw")[0].value = pipe.pw.toString();
            document.getElementsByName("wgt")[0].value = pipe.wgt.toString();
            break;
        case "getInsultionVandS":
            var dom = Number(document.getElementsByName("do")[0].value);
            var ithk = Number(document.getElementsByName("ithk")[0].value);
            var l = Number(document.getElementsByName("l")[0].value);
            var pipe = new Pipe("", { do: dom, ithk: ithk, l: l });
            pipe.getVOfInsultion();
            pipe.getSOfInsultion();
            document.getElementsByName("iv")[0].value = pipe.iv.toString();
            document.getElementsByName("is")[0].value = pipe.is.toString();
            break;
        case "water":
            var s = new State2();
            var inputs = {};
            s.inputs.forEach(function (v) {
                var input = document.getElementsByName(v)[0].value;
                if (input != '') {
                    inputs[v] = Number(input);
                }
            });
            if (Object.keys(inputs).length == 2) {
                var w = new IAPWS97();
                var s2 = w.solve(inputs);
                console.log(s2);
                Object.keys(s2.properties).forEach(function (v) {
                    document.getElementsByName(v)[0].value = s2[v];
                    console.log(v);
                });
            }
            else {
                alert("请点击重置后，重新输入");
            }
            break;
        default:
            console.log("未找到", m, "in run()");
    }
}
function isRunable(m) {
    var flag = true;
    if ("water" == m.mm) {
        var count = 0;
        m.inputs.forEach(function (v) {
            if (document.getElementsByName(v)[0].value != '') {
                ++count;
            }
        });
        if (count != 2) {
            flag = false;
            m.inputs.forEach(function (v) {
                document.getElementsByName(v)[0].readOnly = false;
            });
        }
        else {
            m.inputs.forEach(function (v) {
                if (document.getElementsByName(v)[0].value == '') {
                    document.getElementsByName(v)[0].readOnly = true;
                }
            });
        }
    }
    else {
        m.inputs.forEach(function (v) {
            if (document.getElementsByName(v)[0].value == '') {
                //alert("参数 [" + m.properties[v].name + "] 必须填写。");
                flag = false;
            }
        });
    }
    if (flag) {
        document.getElementById("runbtn").disabled = false;
    }
    else {
        document.getElementById("runbtn").disabled = true;
    }
}
//网页载入时，初始化侧边栏
(function () { initSiderbar(mlist, mnlist); })();
