function pipeDiameter(rows) {
    let flowRate = parseFloat(rows[2].value) / 3600;
    let v = parseFloat(rows[3].value);

    if (isNaN(flowRate) || isNaN(v)) {
        return false;
    }

    let d = (Math.sqrt(flowRate / v / 3.14) * 2 * 1000).toFixed(2);

    return [d];
}

function pipeVelocity(rows) {
    let flowRate = parseFloat(rows[2].value) / 3600;
    let d = parseFloat(rows[3].value) / 1000;

    if (isNaN(flowRate) || isNaN(d)) {
        alert("请检查输入！");
        return false;
    }

    let v = (flowRate / (3.14 * d * d / 4)).toFixed(2);

    return [v];
}

function pipeWeight(rows) {
    let d = parseFloat(rows[2].value) / 1000;
    let thk = parseFloat(rows[3].value) / 1000;
    let rho = parseFloat(rows[4].value);
    let len = parseFloat(rows[5].value);

    if (isNaN(d) || isNaN(thk) || isNaN(rho)) {
        alert("请检查输入！");
        return false;
    }

    let perweight = (3.14 * rho * (d - thk) * thk).toFixed(2);
    let weight
    if (!isNaN(len)) {
        weight = (3.14 * rho * (d - thk) * thk * len).toFixed(2);  
    }
    
    return [perweight, weight];
}

function pipeSize(rows) {
    let idn = parseInt(rows[0].value) ;
    let isch = parseInt(rows[1].value) ;
    
    const Schs = ["SCH5", "SCH10", "SCH20", "SCH30", "SCH40", "SCH60", "SCH80", "SCH100", "SCH120", "SCH140", "SCH160", "STD", "XS", "XXS", "SCH5S", "SCH10S", "SCH40S", "SCH80S"]
	const DN = ["DN6", "DN8", "DN10", "DN15", "DN20", "DN25", "DN32", "DN40", "DN50", "DN65", "DN80", "DN90", "DN100", "DN125", "DN150", "DN200", "DN250", "DN300", "DN350", "DN400", "DN450", "DN500", "DN550", "DN600"]
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
	}
	const Thk = {
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
    
    //let i = Schs.indexOf(sch);
    let dn = DN[idn];
    let d = D0[dn];
    let thk = Thk[dn][isch];
    thk = thk == undefined ? dn + "钢管无" + Schs[isch] : thk;
    let m; 
    if (thk != undefined) {
        m = (3.14 * 7850 * (d / 1000 - thk / 1000) * thk / 1000).toFixed(2);
    }
    
    return [d, thk, m];
}

function pipeStrength(rows) {
    let d = parseFloat(rows[2].value);
    let p = parseFloat(rows[3].value);
    let s = parseFloat(rows[4].value);
    let y = parseFloat(rows[5].value);
    let w = parseFloat(rows[6].value);
    let phi = parseFloat(rows[7].value);
    let c1 = parseFloat(rows[8].value);
    let c2 = parseFloat(rows[9].value);
    let c3 = parseFloat(rows[10].value);
    let r = parseFloat(rows[11].value);

    if (isNaN(d) || isNaN(p) || isNaN(s) || isNaN(y) || isNaN(w) || isNaN(phi) || isNaN(c1) || isNaN(c2) || isNaN(c3)) {
        alert("请检查输入！");
        return false;
    }

    let t = (p * d / (2 * (s * phi * w + p * y))).toFixed(2);
    let t_ = (t * (1 + c1) + c2 + c3).toFixed(2);
    let t0, t0_, t1, t1_, t2, t2_;
    if (!isNaN(r)) {
        //弯管内侧壁厚
        let i1 = (4 * (r / d) - 1) / (4 * (r / d) - 2);
        t0 = (p * d / (2 * (s * phi * w / i1 + p * y))).toFixed(2);
        t0_ = (t0 * (1 + c1) + c2 + c3).toFixed(2);

        //弯管外侧壁厚
        let i2 = (4 * (r / d) + 1) / (4 * (r / d) + 2);
        t1 = (p * d / (2 * (s * phi * w / i2 + p * y))).toFixed(2);
        t1_ = (t1 * (1 + c1) + c2 + c3).toFixed(2);

        //弯管中心线侧壁
        t2 = (p * d / (2 * (s * phi * w / 1 + p * y))).toFixed(2); 
        t2_ = (t2 * (1 + c1) + c2 + c3).toFixed(2);
    }
    
    return [t, t_, t0, t0_, t1, t1_, t2, t2_];
}

function pipeHInsultion(rows) {
    return pipeInsultion(rows);
}

function pipeCInsultion(rows) {
    return pipeInsultion(rows);
}

function pipeInsultion(rows) {
    let d0 = parseFloat(rows[2].value) / 1000;
    let t0 = parseFloat(rows[3].value);
    let ta = parseFloat(rows[4].value);
    let w = parseFloat(rows[5].value);
    let d1 = d0 + 2 * parseFloat(rows[6].value) / 1000;
    let epsilon = parseFloat(rows[7].value);
    let lambdaArgs = [];
    for (let i = 0; i < 7; i++) {
        lambdaArgs[i] = parseFloat(rows[8 + i].value);
        if (isNaN(lambdaArgs[i])) {
            alert("请检查输入！");
            return false;
        }
    }
    
    if (isNaN(d0) || isNaN(t0) || isNaN(ta) || isNaN(w) || isNaN(d1) || isNaN(epsilon)) {
        alert("请检查输入！");
        return false;
    }

    let lambda = getLambda(t0, ta, lambdaArgs);
    let ts = getTs(t0, ta, w, d0, d1, lambda, epsilon);
    let alpha = getAlpha(ts, ta, w, d1, epsilon);
    let Q = getQ(t0, ta, d0, d1, lambda, alpha);
    let q = 3.14 * d1 * Q;

    return [lambda.toFixed(2), alpha.toFixed(2), ts.toFixed(2), Q.toFixed(2), q.toFixed(2)];
}

function waterpipe(rows) {
    let flowRate = parseFloat(rows[2].value) / 3600;
    let a_t = parseFloat(rows[3].value);//入口温度
    let a_p = parseFloat(rows[4].value);//出口压力
    let d0 = parseFloat(rows[5].value) / 1000;
    let di = d0 - 2 * parseFloat(rows[6].value) / 1000;
    let li = parseFloat(rows[7].value);
    let lp = li + parseFloat(rows[8].value);
    let rough = parseFloat(rows[9].value) / 1000;
    let d1 = d0 + 2 * parseFloat(rows[10].value) / 1000;
    let lambdaArgs = [];
    for (let i = 0; i < 7; i++) {
        lambdaArgs[i] = parseFloat(rows[11 + i].value);
        if (isNaN(lambdaArgs[i])) {
            alert("请检查输入！");
            return false;
        }
    }
    let epsilon = parseFloat(rows[18].value);
    let ta = parseFloat(rows[19].value);
    let w = parseFloat(rows[20].value);

    if (isNaN(flowRate) || isNaN(a_t) || isNaN(a_p) || isNaN(d0) || isNaN(di) || isNaN(li) 
        || isNaN(lp) ||isNaN(rough) || isNaN(d1) || isNaN(ta) || isNaN(w) || isNaN(epsilon)) {
        alert("请检查输入！");
        return false;
    }

    let a_h = h_pT(a_p, a_t + 273.15);
    let a_rho = rho_pT(a_p, a_t + 273.15);
    let a_x = x_ph(a_p, a_h);
    let a_f_gas = flowRate * a_x;
    let a_f_liquid = flowRate * (1 - a_x);
    let a_ve = getVelocity(flowRate / a_rho, di);
    let a_pd = pd(a_ve, 1 / a_rho);

    let re = reynolds(di, a_ve, a_rho, my_pT(a_p, a_t + 273.15));
    let resis = resistace(re, di, rough);
    let kt0 = kt(resis, lp, di);

    let b_p = p2(a_pd, a_p * 1e6, kt0) / 1e6;

    let lambda = getLambda(a_t, ta, lambdaArgs);
    let ts = getTs(a_t, ta, w, d0, d1, lambda, epsilon);
    let alpha = getAlpha(ts, ta, w, d1, epsilon);
    let t0 = getT0(flowRate, d0, d1, li, a_t, a_p, b_p, ta, lambda, alpha);
    let Q = getQ(t0, ta, d0, d1, lambda, alpha);
    let q = getq(Q, d1);

    let b_h = getHB(a_h, flowRate, q * li);
    let b_t = T_ph(b_p, b_h) - 273.15;
    let b_rho = rho_ph(b_p, b_h);
    let b_x = x_ph(b_p, b_h);
    let b_f_gas = flowRate * b_x;
    let b_f_liquid = flowRate * (1 - b_x);
    let b_ve = getVelocity(flowRate / b_rho, di);

    return [(a_p - b_p).toFixed(2),
        (a_t - b_t).toFixed(2),
        lambda.toFixed(4),
        alpha.toFixed(2),
        ts.toFixed(2),
        Q.toFixed(2),
        q.toFixed(2),
        (q * li).toFixed(2),
        b_p.toFixed(2),
        b_t.toFixed(2),
        (a_f_gas * 3600).toFixed(2),
        (a_f_liquid * 3600).toFixed(2),
        (b_f_gas * 3600).toFixed(2),
        (b_f_liquid * 3600).toFixed(2),
        a_ve.toFixed(2),
        b_ve.toFixed(2),
        a_h.toFixed(2),
        b_h.toFixed(2),
        a_x.toFixed(2),
        b_x.toFixed(2)
    ];
}

/**
 * 计算管道流速
 * @param f flow rate [m3/s]
 * @param d Pipe Internal diameter [m]
 * @returns velocity [m/s]
 */
function getVelocity(f, d) {
    return f / (3.14 * d * d / 4);
}

/*** 管道阻力 ***/
/**
 * 雷诺数
 * @param di Pipe Internal diameter [m]
 * @param ve flow rate [m/s]
 * @param rho density [kg/m3]
 * @param mu Dynamic viscosity [Ps.s]
 * @returns reynolds number
 */
function reynolds(di, ve, rho, mu) {
    return di * ve * rho / mu;
}
/**
 * 阻力系数
 * @param re reynolds number
 * @param di  Pipe Internal diameter [m]
 * @param e Roughness [m]
 * @returns resistance coefficient
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
 * @param f f flow [m3/h]
 * @param e Roughness [m]
 * @param di Pipe Internal diameter [m]
 * @param l length [m]
 * @param rho density [kg/m3]
 * @param mu Dynamic viscosity [Ps.s]
 * @returns dp [kPa]
 */
function pipeDp0(f, e, di, l, rho, mu) {
    var ve = f / 3600 / (Pi * Math.pow((di / 2), 2));
    var re = reynolds(di, ve, rho, mu);
    var lambda = resistace(re, di, e);
    var dp0 = lambda * (l / di) * (rho * Math.pow(ve, 2) / 2) * 0.001;
    return dp0;
}
/**
 * 局部阻力 SHT3035-2018
 * @param ksum 总局部阻力系数
 * @param ve
 * @param rho
 * @returns
 */
function pipeDp1(ksum, ve, rho) {
    var dp1 = ksum * rho * Math.pow(ve, 2) / 2 * 0.001;
    return dp1;
}
/**
 *
 * @param lambda 阻力系数
 * @param l 长度 [m]
 * @param di 内径 [m]
 * @returns
 */
function kt(lambda, l, di) {
    return lambda / di * l;
}
/**
 * 动压 [Pa]
 * @param ve 流速 [m/s]
 * @param v 比体积 [m3/kg]
 * @returns
 */
function pd(ve, v) {
    return ve * ve / v / 2;
}
/**
 * 终端压力 [Pa] DLT5054-2016 7.3.3-1
 * @param pd1 始端动压力[Pa]
 * @param p1 始端压力[Pa]
 * @param kt 管道总阻力系数
 * @returns
 */
function p2(pd1, p1, kt) {
    //console.log(pd1, p1, kt, );
    let c = 1 - 2 * pd1 / p1 * kt * (1 + 2.5 * pd1 / p1);
    if (c < 0) {
        alert("错误：管道阻力过大。");
        return -1;
    }
    return p1 * Math.pow(c, 0.5);
}
/**
 GB50264-2013
 t0  管道或设备的外表面温度[C]
 ta  环境温度[C]
 tm  绝热材料平均温度[C]
 a_t  介质（上游）A点处的温度[C]
 b_t  介质（下游）B点处的温度[C]
 ts  绝热层外表面温度[C]
 d0  管道或设备外径[m]
 d1  内层绝热层外径[m]
 lambda  绝热材料在平均温度下的导热系数[W/(m*K)]
 alpha  对流换热系数 [W/(m2*K)]
 epsilon 绝热结构外面材料的黑度
 kr 管道通过吊架处的热损失附加系数，kr=1.1-1.2，大管取值应靠下限，小管取值应靠上限
 Lc 特征长度，d1log(d1/d0)
 w 风速[m/s]
 */
 
/**
 * 绝热材料传热系数
 * @param ta
 * @param t0
 * @param coefArgs[]
 * @returns
 */
function getLambda(t0, ta, coefArgs) {
	let tm = 0.5 * (t0 + ta);
    return coefArgs[0] + coefArgs[1] * (tm - coefArgs[2]) + coefArgs[3] * Math.pow(tm - coefArgs[4], 2) + coefArgs[5] * Math.pow(tm - coefArgs[6], 3);
}

//外表面温度计算，公式5.3.11，假定外表面温度，迭代计算
function getTs(t0, ta, w, d0, d1, lambda, epsilon) {
	let ts_pre = ta + 0.1 * (t0 - ta), ts
	do
	{
		ts = ts_pre
		let alpha = getAlpha(ts, ta, w, d1, epsilon)
		let Q = getQ(t0, ta, d0, d1, lambda, alpha)
		ts_pre = ts_Q(Q, ta, alpha)
	}
	while(Math.abs(ts - ts_pre) > 1e-3)
	
	return ts
}
/**
 * 已知Q，计算绝热结构的外表面温度，公式5.5.1
 * @param Q
 * @param ta
 * @param alphas
 * @returns
 */
function ts_Q(Q, ta, alpha) {
    return Q / alpha + ta;
}
/**
 * 已知最大允许热损失，计算Lc，公式5.3.3-1
 * @param t0
 * @param ta
 * @param Q
 * @param lambda
 * @param alphas
 * @returns
 */
function lc_Q(t0, ta, Q, lambda, alphas) {
    return 2 * lambda * ((t0 - ta) / Q - 1 / alphas);
}
/**
 * 已知表面温度，计算特征长度，公式5.3.11
 * @param t0
 * @param ta
 * @param ts
 * @param lambda
 * @param alphas
 * @returns
 */
function lc_ts(t0, ta, ts, lambda, alphas) {
    return 2 * lambda / alphas * (t0 - ts) / (ts - ta);
}
/**
 * 圆筒型单层绝热结构热、冷损失量计算，公式：5.4.3-1
 * @param t0
 * @param ta
 * @param d0
 * @param d1
 * @param lambda
 * @param alphas
 * @returns 每平方米绝热层外表面积的热损失量[W/m2]
 */
function getQ(t0, ta, d0, d1, lambda, alpha) {
    return (t0 - ta) / (d1 / (2 * lambda) * Math.log(d1 / d0) + 1 / alpha);
}
/**
 * 每米管道热损失量计算，公式5.4.3-2
 * @param Q
 * @param d1
 * @returns 单位长度散热量[W/m]
 */
function getq(Q, d1) {
    return Math.PI * d1 * Q;
}

/**
 * 表面换热系数计算
 * @param
 * @param
 * @returns
 */
function getAlpha(ts, ta, w, d1, epsilon) {
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
 *  表面换热系数计算，防烫保温
 * @returns
 */
function getAlpha2() {
    return 8.141;
}
/**
 * 管托热损失
 * @param H 管托高度      [m]
 * @param P 管托截面周长  [m]
 * @param A0 管托截面面积 [m2]
 * @returns
 */

/**
 * 计算管道末端介质焓值
 * @param a_h 始端焓值[kj/kg]
 * @param f 流量[kg/s]
 * @param qTotal 总散热量[W]
 * @param l 管道长度[m]
 * @returns 末端焓值 kj/kg
 */
function getHB(a_h, f, qTotal) {
    return a_h - qTotal / 1000 / f;
}
/**
 * 仅限于蒸汽
 * @param a_t 始端温度[℃]
 * @param a_h 始端压力[MPaA]
 * @param b_p 末端压力[MPaA]
 * @param ta 环境温度[℃]
 * @param d0 管道外径[m]
 * @param d1 保温外径[m]
 * @param epsilon 黑度
 * @param w 风速[m/s]
 * @param f 流量[kg/s]
 * @param l 管道长度[m]
 * @returns 管道或设备外表面平均温度[℃]
 */
function getT0(f, d0, d1, l, a_t, a_p, b_p, ta, lambda, alpha) {
	var t0 = a_t, Q, q, b_h, b_t, qbr;
    var t0_tmp; // = t_A;
    do {
        t0_tmp = t0;
        Q = getQ(t0, ta, d0, d1, lambda, alpha);
        q = getq(Q, d1);
		
        b_h = getHB(h_pT(a_p, a_t + 273.15), f, q * l);
		b_t = T_ph(b_p, b_h) - 273.15;
        t0 = (a_t + b_t) / 2;
    } while (Math.abs(t0 - t0_tmp) > 0.01);
    return t0;
}