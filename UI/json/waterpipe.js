function waterpipe(rows) {
    let flowRate = parseFloat(rows[2].value) / 3600;
    let tA = parseFloat(rows[3].value);
    let pA = parseFloat(rows[4].value);
    let d0 = parseFloat(rows[5].value) / 1000;
    let thk = parseFloat(rows[6].value);
    let li = parseFloat(rows[7].value);
    let lp = parseFloat(rows[8].value);
    let rough = parseFloat(rows[9].value);
    let d1 = d0 + 2 * parseFloat(rows[10].value) / 1000;
    let lambdaArgs = [];
    for (let i = 0; i < 7; i++) {
        lambdaArgs[i] = parseFloat(rows[10 + i].value);
        if (isNaN(lambdaArgs[i])) {
            alert("请检查输入！\n输入完成后，需将鼠标移出输入框，并点击表格任意位置！");
            return false;
        }
    }
    let epsilon = parseFloat(rows[18].value);
    let ta = parseFloat(rows[19].value);
    let w = parseFloat(rows[20].value);

    if (isNaN(flowRate) || isNaN(tA) || isNaN(pA) || isNaN(d0) || isNaN(tA) || isNaN(ta) || isNaN(w) || isNaN(d1) || isNaN(epsilon)) {
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