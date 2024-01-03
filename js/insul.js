"use strict"

/**
 * y = xln(x/c)方程，求解x
 * @param {number} y 
 * @param {number} c 
 * @returns {number} x
 */
function xlnxdc_(y, c) {
    let x0 = c, x1 = 100;
    do {
        let tmp = 0.5 * (x0 + x1);
        if (tmp * Math.log(tmp / c) < y) {
            x0 = tmp;
        } else {
            x1 = tmp;
        }
    } while (Math.abs(x0 - x1) > 1e-6);
    return x0;
}

/**
 * y = ln(x/c)+a/x，求解x
 * @param {number} y 
 * @param {number} c 
 * @param {number} a 
 * @returns {number} x
 */
function lnxdcpax_(y, c, a) {
    let x0 = c, x1 = 100;
    do {
        let tmp = 0.5 * (x0 + x1);
        if (Math.log(tmp / c) + a / tmp < y) {
            x0 = tmp;
        } else {
            x1 = tmp;
        }
    } while (Math.abs(x0 - x1) > 1e-6);
    return x0;
}

/**
 * 圆筒型，绝热层经济厚度计算，GB50264-2013 式5.3.2-1
 * @param {number} d0 管道或设备外径 m
 * @param {number} pe 能量价格 元/GJ
 * @param {number} pt 绝热结构单位造价 元/m3
 * @param {number} t 年运行时间 s
 * @param {number} s 绝热工程投资年摊销率 %
 * @param {number} t0 管道或设备的外表面温度 K
 * @param {number} ta 环境温度 K
 * @param {number} lambda 绝热材料在平均温度下的导热系数 W/(m.K)
 * @param {number} alphas 绝热层外表面与周围空气的换热系数 W/(m2.K)
 * @returns {number} 经济保温厚度，保温外径 m
 */
function d1_eco(d0, pe, pt, t, s, t0, ta, lambda, alphas) {
    let y = 3.795e-3 * Math.sqrt(pe * lambda * t * Math.abs(t0 - ta) / (pt * s)) - 2 * lambda / alphas;
    return xlnxdc_(y, d0);
}

/**
 * 平面型，绝热层经济厚度计算，GB50264-2013 式5.3.2-2
 * @param {number} pe 能量价格 元/GJ
 * @param {number} pt 绝热结构单位造价 元/m3
 * @param {number} t 年运行时间 s
 * @param {number} s 绝热工程投资年摊销率 %
 * @param {number} t0 管道或设备的外表面温度 K
 * @param {number} ta 环境温度 K
 * @param {number} lambda 绝热材料在平均温度下的导热系数 W/(m.K)
 * @param {number} alphas 绝热层外表面与周围空气的换热系数 W/(m2.K)
 * @returns {number} 经济保温厚度 m
 */
function delta_eco(pe, pt, t, s, t0, ta, lambda, alphas) {
    return 1.8975e-3 * Math.sqrt(pe * lambda * t * Math.abs(t0 - ta) / (pt * s)) - lambda / alphas;
}

/**
 * 圆筒型，最大允许热、冷损失下的绝热层厚度，GB50264-2013 式5.3.3-1
 * @param {number} Qmax 每平方米绝热层外表面积最大允许冷、热损失量，保温为正直，保冷为负值 w/m2
 * @param {number} d0 管道或设备外径 m
 * @param {number} t0 管道或设备的外表面温度 K
 * @param {number} ta 环境温度 K
 * @param {number} lambda 绝热材料在平均温度下的导热系数 W/(m.K)
 * @param {number} alphas 绝热层外表面与周围空气的换热系数 W/(m2.K)
 * @returns {number} 保温外径 m
 */
function d1_Qmax(Qmax, d0, t0, ta, lambda, alphas) {
    let y = 2 * lambda ((t0 - ta) / Qmax - 1 / alphas);
    return xlnxdc_(y, d0);
}

/**
 * 圆筒型，最大允许热、冷损失下的绝热层厚度，GB50264-2013 式5.3.3-2
 * @param {number} qmax 每米管道长度最大允许冷、热损失量，保温为正直，保冷为负值 w/m
 * @param {number} d0 管道或设备外径 m
 * @param {number} t0 管道或设备的外表面温度 K
 * @param {number} ta 环境温度 K
 * @param {number} lambda 绝热材料在平均温度下的导热系数 W/(m.K)
 * @param {number} alphas 绝热层外表面与周围空气的换热系数 W/(m2.K)
 * @returns {number} 保温外径 m
 */
function d1_qmax(qmax, d0, t0, ta, lambda, alphas) {
    let y = 2 * Math.PI * lambda * (t0 - ta) / qmax;

    // let x0 = d0, x1 = 100;
    // do {
    //     let tmp = 0.5 * (x0 + x1);
    //     if (Math.log(temp / d0) + 2 * lambda / (tmp * alphas) < y) {
    //         x0 = tmp;
    //     } else {
    //         x1 = tmp;
    //     }
    // } while (Math.abs(x0 - x1) < 1e-6);
    // return x0;
    return lnxdcpax_(y, d0, 2 * lambda / alphas);
}

/**
 * 圆筒型，不同材料双层保温，最大允许热、冷损失下的绝热层厚度，GB50264-2013 式5.3.4-1
 * @param {number} Qmax 每平方米绝热层外表面积最大允许冷、热损失量，保温为正直，保冷为负值 w/m2
 * @param {number} d0 管道或设备外径 m
 * @param {number} t0 管道或设备的外表面温度 K
 * @param {number} t1 内层绝热层外表面温度 K
 * @param {number} ta 环境温度 K
 * @param {number} lambda1 内层绝热材料导热系数 W/(m.K)
 * @param {number} lambda2 外层绝热材料导热系数 W/(m.K)
 * @param {number} alphas 绝热层外表面与周围空气的换热系数 W/(m2.K)
 * @returns {number} 外层绝热层外径 m
 */
function d2_Qmax_2(Qmax, d0, t0, t1, ta, lambda1, lambda2, alphas) {
    let y = 2 * ((lambda1 * (t0 - t1) + lambda2 * (t1 - ta)) / Qmax - lambda2 / alphas);
    return xlnxdc_(y, d0);
}

/**
 * 圆筒型，不同材料双层保温，最大允许热、冷损失下的绝热层厚度，GB50264-2013 式5.3.4-2
 * @param {number} Qmax 每平方米绝热层外表面积最大允许冷、热损失量，保温为正直，保冷为负值 w/m2
 * @param {number} d0 管道或设备外径 
 * @param {number} d2 外层绝热层外径 m
 * @param {number} t0 管道或设备的外表面温度 K
 * @param {number} t1 内层绝热层外表面温度 K 
 * @param {number} lambda1 内层绝热材料导热系数 W/(m.K)
 * @returns {number} 内层绝热层外径 m
 */
function d1_Qmax_2(Qmax, d0, d2, t0, t1, lambda1) {
    let y = 2 * lambda1 / d2 * (t0 - t1) / Qmax; //y = ln(d1/d0)
    return Math.exp(y) * d0;
}

/**
 * 圆筒型，不同材料双层保温，最大允许热、冷损失下的绝热层厚度，GB50264-2013 式5.3.4-3
 * @param {number} qmax 每米管道长度最大允许冷、热损失量，保温为正直，保冷为负值 w/m
 * @param {number} d0 管道或设备外径 m
 * @param {number} t0 管道或设备的外表面温度 K
 * @param {number} t1 内层绝热层外表面温度 K
 * @param {number} ta 环境温度 K
 * @param {number} lambda1 内层绝热材料导热系数 W/(m.K)
 * @param {number} lambda2 外层绝热材料导热系数 W/(m.K)
 * @param {number} alphas 绝热层外表面与周围空气的换热系数 W/(m2.K)
 * @returns {number} 外层绝热层外径 m
 */
function d2_qmax_2(qmax, d0, t0, t1, ta, lambda1, lambda2, alphas) {
    let y = 2 * Math.PI(lambda1 * (t0 - t1) + lambda2 * (t1 - ta)) / qmax;

    // let x0 = d0, x1 = 100;
    // do {
    //     let tmp = 0.5 * (x0 + x1);
    //     if (Math(tmp / d0) + 2 * lambda2 / (d2 * alphas) < y) {
    //         x0 = tmp;
    //     } else {
    //         x1 = tmp;
    //     }
    // } while (Math.abs(x0 - x1));
    // return x0;
    return lnxdcpax_(y, d0, 2 * lambda2 / alphas);
}

/**
 * 圆筒型，不同材料双层保温，最大允许热、冷损失下的绝热层厚度，GB50264-2013 式5.3.4-4
 * @param {number} qmax 每米管道长度最大允许冷、热损失量，保温为正直，保冷为负值 w/m
 * @param {number} d0 管道或设备外径 m
 * @param {number} t0 管道或设备的外表面温度 K
 * @param {number} t1 内层绝热层外表面温度 K
 * @param {number} lambda1 内层绝热材料导热系数 W/(m.K)
 * @returns {number} 内层绝热层外径 m
 */
function d1_qmax_2(qmax, d0, t0, t1, lambda1) {
    let y = 2 * Math.PI * lambda1 * (t0 - t1) / qmax;
    return Math.exp(y) * d0;
}

/**
 * 平面型，单层保温，最大允许热、冷损失下的绝热层厚度，GB50264-2013 式5.3.5
 * @param {number} Qmax 每平方米绝热层外表面积最大允许冷、热损失量，保温为正直，保冷为负值 w/m2
 * @param {number} t0 设备的外表面温度 K
 * @param {number} ta 环境温度 K
 * @param {number} lambda 绝热材料在平均温度下的导热系数 W/(m.K)
 * @param {number} alphas 绝热层外表面与周围空气的换热系数 W/(m2.K)
 * @returns {number} 绝热层材料厚度 m
 */
function delta_Qmax(Qmax, t0, ta, lambda, alphas) {
    return lambda * ((t0 - ta) / Qmax - 1 / alphas);
}

/**
 * 平面型，不同材料双保温，最大允许热、冷损失下的绝热层厚度，GB50264-2013 式5.3.6-1
 * @param {number} Qmax 每平方米绝热层外表面积最大允许冷、热损失量，保温为正直，保冷为负值 w/m2
 * @param {number} t0 设备的外表面温度 K
 * @param {number} t1 内层绝热层外表面温度 K
 * @param {number} lambda1 内层绝热材料导热系数 W/(m.K)
 * @returns {number} 内层绝热层厚度 m
 */
function delta1_Qmax_2(Qmax, t0, t1, lambda1) {
    return lambda1 * (t0 - t1) / Qmax;
}

/**
 * 平面型，不同材料双保温，最大允许热、冷损失下的绝热层厚度，GB50264-2013 式5.3.6-2
 * @param {number} Qmax 每平方米绝热层外表面积最大允许冷、热损失量，保温为正直，保冷为负值 w/m2
 * @param {number} t1 内层绝热层外表面温度 K
 * @param {number} ta 环境温度 K
 * @param {number} lambda2 外层绝热材料导热系数 W/(m.K)
 * @param {number} alphas 绝热层外表面与周围空气的换热系数 W/(m2.K)
 * @returns {number} 外层绝热层厚度 m
 */
function delta2_Qmax_2(Qmax, t1, ta, lambda2, alphas) {
    return lambda2 * ((t1 - ta) / Qmax - 1 / alphas);
}

/**
 * 圆筒型，单层防止绝热层外表面结露，绝热层厚度计算，GB50264-2013 式5.3.7
 * @param {number} d0 管道或设备外径 m
 * @param {number} t0 管道或设备的外表面温度 K
 * @param {number} ts 保冷层表面温度 K
 * @param {number} ta 环境温度 K
 * @param {number} lambda 绝热材料导热系数 W/(m.K)
 * @param {number} alphas 绝热层外表面与周围空气的换热系数 W/(m2.K)
 * @returns {number} 绝热层外径 m
 */
function d1_dew(d0, t0, ts, ta, lambda, alphas) {
    let y = 2 * lambda / alphas * (ts - t0) / (ta - ts);
    return xlnxdc_(y, d0);
}

/**
 * 圆筒型，不同材料双层防止绝热层外表面结露，绝热层厚度计算，GB50264-2013 式5.3.8-1
 * @param {number} d0 管道或设备外径 m
 * @param {number} t0 管道或设备的外表面温度 K
 * @param {number} t1 内层绝热层外表面温度 K
 * @param {number} ts 保冷层表面温度 K
 * @param {number} ta 环境温度 K
 * @param {number} lambda1 内层绝热材料导热系数 W/(m.K)
 * @param {number} lambda2 外层绝热材料导热系数 W/(m.K)
 * @param {number} alphas 绝热层外表面与周围空气的换热系数 W/(m2.K)
 * @returns {number} 外层绝热层外径 m 
 */
function d2_dew_2(d0, t0, t1, ts, ta, lambda1, lambda2, alphas) {
    let y = 2 / alphas * (lambda1 * (t1 - t0) + lambda2 * (ts - t1)) / (ta - ts);
    return xlnxdc_(y, d0);
}

/**
 * 圆筒型，不同材料双层防止绝热层外表面结露，绝热层厚度计算，GB50264-2013 式5.3.8-2
 * @param {number} d0 管道或设备外径 m
 * @param {number} d2 外层绝热层外径 m
 * @param {number} t0 管道或设备的外表面温度 K
 * @param {number} t1 内层绝热层外表面温度 K
 * @param {number} ts 保冷层表面温度 K
 * @param {number} ta 环境温度 K
 * @param {number} lambda1 内层绝热材料导热系数 W/(m.K) 
 * @param {number} alphas 绝热层外表面与周围空气的换热系数 W/(m2.K)
 * @returns {number} 内层绝热层外径 m
 */
function d1_dew_2(d0, d2, t0, t1, ts, ta, lambda1, alphas) {
    let y = 2 * lambda1 / (d2 * alphas) * (t1 - t0) / (ta - ts);
    return Math.exp(y) * d0;
}

/**
 * 圆筒型，不同材料双层防止绝热层外表面结露，绝热层厚度计算，GB50264-2013 式5.3.8-3
 * @param {number} d1 内层绝热层外径 m
 * @param {number} t1 内层绝热层外表面温度 K
 * @param {number} ts 保冷层表面温度 K
 * @param {number} ta 环境温度 K
 * @param {number} lambda1 内层绝热材料导热系数 W/(m.K) 
 * @param {number} alphas 绝热层外表面与周围空气的换热系数 W/(m2.K)
 * @returns {number} 外层绝热层外径 m 
 */
function d2_d1_dew_2(d1, t1, ts, ta, lambda2, alphas) {
    let x0 = d1, x1 = 100;
    do {
        let tmp = 0.5 * (x0 + x1);
        if (Math.log(tmp / d1) < 2 * lambda2 / (tmp * alphas) * ((ts - t1) / (ta - ts))) {
            x0 = tmp;
        } else {
            x1 = tmp;
        }
    } while (Math.abs(x0 - x1) < 1e-6);
    return x0;
}

/**
 * 平面型，单层防结露保冷层厚度，GB50264-2013 式5.3.9
 * @param {number} t0 设备的外表面温度 K
 * @param {number} ts 保冷层表面温度 K
 * @param {number} ta 环境温度 K
 * @param {number} lambda 绝热材料导热系数 W/(m.K) 
 * @param {number} alphas 绝热层外表面与周围空气的换热系数 W/(m2.K)
 * @returns {number} 保冷层厚度 m
 */
function delta_dew(t0, ts, ta, lambda, alphas) {
    return lambda / alphas * (ts - t0) / (ta - ts);
}

/**
 * 平面型，不同材料双层防结露保冷层厚度，GB50264-2013 式5.3.10-1
 * @param {number} t0 设备的外表面温度 K
 * @param {number} t1 内层保冷层外表面温度 K
 * @param {number} ts 保冷层表面温度 K
 * @param {number} ta 环境温度 K
 * @param {number} lambda1 内层绝热材料导热系数 W/(m.K) 
 * @param {number} alphas 绝热层外表面与周围空气的换热系数 W/(m2.K)
 * @returns {number} 内层保冷层厚度 m
 */
function delta1_dew_2(t0, t1, ts, ta, lambda1, alphas) {
    return lambda1 / alphas * (t1 - t0) / (ta - ts);
}

/**
 * 平面型，不同材料双层防结露保冷层厚度，GB50264-2013 式5.3.10-2
 * @param {number} t1 内层保冷层外表面温度 K
 * @param {number} ts 保冷层表面温度 K
 * @param {number} ta 环境温度 K
 * @param {number} lambda2 外层绝热材料导热系数 W/(m.K) 
 * @param {number} alphas 绝热层外表面与周围空气的换热系数 W/(m2.K)
 * @returns {number} 外层保冷层厚度 m
 */
function delta2_dew_2(t1, ts, ta, lambda2, alphas) {
    return lambda2 / alphas * (ts - t1) / (ta - ts);
}

/**
 * 圆筒型，表面温度法计算绝热层厚度，GB50264-2013 式5.3.11
 * @param {number} d0 管道或设备外径 m
 * @param {number} t0 管道或设备的外表面温度 K
 * @param {number} ts 绝热层表面温度 K，防烫保温可取60
 * @param {number} ta 环境温度 K
 * @param {number} lambda 绝热材料导热系数 W/(m.K) 
 * @param {number} alphas 绝热层外表面与周围空气的换热系数 W/(m2.K)
 * @returns {number} 绝热层外径 m
 */
function d1_ts(d0, t0, ts, ta, lambda, alphas) {
    let y = 2 * lambda / alphas * (t0 - ts) / (ts - ta);
    return xlnxdc_(y, d0);
}

/**
 * 平面型，表面温度法计算绝热层厚度，GB50264-2013 式5.3.12
 * @param {number} t0 管道或设备的外表面温度 K
 * @param {number} ts 绝热层表面温度 K，防烫保温可取60
 * @param {number} ta 环境温度 K
 * @param {number} lambda 绝热材料导热系数 W/(m.K) 
 * @param {number} alphas 绝热层外表面与周围空气的换热系数 W/(m2.K)
 * @returns {number} 绝热层厚度 m
 */
function delta_ts(t0, ts, ta, lambda, alphas) {
    return lambda / alphas * (t0 - ts) / (ts - ta);
}

/**
 * 延迟管道内介质冻结、凝固、结晶的保温厚度，GB50264-2013 式5.3.13
 * @param {number} d0 管道或设备外径 m
 * @param {number} timefr 介质在管道内不出现冻结的停留时间 s
 * @param {number} t0 管道或设备的外表面温度 K
 * @param {number} tfr 介质凝固点 K
 * @param {number} ta 环境温度 K，室外温度应取冬季极端平均最低温度
 * @param {number} lambda 绝热材料导热系数 W/(m.K) 
 * @param {number} alphas 冬季最多风向平均风速下绝热层外表面与周围空气的换热系数 W/(m2.K)
 * @param {number} v 介质单位长度体积单位长度体积 m3/m
 * @param {number} rho 介质密度和管壁密度kg/m3
 * @param {number} c 介质热容 J/(kg.K)
 * @param {number} vp 管壁单位长度体积 m3/m
 * @param {number} rhop 管壁密度 kg/m3
 * @param {number} cp 管壁热容 J/(kg.K
 * @param {number} kr 管道通过吊架处的热损失附加系数，K =l.l~l.2，大管取值应靠下限，小管取值应靠上限
 * @returns {number} 保温外径 m
 */
function d1_freeze(d0, timefr, t0, tfr, ta, lambda, alphas, v, rho, c, vp, rhop, cp, kr) {
    let y = 2 * kr * Math.PI * lambda * timefr / (v * rho * c + vp) * Math.log((t0 - ta) / (tfr - ta));
    return lnxdcpax_(y, d0, 2 * lambda / alphas);
}

/**
 * 给定液体管道允许温度降的保温厚度，GB50264-2013 式5.3.14-1
 * @param {number} di 管道内径 m
 * @param {number} d0 管道或设备外径 m
 * @param {number} l_AB A、B之间管道的实际长度 m
 * @param {number} t_A 介质在（上游）A点处的温度 K
 * @param {number} t_B 介质在（下游）B点处的温度 K
 * @param {number} ta 环境温度 K，室外温度应取冬季极端平均最低温度
 * @param {number} lambda 绝热材料导热系数 W/(m.K) 
 * @param {number} alphas 冬季最多风向平均风速下绝热层外表面与周围空气的换热系数 W/(m2.K)
 * @param {number} w 介质流速 m/s
 * @param {number} rho 介质密度 kg/m3
 * @param {number} c 介质热容 J/(kg.K)
 * @param {number} kr 管道通过吊架处的热损失附加系数，K =l.l~l.2，大管取值应靠下限，小管取值应靠上限
 * @returns {number} 保温外径 m
 */
function d1_dt(di, d0, l_AB, t_A, t_B, ta, lambda, alphas, w, rho, c, kr) {
    let y = 8 * lambda * l_AB * kr / (di * di * w * rho * c * Math.log((t_A - ta) / (t_B - ta)));
    return lnxdcpax_(y, d0, 2 * lambda / alphas);
}

/**
 * 球形容器保冷层厚度，GB50264-2013 式5.3.15
 * @param {number} d0 设备直径 m
 * @param {number} t0 设备的外表面温度 K
 * @param {number} ts 绝热层表面温度 K
 * @param {number} ta 环境温度 K
 * @param {number} lambda 绝热材料导热系数 W/(m.K)
 * @param {number} alphas 绝热层外表面与周围空气的换热系数 W/(m2.K)
 * @returns {number} 保冷层外径 m
 */
function d1_ball(d0, t0, ts, ta, lambda, alphas) {
    return (0.5 + Math.sqrt(0.25 + 2 / d0 * (lambda / alphas * (t0 - ts) / (ts - ta)))) / (1 / d0);
}

/**
 * 最大允许热损失量，GB50264-2013 附录B
 * @param {number} t0 设备的外表面温度 K
 * @param {string} tt 全年运行tt="yearly"，季节运行tt="quarter"
 * @returns {number} 最大允许热损失 W/m2
 */
function Qmax_hot(t0, tt) {
    t0 -= 273.15;
    switch (tt) {
        case "yearly":
            if (t0 <= 50) {
               return 52; 
            }
            else if (t0 > 50 && t0 <= 100) {
                return 84;
            }
            else if (t0 > 100 && t0 <= 150) {
                return 104;
            }
            else if (t0 > 150 && t0 <= 200) {
                return 106;
            }
            else if (t0 > 200 && t0 <= 250) {
                return 147;
            }
            else if (t0 > 250 && t0 <= 300) {
                return 167;
            }
            else if (t0 > 300 && t0 <= 350) {
                return 188;
            }
            else if (t0 > 350 && t0 <= 400) {
                return 204;
            }
            else if (t0 > 400 && t0 <= 450) {
                return 220;
            }
            else if (t0 > 450 && t0 <= 500) {
                return 236;
            }
            else if (t0 > 500 && t0 <= 550) {
                return 251;
            }
            else if (t0 > 550 && t0 <= 600) {
                return 266;
            }
            else if (t0 > 600 && t0 <= 650) {
                return 283;
            }
            else if (t0 > 650 && t0 <= 700) {
                return 297;
            }
            else if (t0 > 700 && t0 <= 750) {
                return 311;
            }
            else if (t0 > 750 && t0 <= 800) {
                return 324;
            }
            else if (t0 > 800 && t0 <= 850) {
                return 338;
            }
            break;
        case "quarter":
            if (t0 < 50) {
                return 104; 
             }
             else if (t0 > 50 && t0 <= 100) {
                 return 147;
             }
             else if (t0 > 100 && t0 <= 150) {
                 return 183;
             }
             else if (t0 > 150 && t0 <= 200) {
                 return 220;
             }
             else if (t0 > 200 && t0 <= 250) {
                 return 251;
             }
             else if (t0 > 250 && t0 <= 300) {
                 return 272;
             }
             break;
        default:
            break;
    }
}

/**
 * 最大允许冷损失量，GB50264-2013 式5.4.2
 * @param {number} 环境温度 K
 * @param {number} td 最热月露点温度 K
 * @param {number} alphas 绝热层外表面与周围空气的换热系数 W/(m2.K)
 * @returns {number} 最大允许冷损失 W/m2
 */
function Qmax_cool(ta, td, alphas) {
    if (ta - ta <= 4.5) {
        return -(ta - td) * alphas;
    } else {
        return -4.5 * alphas;
    }  
}

/**
 * 圆筒型，单层绝热结构，以每平方米绝热层外表面积表示的冷、热损失量，GB50264-2013 式5.4.3-1
 * @param {number} d0 管道或设备外径 m
 * @param {number} d1 绝热层外径 m
 * @param {number} t0 管道或设备的外表面温度 K
 * @param {number} ta 环境温度 K
 * @param {number} lambda 绝热材料导热系数 W/(m.K)
 * @param {number} alphas 绝热层外表面与周围空气的换热系数 W/(m2.K)
 * @returns {number} 冷、热损失量 W/m2
 */
function Q(d0, d1, t0, ta, lambda, alphas) {
    return (t0 - ta) / (d1 / (2 * lambda) * Math.log(d1 / d0) + 1 / alphas);
}

/**
 * 圆筒型，单层绝热结构，以每米管道长度表示的冷、热损失量，GB50264-2013 式5.4.3-2
 * @param {number} d1 绝热层外径 m
 * @param {number} Q 以每平方米绝热层外表面积表示的热损失量，W/m2
 * @returns {number} 冷、热损失量 W/m
 */
function q(d1, Q) {
    return Math.PI * d1 * Q;
}

/**
 * 圆筒型，不同材料双层绝热结构，以每平方米绝热层外表面积表示的冷、热损失量，GB50264-2013 式5.4.3-3
 * @param {number} d0 管道或设备外径 m
 * @param {number} d1 内层绝热层外径 m
 * @param {number} d2 外层绝热层外径 m
 * @param {number} t0 管道或设备的外表面温度 K
 * @param {number} ta 环境温度 K
 * @param {number} lambda1 内层绝热材料导热系数 W/(m.K)
 * @param {number} lambda2 外层绝热材料导热系数 W/(m.K)
 * @param {number} alphas 绝热层外表面与周围空气的换热系数 W/(m2.K)
 * @returns {number} 冷、热损失量 W/m2
 */
function Q_2(d0, d1, t0, ta, lambda1, lambda2, alphas) {
    return (t0 - ta) / (d2 / (2 * lambda1) * Math.log(d1 / d0) + d2 / (2 * lambda2) * Math.log(d2 / d1) + 1 / alphas);
}

/**
 * 圆筒型，不同材料双层绝热结构，以每米管道长度表示的冷、热损失量，GB50264-2013 式5.4.3-4
 * @param {number} d2 外绝热层外径 m
 * @param {number} Q 以每平方米绝热层外表面积表示的热损失量，W/m2
 * @returns {number} 冷、热损失量 W/m
 */
function q_2(d2, Q) {
    return Math.PI * d2 * Q;
}

/**
 * 平面型，单层绝热结构，以每平方米绝热层外表面积表示的冷、热损失量，GB50264-2013 式5.4.3-5
 * @param {number} delta 绝热层厚度 m
 * @param {number} t0 设备的外表面温度 K
 * @param {number} ta 环境温度 K
 * @param {number} lambda 绝热材料导热系数 W/(m.K)
 * @param {number} alphas 绝热层外表面与周围空气的换热系数 W/(m2.K)
 * @returns {number} 冷、热损失量 W/m2
 */
function Q_flat(delta, t0, ta, lambda, alphas) {
    return (t0 - ta) / (delta / lambda + 1 / alphas);
}

/**
 * 平面型，不同材料双层绝热结构，以每平方米绝热层外表面积表示的冷、热损失量，GB50264-2013 式5.4.3-5
 * @param {number} delta1 内层绝热层厚度 m
 * @param {number} delta2 外层绝热层厚度 m
 * @param {number} t0 设备的外表面温度 K
 * @param {number} ta 环境温度 K
 * @param {number} lambda1 内层绝热材料导热系数 W/(m.K)
 * @param {number} lambda2 外层绝热材料导热系数 W/(m.K)
 * @param {number} alphas 绝热层外表面与周围空气的换热系数 W/(m2.K)
 * @returns {number} 冷、热损失量 W/m2
 */
function Q_flat_2(delta1, delta2, t0, ta, lambda1, lambda2, alphas) {
    return (t0 - ta) / (delta1 / lambda1 + delta2 / lambda2 + 1 / alphas);
}

/**
 * 球型，单层绝热结构，以每平方米绝热层外表面积表示的冷损失量，GB50264-2013 式5.4.3-7
 * @param {number} d1 绝热层外径 m
 * @param {number} ts 绝热层外表面温度 K
 * @param {number} ta 环境温度 K
 * @param {number} alphas 绝热层外表面与周围空气的换热系数 W/(m2.K)
 * @returns {number} 冷损失量 W/m2
 */
function Q_ball(d1, ts, ta, alphas) {
    return Math.PI * d1 * d1 * alphas * (ts - ta)
}

/**
 * 圆筒、平面，其单、双层绝热结构的外表面温度，GB50264-2013 式5.5.1
 * @param {number} Q 以每平方米绝热层外表面积表示的冷、热损失量 W/m2
 * @param {number} ta 环境温度 K
 * @param {number} alphas 绝热层外表面与周围空气的换热系数 W/(m2.K)
 * @returns {number} 绝热结构的外表面温度 K
 */
function ts_Q(Q, ta, alphas) {
    return Q / alphas + ta;
}

/**
 * 圆筒、其单、双层绝热结构的外表面温度，GB50264-2013 式5.5.2
 * @param {number} q 以每平方米绝热层外表面积表示的冷、热损失量 W/m2
 * @param {number} d2 外绝热层外径 m
 * @param {number} ta 环境温度 K
 * @param {number} alphas 绝热层外表面与周围空气的换热系数 W/(m2.K)
 * @returns {number} 绝热结构的外表面温度 K
 */
function ts_q(q, d2, ta, alphas) {
    return q / (Math.PI * d2 * alphas) + ta;
}

/**
 * 球形，其单层绝热结构的外表面温度，GB50264-2013 式5.5.3
 * @param {number} Q 以每平方米绝热层外表面积表示的冷、热损失量 W/m2
 * @param {number} d1 绝热层外径 m
 * @param {number} ta 环境温度 K
 * @param {number} alphas 绝热层外表面与周围空气的换热系数 W/(m2.K)
 * @returns {number} 绝热结构的外表面温度 K
 */
function ts_ball_Q(Q, d1, ta, alphas) {
    return Q / (Math.PI * d1 * d1 * alphas) + ta;
}

/**
 * 圆筒型，不同材料双层绝热结构层间界面处温度，GB50264-2013 式5.6.1
 * @param {number} d0 管道或设备外径 m
 * @param {number} d1 内层绝热层外径 m
 * @param {number} d2 外层绝热层外径 m
 * @param {number} t0 管道或设备的外表面温度 K
 * @param {number} ts 绝热层外表面温度 K
 * @param {number} lambda1 内层绝热材料导热系数 W/(m.K)
 * @param {number} lambda2 外层绝热材料导热系数 W/(m.K)
 * @returns {number} 绝热结构层间界面处温度 K
 */
function t1(d0, d1, d2, t0, ts, lambda1, lambda2) {
    return (lambda1 * t0 * Math.log(d2 / d1) + lambda2 * ts * Math.log(d1 / d0)) / (lambda1 * Math.log(d2 / d1) + lambda2 * Math.log(d1 / d0));
}

/**
 * 平面型，不同材料双层绝热结构层间界面处温度，GB50264-2013 式5.6.2
 * @param {number} delta1 内层绝热层厚度 m
 * @param {number} delta2 外层绝热层厚度 m
 * @param {number} t0 管道或设备的外表面温度 K
 * @param {number} ts 绝热层外表面温度 K
 * @param {number} lambda1 内层绝热材料导热系数 W/(m.K)
 * @param {number} lambda2 外层绝热材料导热系数 W/(m.K)
 * @returns {number} 绝热结构层间界面处温度 K
 */
function t1_flat(delta1, delta2, t0, ts, lambda1, lambda2) {
    return (lambda1 * t0 * delta2 + lambda2 * ts * lambda1) / (lambda1 * delta2 + lambda2 * lambda1);
}