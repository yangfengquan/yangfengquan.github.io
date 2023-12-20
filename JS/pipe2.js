"use strict"

/**
 * 已知体积流量、流速，计算管径
 * @param {number} flowRate 体积流量 m3/s
 * @param {number} v 流速 m/s
 * @returns {number} 管径 m
 */
function pipe_diameter(flowRate, v) {
    return 2 * Math.sqrt(flowRate / Math.PI / v)
}

/**
 * 已知体积流量、内径，计算流速
 * @param {number} flowRate 体积流量 m3/s
 * @param {number} di 内径 m
 * @returns {number} 流速 m/s
 */
function pipe_velocity(flowRate, di) {
    return flowRate / (3.14 * di * di / 4);
}

/**
 * 计算管子单重
 * @param {number} d 外径 m
 * @param {number} deta 壁厚 m
 * @param {number} len 长度 m
 * @param {number} rho 密度 kg/m3
 * @returns {number} 单重 kg/m
 */
function pipe_weight(d, deta, rho = 7850) {
    return 3.14 * rho * (d - deta) * deta;
}

const Schs = ["SCH5", "SCH10", "SCH20", "SCH30", "SCH40", "SCH60", "SCH80", "SCH100", "SCH120", "SCH140", "SCH160", "STD", "XS", "XXS", "SCH5S", "SCH10S", "SCH40S", "SCH80S"];
const DN = ["DN6", "DN8", "DN10", "DN15", "DN20", "DN25", "DN32", "DN40", "DN50", "DN65", "DN80", "DN90", "DN100", "DN125", "DN150", "DN200", "DN250", "DN300", "DN350", "DN400", "DN450", "DN500", "DN550", "DN600"];
const D0 = {
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

//mm
const SCH_Delta = {
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

/**
 * 查询管子外径、壁厚，计算管子单重
 * @param {string} dn 公称直径 例如‘DN20’
 * @param {string} sch 壁厚系列 例如‘SCH20’
 * @returns {object} {外径,壁厚,单重} {mm,mm,kg/m}
 */
function pipe_size(dn, sch) {
    let r = {}
    let i = Schs.indexOf(sch);
    let d = D0[dn];
    let delta = SCH_Delta[dn][i];
    if (delta == undefined) {
        alert(dn + "钢管无" + sch)
    }
   
    let m = pipe_weight(d / 1000, delta / 1000);
    return {d: d, delta: delta, m: m};
}

/**
 * 计算直管壁厚 GB/T20801
 * @param {number} d 外径 mm
 * @param {number} p 设计压力 MPa
 * @param {number} s 设计温度下管道材料许用应力 MPa
 * @param {number} y 计算系数
 * @param {number} w 焊接接头高温降低系数
 * @param {number} phi 焊件的纵向焊接接头系数或铸件质量系数
 * @param {number} c1 材料负厚度偏差 %
 * @param {number} c2 腐蚀、冲蚀裕量 mm
 * @param {number} c3 机械加工深度 mm
 * @returns {object} [直管计算厚度,直管名义厚度] [mm,mm]
 */
function pipe_strength(d, p, s, y, w, phi, c1, c2, c3) {
    let r = {};
    r.t = p * d / (2 * (s * phi * w + p * y));
    r.nt = t * (1 + c1 / 100) + c2 + c3;

    return r;
}

/**
 * 计算弯管壁厚 GB/T20801
 * @param {number} d 外径 mm
 * @param {number} p 设计压力 MPa
 * @param {number} s 设计温度下管道材料许用应力 MPa
 * @param {number} y 计算系数
 * @param {number} w 焊接接头高温降低系数
 * @param {number} phi 焊件的纵向焊接接头系数或铸件质量系数
 * @param {number} c1 材料负厚度偏差 %
 * @param {number} c2 腐蚀、冲蚀裕量 mm
 * @param {number} c3 机械加工深度 mm
 * @param {number} r 弯管或弯头在管子中心线处的弯曲半径 mm
 * @returns {object} [内侧计算厚度,内侧名义厚度,外侧计算厚度,外侧名义厚度,中心侧计算厚度,中心侧名义厚度] [mm,mm,mm,mm,mm,mm]
 */
function bend_strength(d, p, s, y, w, phi, c1, c2, c3, r) {
    let tw = function (i) {
        return p * d / (2 * (s * phi * w / i + p * y));
    }

    let ntw = function (t) {
        return t * (1 + c1 / 100) + c2 + c3;
    }

    let i1 = (4 * (r / d) - 1) / (4 * (r / d) - 2); //内侧计算系数
    let i2 = (4 * (r / d) + 1) / (4 * (r / d) + 2); //外侧计算系数
    let i3 = 1; //中心侧壁计算系数

    let t1 = tw(i1); //内侧计算厚度
    let t2 = tw(i2); //外侧计算厚度
    let t3 = tw(i3); //中心侧计算厚度

    let nt1 = ntw(t1); //内侧名义厚度
    let nt2 = ntw(t2); //外侧名义厚度
    let nt3 = ntw(t3); //中心侧名义厚度
    return {t1: t1, t2: t2, t3: t3, nt1: nt1, nt2:nt2, nt3: nt3};
}


/* 管道阻力 */

/**
 * reynolds number
 * @param {number} di Pipe Internal diameter [m]
 * @param {number} ve Velocity [m/s]
 * @param {number} rho density [kg/m3]
 * @param {number} mu Dynamic viscosity [Ps.s]
 * @returns {number} reynolds number
 */
function reynolds(di, ve, rho, mu) {
    return di * ve * rho / mu;
}

/**
 * 阻力系数
 * @param {number} re reynolds number
 * @param {number} di  Pipe Internal diameter [m]
 * @param {number} e Roughness [m]
 * @returns {number} resistance coefficient
 */
function resistace(re, di, e) {
    var lambda;
    if (re <= 2000) {
        lambda = 64 / re;
    }
    else if (re > 2000 && re <= 4000) {
        lambda = 0.3164 * Math.pow(re, -0.25);
    }
    else if (re > 4000 && re < 396 * di / e * Math.log10(3.7 * di / e)) {
        var x0 = 0, x1 = 0.1;
        do {
            var mid = (x0 + x1) / 2;
            if (1 / Math.pow(mid, 0.5) + 2 * Math.log10(e / (3.7 * di) + 2.51 / (re * Math.pow(mid, 0.5))) > 0) {
                x0 = mid;
            }
            else {
                x1 = mid;
            }
        } while (Math.abs(x0 - x1) > 1e-6);
        lambda = (x0 + x1) / 2;
    }
    else {
        var x0 = 0, x1 = 0.1;
        do {
            var mid = (x0 + x1) / 2;
            if (1 / Math.pow(mid, 0.5) + 2 * Math.log10(e / (3.7 * di)) > 0) {
                x0 = mid;
            }
            else {
                x1 = mid;
            }
        } while (Math.abs(x0 - x1) > 1e-6);
        lambda = (x0 + x1) / 2;
    }
    return lambda;
}

/**
 * 直管阻力 SHT3035-2018
 * @param {number} flowRate flow rate [m3/s]
 * @param {number} e Roughness [m]
 * @param {number} di Pipe Internal diameter [m]
 * @param {number} l length [m]
 * @param {number} rho density [kg/m3]
 * @param {number} mu Dynamic viscosity [Ps.s]
 * @returns {number} pressure drop [Pa]
 */
function pipe_lDp(flowRate, e, di, l, rho, mu) {
    var ve = pipe_velocity(flowRate, di);
    var re = reynolds(di, ve, rho, mu);
    var lambda = resistace(re, di, e);
    return lambda * (l / di) * (rho * Math.pow(ve, 2) / 2);
}

/**
 * 局部阻力 局部阻力系数法 SHT3035-2018
 * @param {number} flowRate flow rate [m3/s]
 * @param {number} di Pipe Internal diameter [m]
 * @param {number} ksum 局部阻力系数总和
 * @param {number} rho density [kg/m3]
 * @returns {number} [pa]
 */
function pipe_fDpK(flowRate, di, ksum, rho) {
    var ve = pipe_velocity(flowRate, di);
    return ksum * rho * Math.pow(ve, 2) / 2;
}

/**
 * 局部阻力 当量长度法 SHT3035-2018
 * @param {number} flowRate flow rate [m3/s]
 * @param {number} di Pipe Internal diameter [m]
 * @param {number} le 当量长度总和 [m]
 * @param {number} rho density [kg/m3]
 * @returns {number} [Pa]
 */
function pipe_fDpK(flowRate, di, le, rho, mu) {
    var ve = pipe_velocity(flowRate, di);
    var re = reynolds(di, ve, rho, mu);
    var lambda = resistace(re, di, e);
    return lambda * le / di * rho * Math.pow(ve, 2) / 2;
}

/**
 * 管道总阻力系数 DL/T5054
 * @param {number} lambda 管道摩擦系数
 * @param {number} l 管道总展开长度，包括附件长度 m
 * @param {number} di 内径 m
 * @param {number} ksum 局部阻力系数总和
 * @returns {number}
 */
function kt(lambda, l, di, ksum = 0) {
    return lambda / di * l + ksum;
}

/**
 * 动压 [Pa]
 * @param {number} ve 流速 [m/s]
 * @param {number} v 比体积 [m3/kg]
 * @returns {number} [Pa]
 */
function pd(ve, v) {
    return ve * ve / v / 2;
}

/**
 * 终端压力 [Pa] DLT5054-2016 7.3.3-1
 * @param {number} pd1 始端动压力 [Pa]
 * @param {number} p1 始端压力 [Pa]
 * @param {number} kt 管道总阻力系数
 * @returns {number} [Pa]
 */
function p2(pd1, p1, kt) {
    let c = 1 - 2 * pd1 / p1 * kt * (1 + 2.5 * pd1 / p1);
    if (c < 0) {
        alert("错误：管道阻力过大。");
        return -1;
    }
    return p1 * Math.pow(c, 0.5);
}


/* 绝热 */

/**
 * 绝热材料在平均温度下的导热系数
 * @param {number} t0 管道或设备的外表面温度[C]
 * @param {number} ta 环境温度[C]
 * @param {number[]} coefArgs 绝热材料系数
 * @returns {number} [W/(m*K)]
 */
function insul_lambda(t0, ta, coefArgs) {
	let tm = 0.5 * (t0 + ta);
    return coefArgs[0] + coefArgs[1] * (tm - coefArgs[2]) + coefArgs[3] * Math.pow(tm - coefArgs[4], 2) + coefArgs[5] * Math.pow(tm - coefArgs[6], 3);
}

/**
 * 外表面温度计算，GB50264公式5.3.11，假定外表面温度，迭代计算
 * @param {number} t0 管道或设备的外表面温度[C]
 * @param {number} ta 环境温度[C]
 * @param {number} w 风速[m/s]
 * @param {number} d0 管道或设备外径[m]
 * @param {number} d1 内层绝热层外径[m]
 * @param {number} lambda 绝热材料在平均温度下的导热系数[W/(m*K)]
 * @param {number} epsilon 绝热结构外面材料的黑度
 * @returns 
 */
function insul_ts(t0, ta, w, d0, d1, lambda, epsilon) {
	let ts_pre = ta + 0.1 * (t0 - ta), ts
	do
	{
		ts = ts_pre
		let alpha = insul_alpha(ts, ta, w, d1, epsilon)
		let Q = insul_Q(t0, ta, d0, d1, lambda, alpha)
		ts_pre = ts_Q(Q, ta, alpha)
	}
	while(Math.abs(ts - ts_pre) > 1e-3)
	
	return ts
}

/**
 * 已知Q，计算绝热结构的外表面温度，GB50264公式5.5.1
 * @param {number} Q 以每平方米绝热层外表面积表示的热损失量[W/m2]
 * @param {number} ta 环境温度[C]
 * @param {number} alpha 对流换热系数[W/(m2*K)]
 * @returns {number} [C]
 */
function ts_Q(Q, ta, alpha) {
    return Q / alpha + ta;
}

/**
 * 已知最大允许热损失，计算Lc，Lc=d1ln(d1/d0)，GB50264公式5.3.3-1
 * @param {number} t0 管道或设备的外表面温度[C]
 * @param {number} ta 环境温度[C]
 * @param {number} Q 以每平方米绝热层外表面积表示的热损失量[W/m2]
 * @param {number} lambda 绝热材料在平均温度下的导热系数[W/(m*K)]
 * @param {number} alphas 对流换热系数[W/(m2*K)]
 * @returns {number}
 */
function lc_Q(t0, ta, Q, lambda, alphas) {
    return 2 * lambda * ((t0 - ta) / Q - 1 / alphas);
}

/**
 * 已知表面温度，计算特征长度，GB50264公式5.3.11
 * @param {number} t0 管道或设备的外表面温度[C]
 * @param {number} ta 环境温度[C]
 * @param {number} ts 绝热层外表面温度[C]
 * @param {number} lambda 绝热材料在平均温度下的导热系数[W/(m*K)]
 * @param {number} alphas 对流换热系数[W/(m2*K)]
 * @returns {number}
 */
function lc_ts(t0, ta, ts, lambda, alphas) {
    return 2 * lambda / alphas * (t0 - ts) / (ts - ta);
}

/**
 * 圆筒型单层绝热结构热、冷损失量计算，GB50264公式：5.4.3-1
 * @param {number} t0 管道或设备的外表面温度[C]
 * @param {number} ta 环境温度[C]
 * @param {number} d0 管道或设备外径[m]
 * @param {number} d1 内层绝热层外径[m]
 * @param {number} lambda 绝热材料在平均温度下的导热系数[W/(m*K)]
 * @param {number} alpha 对流换热系数[W/(m2*K)]
 * @returns {number} [W/m2]
 */
function insul_Q(t0, ta, d0, d1, lambda, alpha) {
    return (t0 - ta) / (d1 / (2 * lambda) * Math.log(d1 / d0) + 1 / alpha);
}

/**
 * 每米管道热损失量计算，GB50264公式5.4.3-2
 * @param {number} Q 每平方米绝热层外表面积的热损失量[W/m2]
 * @param {number} d1 内层绝热层外径[m]
 * @returns {number} [W]
 */
function insul_q(Q, d1) {
    return Math.PI * d1 * Q;
}

/**
 * 表面换热系数计算，GB50264公式5.8.4
 * @param {number} ts 绝热层外表面温度[C]
 * @param {number} ta 环境温度[C]
 * @param {number} w 风速[m/s]
 * @param {number} d1 内层绝热层外径[m]
 * @param {number} epsilon 绝热结构外面材料的黑度
 * @returns {number} 对流换热系数 [W/(m2*K)]
 */
function insul_alpha(ts, ta, w, d1, epsilon) {
	let alpha_r = 5.669 * epsilon / (ts - ta) * (Math.pow((273 + ts) / 100, 4) - Math.pow((273 + ta) / 100, 4));
	let alpha_c;
	if (w == 0) {
		alpha_c = 26.4 / Math.pow(297 + 0.5 * (ts + ta), 0.5) * Math.pow((ts - ta) / d1, 0.25);
	}
	else {
		if (w * d1 < 0.8) {	
			alpha_c = 0.08 / d1 + 4.2 * Math.pow(w, 0.618) / Math.pow(d1, 0.382);
		}
		else {
			alpha_c = 4.53 * Math.pow(w, 0.805) / Math.pow(d1, 0.195);
		}
	}

	return alpha_r + alpha_c
}

/**
 * 表面换热系数计算，防烫保温
 * @returns {number} 对流换热系数 [W/(m2*K)]
 */
function insul_alphaAS() {
    return 8.141;
}

/**
 * 计算管道末端介质焓值
 * @param {number} a_h 始端焓值[kj/kg]
 * @param {number} f 流量[kg/s]
 * @param {number} qTotal 总散热量[W]
 * @returns {number} 末端焓值 kj/kg
 */
function insul_bH(a_h, f, qTotal) {
    return a_h - qTotal / 1000 / f;
}

/**
 * 管道或设备外表面平均温度,仅限于蒸汽
 * @param {number} flowRate 流量[kg/s]
 * @param {number} d0 管道外径[m]
 * @param {number} d1 保温外径[m]
 * @param {number} l 管道长度[m]
 * @param {number} a_t 始端温度[C]
 * @param {number} a_p 始端压力[MPaA]
 * @param {number} b_p 末端压力[MPaA]
 * @param {number} ta 环境温度[C]
 * @param {number} lambda 绝热材料在平均温度下的导热系数[W/(m*K)]
 * @param {number} alpha 对流换热系数 [W/(m2*K)]
 * @returns {number} 道或设备外表面平均温度[C]
 */
function insul_t0(flowRate, d0, d1, l, a_t, a_p, b_p, ta, lambda, alpha) {
	var t0 = a_t, Q, q, b_h, b_t, qbr;
    var t0_tmp; // = t_A;
    do {
        t0_tmp = t0;
        Q = insul_Q(t0, ta, d0, d1, lambda, alpha);
        q = insul_q(Q, d1);
		
        b_h = insul_bH(h_pT(a_p, a_t + 273.15), f, q * l);
		b_t = T_ph(b_p, b_h) - 273.15;
        t0 = (a_t + b_t) / 2;
    } while (Math.abs(t0 - t0_tmp) > 0.01);
    return t0;
}

/**
 * 已知绝热厚度，计算绝热材料传热系数、表面对流换热系数、表面温度、单位面积传热量、单位长度传热量
 * @param {number} t0 管道或设备的外表面温度[C]
 * @param {number} ta 环境温度[C]
 * @param {number} w 风速[m/s]
 * @param {number} d0 管道或设备外径[m]
 * @param {number} d1 内层绝热层外径[m]
 * @param {number[]} lambdaArgs 绝热材料传热系数计算系数
 * @param {number} epsilon 绝热结构外面材料的黑度
 * @returns {object} [绝热材料传热系数,表面对流换热系数,表面温度,单位面积传热量,单位长度传热量] [W/(m*K),W/(m2*K),C,W/m2,w/m]
 */
function pipe_insultion(t0, ta, w, d0, d1, lambdaArgs, epsilon) {
    let r = {};
    r.lambda = insul_lambda(t0, ta, lambdaArgs);
    r.ts = insul_ts(t0, ta, w, d0, d1, lambda, epsilon);
    r.alpha = insul_alpha(ts, ta, w, d1, epsilon);
    r.Q = insul_Q(t0, ta, d0, d1, lambda, alpha);
    r.q = insul_q(Q, d1);

    return r;
}

/**
 * 蒸汽管道阻力和绝热计算
 * @param {number} flowRate 流量[kg/s]
 * @param {number} a_t 始端温度[C]
 * @param {number} a_p 始端压力[MPaA]
 * @param {number} di 管道内径[m]
 * @param {number} d0 管道外径[m]
 * @param {number} d1 绝热外径[m]
 * @param {number} li 直管长度[m]
 * @param {number} lf 局部当量长度[m]
 * @param {number} rough 粗糙度 [m]
 * @param {number} delta1 管子壁厚[m]
 * @param {number} delta2 保温厚度[m]
 * @param {number[]} lambdaArgs 绝热材料传热系数计算系数
 * @param {number} epsilon 绝热结构外面材料的黑度
 * @param {number} ta 环境温度[C]
 * @param {number} w 风速[m/s]
 * @returns {object} 
 */
function water_pipe(flowRate, a_t, a_p, di, d0, d1, ll, lf, rough, lambdaArgs, epsilon, ta, w) {
    let lp = ll + lf;

    let a_h = h_pT(a_p, a_t + 273.15);
    let a_rho = rho_pT(a_p, a_t + 273.15);
    let a_x = x_ph(a_p, a_h);
    let a_f_gas = flowRate * a_x;
    let a_f_liquid = flowRate * (1 - a_x);
    let a_ve = pipe_velocity(flowRate / a_rho, di);
    let a_pd = pd(a_ve, 1 / a_rho);

    let re = reynolds(di, a_ve, a_rho, my_pT(a_p, a_t + 273.15));
    let resis = resistace(re, di, rough);
    let kt0 = kt(resis, lp, di);

    let b_p = p2(a_pd, a_p * 1e6, kt0) / 1e6;

    let lambda = insul_lambda(a_t, ta, lambdaArgs);
    let ts = insul_ts(a_t, ta, w, d0, d1, lambda, epsilon);
    let alpha = insul_alpha(ts, ta, w, d1, epsilon);
    let t0 = insul_t0(flowRate, d0, d1, li, a_t, a_p, b_p, ta, lambda, alpha);
    let Q = insul_Q(t0, ta, d0, d1, lambda, alpha);
    let q = insul_q(Q, d1);

    let b_h = insul_bH(a_h, flowRate, q * li);
    let b_t = T_ph(b_p, b_h) - 273.15;
    let b_rho = rho_ph(b_p, b_h);
    let b_x = x_ph(b_p, b_h);
    let b_f_gas = flowRate * b_x;
    let b_f_liquid = flowRate * (1 - b_x);
    let b_ve = pipe_velocity(flowRate / b_rho, di);

    return {dp: a_p - b_p, dt: a_t - b_t, lambda: lambda, alpha: alpha, ts: ts, Q: Q, q: q, tq: q * li, b_p: b_p, b_t: b_t, a_f_gas: a_f_gas,
        a_f_liquid: a_f_liquid, b_f_gas: b_f_gas, b_f_liquid: b_f_liquid, a_ve: a_ve, b_ve: b_ve, a_h: a_h, b_h: b_h, a_x: a_x, b_x: b_x};
}