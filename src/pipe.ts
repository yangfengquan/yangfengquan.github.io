const Pi = 3.1415926;

const Sch = ["SCH5","SCH10","SCH20","SCH30","SCH40","SCH60","SCH80","SCH100","SCH120","SCH140","SCH160","STD","XS","XXS","SCH5S","SCH10S","SCH40S","SCH80S"];

const Do = {
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

const SHT3405 = {
    //["SCH5","SCH10","SCH20","SCH30","SCH40","SCH60","SCH80","SCH100","SCH120","SCH140","SCH160","STD","XS","XXS","SCH5S","SCH10S","SCH40S","SCH80S"]
    DN6: [,1.24,,1.45,1.73,,2.41,,,,,1.73,2.41,,,1.24,1.73,2.41],
    DN8: [,1.65,,1.85,2.24,,3.02,,,,,2.24,3.02,,,1.65,2.24,3.02],
    DN10: [,1.65,,1.85,2.31,,3.2,,,,,2.31,3.2,,,1.65,2.31,3.2],
    DN15: [1.65,2.11,,2.41,2.77,,3.73,,,,4.78,2.77,3.73,7.47,1.65,2.11,2.77,3.73],
    DN20: [1.65,2.11,,2.41,2.87,,3.91,,,,5.56,,,7.82,,,2.87,3.91],
    DN25: [1.65,2.77,,2.91,3.38,,4.55,,,,6.35,3.38,4.55,9.09,1.65,2.77,3.38,4.55],
    DN32: [1.65,2.77,,2.97,3.56,,4.85,,,,6.35,3.56,4.85,9.7,1.65,2.77,3.56,4.85],
    DN40: [1.65,2.77,,3.18,3.68,,5.08,,,,7.14,3.68,5.08,10.15,1.65,2.77,3.68,5.08],
    DN50: [1.65,2.77,,3.18,3.91,,5.54,,,,8.74,3.91,5.54,11.07,1.65,2.77,3.91,5.54],
    DN65: [2.11,3.05,,4.78,5.16,,7.01,,,,9.53,5.16,7.01,14.02,2.11,3.05,5.16,7.01],
    DN80: [2.11,3.05,,4.78,5.49,,7.62,,,,11.13,5.49,7.62,15.24,2.11,3.05,5.49,7.62],
    DN90: [2.11,3.05,,4.78,5.74,,8.08,,,,,,,,2.11,3.05,5.74,8.08],
    DN100: [2.11,3.05,,4.78,6.02,,8.56,,11.13,,13.49,6.02,8.56,17.12,2.11,3.05,6.02,8.56],
    DN125: [2.77,3.4,,,6.55,,9.53,,12.7,,15.88,6.55,9.53,19.05,2.77,3.4,6.55,9.53],
    DN150: [2.77,3.4,,,7.11,,10.97,,12.7,,18.26,7.11,10.97,19.05,2.77,3.4,7.11,10.97],
    DN200: [2.77,3.76,6.35,7.04,8.18,10.13,12.7,15.09,18.26,20.62,23.01,8.18,12.7,22.23,2.77,3.76,8.18,12.7],
    DN250: [3.4,4.19,6.35,7.8,9.27,12.7,15.09,18.26,21.44,25.4,28.58,9.27,12.7,25.4,3.4,4.19,9.27,12.7],
    DN300: [3.96,4.57,6.35,8.38,10.31,14.27,17.48,21.44,25.4,28.58,33.32,9.53,12.7,25.4,3.96,4.78,9.53,12.7],
    DN350: [3.96,6.35,7.92,9.53,11.13,15.09,19.05,23.83,27.79,31.75,35.71,9.53,12.7,,3.96,4.78,9.53,12.7],
    DN400: [4.19,6.35,7.92,9.53,12.7,16.66,21.44,26.19,30.96,36.53,40.49,9.53,12.7,,4.19,4.78,9.53,12.7],
    DN450: [4.19,6.35,7.92,11.13,14.27,19.05,23.83,29.36,34.93,39.67,45.24,9.53,12.7,,4.19,4.78,9.53,12.7],
    DN500: [4.78,6.35,9.53,12.7,15.09,20.62,26.19,32.54,38.1,44.45,50.01,9.53,12.7,,4.78,5.54,9.53,12.7],
    DN550: [4.78,6.35,9.53,12.7,,22.23,28.58,34.93,41.28,47.63,53.98,9.53,12.7,,4.78,5.54,9.53,12.7],
    DN600: [5.54,6.35,9.53,14.27,17.48,24.61,30.96,38.89,46.02,52.37,59.54,9.53,12.7,,5.54,6.35,9.53,12.7],
};

const Roughness = [
    ["无缝黄铜、铜及铅管", (0.005 + 0.01) / 2],
    ["操作中基本无腐蚀的无缝钢管", (0.05 + 0.1) / 2],
    ["操作中有轻度腐蚀的无缝钢管", (0.1 + 0.2) / 2],
    ["操作中有显著腐蚀的无缝钢管", (0.2 + 0.5) / 2],
    ["钢板卷管", 0.33],
    ["铸铁管", (0.5 + 0.85) / 2],
    ["干净的玻璃管", (0.0015 + 0.01) / 2]
];

const LocalResistace = {
    elbow45: 0.35,
    elbow: 0.75,
    elbow2: 1.30, //90°斜接弯头
    elbow180: 1.5,
    globeValve: 6.00,
    angleValve: 3.00,
    gateValve: 0.17,
    plugValve: 0.05,
    butterflyValve: 0.24,
    checkValve0: 2.00,
    checkValve1: 10.00,
    footValve: 15.00,
};

const PipeFitting = {
    elbow45: "45°弯头",
    elbow: "90°弯头",
    elbow2: "90°斜接弯头",
    elbow180: "180°弯头"
};

const Gate = {
    globeValve: "截止阀",
    angleValve: "角阀",
    gateValve: "闸阀",
    plugValve: "旋塞阀",
    butterflyValve: "蝶阀",
    checkValve0: "旋启式止回阀",
    checkValve1: "升降式止回阀",
    footValve: "底阀"
};

/**
 * 计算管道内径（流速）
 * @param f flow [m3/h]
 * @param v flow rate [m/s]
 * @returns Pipe Internal diameter [mm]
 */
function dim_v(f:number, v:number) {
    return 18.8 * Math.sqrt(f / v);
}

/**
 * 计算管道内径（允许压降） 依据SH/T3405-2018
 * @param f flow [m3/h]
 * @param dP allow pressure drop [kPa/100m]
 * @param rho density [kg/m3]
 * @param nu Kinematic viscosity[m2/s]
 * @returns Pipe Internal diameter [mm]
 */
function dim_dP(f:number, l:number, dP:number, rho:number, nu:number){
    //nu = nu * 10000;
    //return 11.4 * rho ** 0.207 * nu ** 0.033 * f ** 0.38 * dP **-0.207;
    var di = 0.007 * rho ** 0.207 * nu ** 0.033 * l ** 0.207 * f ** 0.38 * dP **-0.207;
    return di * 1000;
}

/**
 * 计算管子重量
 * @param don Pipe outer diameter [mm]
 * @param thk Pipe wall thickness [mm]
 * @param rho density [kg/m3]
 * @returns Weight [mm]
 */
function pipeWgt_thk(don:number, thk:number, rho:number = 7.85){
    return Pi * rho * (don - thk) * thk / 1000;
}

/**
 * @param don Pipe outer diameter [mm]
 * @param thk Insultion thickness [mm]
 * @param l length [mm]
 * @return 保温体积 m3  
 */
 function VInsultion(don:number, ithk:number, l:number){
    return Pi * (don + 1.033 * ithk) * 1.033 * ithk / 1e6 * l;
}

/**
 * @param don Pipe outer diameter [mm]
 * @param thk Insultion thickness [mm]
 * @param l length [mm]
 * @return 保温表面积 m2  
 */
 function SInsultion(don:number, ithk:number, l:number){
    return Pi * (don + 2.1 * ithk + 8.2) / 1e3 * l;
}

/**
 * 雷诺数
 * @param dim Pipe Internal diameter [m]
 * @param ve flow rate [m/s]
 * @param rho density [kg/m3]
 * @param mu Dynamic viscosity [Ps.s]
 * @returns reynolds number
 */
function reynolds(dim:number, ve:number, rho:number, mu:number) {
    return dim * ve * rho / mu;
}

/**
 * 阻力系数
 * @param re reynolds number
 * @param dim  Pipe Internal diameter [m]
 * @param e Roughness [m]
 * @returns resistance coefficient
 */
function resistace(re:number, dim:number = 0, e:number = 0) {
    var lambda:number;
    if (re <= 2000) {
        lambda = 64 / re;
    }
    else if (re > 2000 && re <= 4000) {
        lambda = 0.3164 * re ** -0.25;
    }
    else if (re > 4000 && re < 396 * dim / e * Math.log10(3.7 * dim / e)) {
        var x0:number = 0, x1:number = 0.1;
        do {
            var mid = (x0 + x1) / 2;
            if (1 / mid ** 0.5 + 2 * Math.log10(e / (3.7 * dim) + 2.51 / (re * mid ** 0.5)) > 0) {
                x0 = mid;
            }else{
                x1 = mid;
            }
        } while (Math.abs(x0 - x1) > 1e-6);

        lambda = (x0+x1)/2;
    }
    else {
        var x0:number = 0, x1:number = 0.1;
        do {
            var mid = (x0 + x1) / 2;
            if (1 / Math.pow(mid, 0.5) + 2 * Math.log10(e / (3.7 * dim)) > 0) {
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
 * @param dim Pipe Internal diameter [m]
 * @param l length [m]
 * @param rho density [kg/m3]
 * @param mu Dynamic viscosity [Ps.s]
 * @returns dp [kPa]
 */
function pipeDp0(f:number, e:number, dim:number, l:number, rho:number, mu:number) {
    var ve = f / 3600 / (Pi * (dim / 2) ** 2);
    var re = reynolds(dim, ve, rho, mu);
    var lambda = resistace(re, dim, e);
    var dp0 = lambda * (l / dim) * (rho * ve ** 2 / 2) * 0.001;

    return dp0;
}

/**
 * 局部阻力 SHT3035-2018
 * @param ksum 
 * @param ve 
 * @param rho 
 * @returns 
 */
function pipeDp1(ksum:number, ve:number, rho:number) {
    var dp1 = ksum * rho * ve ** 2 / 2 * 0.001;
    return dp1;
}

/**
 GB50264-2013
 t0  管道或设备的外表面温度[C]
 ta  环境温度[C]
 tm  平均温度（绝热材料内外表面温度的算术平均值）[C]
 tA  介质（上游）A点处的温度[C]
 tB  介质（下游）B点处的温度[C]
 ts  绝热层外表面温度[C]
 d0  管道或设备外径[m]
 d1  内层绝热层外径[m]
 lambda  绝热材料在平均温度下的导热系数[W/(m*K)]
 lambda0 常用导热系数[W/(m*K)]
 alphas  绝热层外表面与周围空气的换热系数[W/(m2*K)]
 alphar  绝热结构外表面材料换热系数 [W/(m2*K)]
 alphac  对流换热系数 [W/(m2*K)]
 epsilon 绝热结构外面材料的黑度
 kr 管道通过吊架处的热损失附加系数，kr=1.1-1.2，大管取值应靠下限，小管取值应靠上限
 Lc 特征长度，d1log(d1/d0)
 w 风速[m/s]
 */

 //GB50264-2018 表5.8.9
 const BlackDegree = [
    ["铝合金薄板", (0.15 + 0.3) / 2],
    ["不锈钢薄板", (0.20 + 0.4) / 2],
    ["有光泽的镀锌钢板", (0.23 + 0.27) / 2],
    ["已氧化的镀锌钢板", (0.28 + 0.32) / 2],
    ["纤维织物", (0.70 + 0.80) / 2],
    ["水泥砂浆", 0.69],
    ["铝粉漆", 0.41],
    ["黑漆（有光泽）", 0.88],
    ["黑漆（无光泽）", 0.96],
    ["油漆", (0.80 + 0.90) / 2]
 ]

 /**
  * 外表面温度计算，公式5.3.11，假定外表面温度，迭代计算
  * @param ta 
  * @param d0 
  * @param d1 
  * @param epsilon 
  * @param w 
  * @returns 
  */
function ts_delta(t0:number,ta:number, d0:number, d1:number, epsilon:number, w:number) {
    
    let tm_tmp, ts_tmp, tss = 20, lambda_tmp, alphas_tmp
    do {
        ts_tmp = tss
        tm_tmp = (t0 + ts_tmp) / 2
        lambda_tmp = getLambda(tm_tmp)
        alphas_tmp = getAlphar(epsilon, ta, ts_tmp) + getAlphac(w,d1,ta,ts_tmp)
        tss = (2 * lambda_tmp * t0 + d1 * Math.log(d1 / d0) * alphas_tmp * ta) / (2 * lambda_tmp + d1 * Math.log(d1 / d0) * alphas_tmp)
    } while (Math.abs(tss - ts_tmp) > 0.01)

    return tss
}

/**
 * 已知Q，计算绝热结构的外表面温度，公式5.5.1
 * @param Q 
 * @param ta 
 * @param alphas 
 * @returns 
 */
 function ts_Q(Q:number, ta:number, alphas:number){
    return Q / alphas + ta
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
function lc_Q(t0:number, ta:number, Q:number, lambda:number, alphas:number) {
    return 2 * lambda * ((t0 - ta) / Q - 1 / alphas)
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
function lc_ts(t0:number, ta:number, ts:number, lambda:number, alphas:number) {
    return 2 * lambda / alphas * (t0 - ts) / (ts - ta)
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
function Qs(t0:number, ta:number, d0:number, d1:number, lambda:number, alphas:number) {
    return (t0 - ta) / (d1 / (2 * lambda) * Math.log(d1 / d0) + 1 / alphas)
}

/**
 * 每米管道热损失量计算，公式5.4.3-2
 * @param Q 
 * @param d1 
 * @returns 单位长度散热量[W/m]
 */
function q_Q(Q:number, d1:number) {
    return Math.PI * d1 * Q
}

/**
 * 辐射换热系数计算，公式5.8.4-1
 * @param epsilon 
 * @param ta 
 * @param ts 
 * @returns 
 */
function getAlphar(epsilon:number, ta:number, ts:number) {
    return 5.669 * epsilon / (ts - ta) * (Math.pow(((273 + ts) / 100), 4) - Math.pow(((273 + ta) / 100), 4))
}

/**
 * 对流换热系数计算，公式5.8.4-2、5.8.4-3、5.8.4-4
 * @param w 
 * @param d1 
 * @param ta 
 * @param ts 
 * @returns 
 */
function getAlphac(w:number, d1:number, ta = 0, ts = 0){
    if (w == 0) {
        return 26.4 / Math.sqrt(297 + 0.5 * (ts + ta)) * Math.pow(((ts - ta) / d1), 0.25)
    } else {
        return w * d1 > 0.8 ? 4.53 * Math.pow(w, 0.805) / Math.pow(d1, 0.195) : (0.08 / d1 + 4.2 * Math.pow(w, 0.618) / Math.pow(d1, 0.382))
    }
}

/**
 * 表面换热系数计算
 * @param alphar 
 * @param alphac 
 * @returns 
 */
function getAlphas(alphar:number, alphac:number){
    return alphar + alphac
}

/**
 *  表面换热系数计算，防烫保温
 * @returns 
 */
function getAlphas2(){
    return 8.141
}

/**
 * 绝热材料在平均温度下的导热系数计算仅针对硅酸铝棉及其制品，附录A
 * @param tm 
 * @param lambda0 
 * @returns 
 */
function getLambda(tm:number, lambda0 = 0.044) {
    var lambdal = lambda0 + 0.0002 * (tm - 70)
    var lambdah = lambdal + 0.00036 * (tm -400)

    return tm > 400 ? lambdah : lambdal
}


function qFin(hf:number, l:number,t0:number, ta:number, ithk:number) {
    var k = -0.0465 * t0 + 61.697;
    var h = 25; //表面传热系数[W/m2.K]
    var thk:number;
    switch (hf) {
        case 0.1:
            thk = 0.007;
            break;
        case 0.15:
            thk = 0.0095;
            break;
        case 0.2:
            thk = 0.01;
            break;    
        default:
            thk = 0.007;
            break;
    }

    var H = hf - ithk;  //只计算未保温管托散热
    var A0 = l * thk;
    var P = 2 * (l + thk);
    var m = Math.pow((h * P) / (k * A0), 0.5);
    var theta0 = t0 - ta;
    var q = h * P / m * theta0 * Math.tanh(m * H);
    return q;
}

/**
 * 
 * @param d0 
 * @param Lc 
 * @returns 
 */
// function getD1_Lc(d0:number, Lc:number) {
//     let d1_min = 0, d1_max = 3000, d1_tmp:number, Lc_tmp = 0
//     while(Math.abs(Lc - Lc_tmp) > 0.01){
//         d1_tmp = (d1_tmp + d1_max) / 2
//         Lc_tmp = d1_tmp * Math.log(d1_tmp / d0)
//         if(lc_tmp < Lc){
//             d1_min = d1_tmp
//         }else{
//             d1_max = d1_tmp
//         }
//     }

//     return d1_tmp
// }

/**
 * 计算管道末端介质焓值
 * @param hA 始端焓值[kj/kg]
 * @param f 流量[kg/s]
 * @param qt 总散热量[W/m]
 * @param l 管道长度[m]
 * @returns 末端焓值 kj/kg 
 */
function getHB(hA:number, f:number, qt:number){
    return (hA * 1000 - qt / f) / 1000
}

/**
 * 计算蒸汽温度
 * @param h 末端焓值[kj/kg]
 * @param p 末端压力[MPaA]
 * @returns 介质温度 [K]
 */
function getT(h:number, p:number){
    var w = new IAPWS97();

    return w.solve({p:p, h:h}).t;
}


/**
 * 计算蒸汽焓值
 * @param t 温度[K]
 * @param p 压力[MPaA]
 * @returns 焓值[kj/kg]
 */
function getH(t:number,p:number){
    var w = new IAPWS97();

    return w.solve({t:t, p:p}).h;
}

/**
 * 仅限于蒸汽
 * @param t_A 始端温度[℃]
 * @param h_A 始端压力[MPaA]
 * @param p_B 末端压力[MPaA]
 * @param ta 环境温度[℃]
 * @param d0 管道外径[m]
 * @param d1 保温外径[m]
 * @param epsilon 黑度
 * @param w 风速[m/s]
 * @param f 流量[kg/s]
 * @param l 管道长度[m]
 * @returns 管道或设备外表面平均温度[℃]
 */
function getT0(t_A:number, h_A:number, p_B:number, ta:number, d0:number, d1:number, epsilon:number, w:number, f:number, l:number){
    var t0 = t_A, ts, tm, lambda, alphar,alphac,alphas,Q,q;
    var t0_tmp// = t_A;
    do {
        t0_tmp = t0;
        ts = ts_delta(t0,ta,d0,d1,epsilon,w);
        tm = (t0 + ts) / 2;
        lambda = getLambda(tm);
        alphar = getAlphar(epsilon,ta,ts);
        alphac = getAlphac(w,d1);
        alphas = getAlphas(alphar,alphac)
       
        Q = Qs(t0,ta,d0,d1,lambda,alphas);
        q = q_Q(Q,d1);
        
        var h_B = getHB(h_A,f,q * l);
        var t_B = getT(h_B,p_B) - 273.15;
        
        t0 = (t_A + t_B) / 2;
        console.log("t0",t0);
    } while (Math.abs(t0 - t0_tmp) > 0.01)

    return t0;
}