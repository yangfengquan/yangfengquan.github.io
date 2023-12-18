"use strict"
function pipeDiameter(fmData) {
    let flowRate = unitConverter(fmData.form[2].other + "tom3/s", fmData.form[2].value);
    let v = unitConverter(fmData.form[3].other + "tom/s", fmData.form[3].value);
    fmData.result[0].value = unitConverter('mto' + fmData.result[0].other, pipe_diameter(flowRate, v)).toFixed(2);
}
function pipeVelocity(fmData) {
    let flowRate = unitConverter(fmData.form[2].other + "tom3/s", fmData.form[2].value);
    let di = unitConverter(fmData.form[3].other + "tom", fmData.form[3].value);
    fmData.result[0].value = unitConverter('m/s' + fmData.result[0].other, pipe_velocity(flowRate, di)).toFixed(2);
}
function pipeWeight(fmData) {
    let d = unitConverter(fmData.form[2].other + "tom", fmData.form[2].value);
    let delta = unitConverter(fmData.form[3].other + "tom", fmData.form[3].value);
    let len = unitConverter(fmData.form[4].other + "tom", fmData.form[4].value);
    let rho = unitConverter(fmData.form[5].other + "tom", fmData.form[5].value);
    let w = pipe_weight(d, delta, len, rho);
    fmData.result[0].value = unitConverter('kg/m' + fmData.result[0].other, w[0]).toFixed(2);
    fmData.result[1].value = unitConverter('kg' + fmData.result[1].other, w[1]).toFixed(2);
}
function pipeStrength(fmData) {
    let d = unitConverter(fmData.form[2].other + "tomm", fmData.form[2].value);
    let p = unitConverter(fmData.form[3].other + "toMPa", fmData.form[3].value);
    let s = unitConverter(fmData.form[4].other + "toMPa", fmData.form[4].value);
    let y = fmData.form[5].value;
    let w = fmData.form[6].value;
    let phi = fmData.form[7].value;
    let c1 = fmData.form[8].value;
    let c2 = unitConverter(fmData.form[9].other + "tomm", fmData.form[9].value);
    let c3 = unitConverter(fmData.form[10].other + "tomm", fmData.form[10].value);
    let r = unitConverter(fmData.form[11].other + "tomm", fmData.form[11].value);

    let l = pipe_strength(d, p, s, y, w, phi, c1, c2, c3);
    let b = bend_strength(d, p, s, y, w, phi, c1, c2, c3, r);

    for (let i = 0; i < l.length; i++) {
        fmData.result[i].value = unitConverter('mm' + fmData.result[i].other, l[i]).toFixed(2);
    }
    for (let i = 0; i < b.length; i++) {
        fmData.result[i + l.length].value = unitConverter('mm' + fmData.result[ + l.length].other, b[i]).toFixed(2);
    }
}