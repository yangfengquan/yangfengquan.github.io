function pumpPower(rows) {
    let flowRate = parseFloat(rows[2].value);
    let h = parseFloat(rows[3].value);
    let rho = parseFloat(rows[4].value);
    let eta = parseFloat(rows[5].value) / 100;

    if (isNaN(flowRate) || isNaN(h) || isNaN(rho) || isNaN(eta)) {
        alert("请检查输入！\n输入完成后，需将鼠标移出输入框，并点击表格任意位置！");
        return false;
    }

    let p = flowRate * h * 9.81 * rho / 3600 / eta / 1000;

    return [p.toFixed(2)];
}