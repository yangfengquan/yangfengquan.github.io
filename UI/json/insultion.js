function pipeInsultion(rows) {
    let d0 = parseFloat(rows[2].value) / 1000;
    let t0 = parseFloat(rows[3].value);
    let ta = parseFloat(rows[4].value);
    let w = parseFloat(rows[5].value);
    let d1 = d0 + 2 * parseFloat(rows[6].value) / 1000;
    let lambdaArgs = [];
    for (let i = 0; i < 7; i++) {
        lambdaArgs[i] = parseFloat(rows[7 + i].value);
        if (isNaN(lambdaArgs[i])) {
            alert("请检查输入！\n输入完成后，需将鼠标移出输入框，并点击表格任意位置！");
            return false;
        }
    }
    let epsilon = parseFloat(rows[14].value);
    if (isNaN(d0) || isNaN(t0) || isNaN(ta) || isNaN(w) || isNaN(d1) || isNaN(epsilon)) {
        alert("请检查输入！\n输入完成后，需将鼠标移出输入框，并点击表格任意位置！");
        return false;
    }

    let lambda = getLambda(t0, ta, lambdaArgs);
    let ts = getTsS(t0, ta, w, d0, d1, lambda, epsilon);
    let alpha = getAlpha(ts, ta, w, d1, epsilon);
    let Q = getQ(t0, ta, d0, d1, lambda, alpha);
    let q = 3.14 * d1 * Q;

    return [lambda.toFixed(2), alpha.toFixed(2), ts.toFixed(2), Q.toFixed(2), q.toFixed(2)];
}
//计算绝热层传热系数
function getLambda(t0, ta, lambdaArgs) {
    let lambda = lambdaArgs[0] + lambdaArgs[1] * (0.5 * (t0 + ta) - lambdaArgs[2]) + lambdaArgs[3] * Math.pow(0.5 * (t0 + ta) - lambdaArgs[4], 2) + lambdaArgs[5] * Math.pow(0.5 * (t0 + ta) - lambdaArgs[6], 3)

    return lambda
}

//计算散热量 [W/m2]
function getQ(t0, ta, d0, d1, lambda, alpha) {
    return (t0 - ta) / (d1 / (2 * lambda) * Math.log(d1 / d0) + 1 / alpha)
}

//计算外表面温度[C]
function getTs(Q, ta, alpha) {
    return Q / alpha + ta
}

//计算表面传热系数
//epsilon = -1 表示计算，表面传热系数
function getAlpha(ts, ta, w, d1, epsilon) {
    let alpha_r = 5.669 * epsilon / (ts - ta) * (Math.pow((273 + ts) / 100, 4) - Math.pow((273 + ta) / 100, 4))
    let alpha_c
    if (w == 0) {
        alpha_c = 26.4 / Math.pow(297 + 0.5 * (ts + ta), 0.5) * Math.pow((ts - ta) / d1, 0.25)
    }
    else {
        if (w * d1 < 0.8) {
            alpha_c = 0.08 / d1 + 4.2 * Math.pow(w, 0.618) / Math.pow(d1, 0.382)
        }
        else {
            alpha_c = 4.53 * Math.pow(w, 0.805) / Math.pow(d1, 0.195)
        }
    }
    
    return alpha_r + alpha_c
}

//迭代计算外表面温度[C]
function getTsS(t0, ta, w, d0, d1, lambda, epsilon) {
    let ts_pre = ta + 0.1 * (t0 - ta), ts
    do
    {
        ts = ts_pre
        let alpha = getAlpha(ts, ta, w, d1, epsilon)
        let Q = getQ(t0, ta, d0, d1, lambda, alpha)
        ts_pre = getTs(Q, ta, alpha)
    }
    while(Math.abs(ts - ts_pre) > 1e-3)
    
    return ts
}