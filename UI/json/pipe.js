function pipeDiameter(rows) {
    let flowRate = parseFloat(rows[2].value) / 3600;
    let v = parseFloat(rows[3].value);

    if (isNaN(flowRate) || isNaN(v)) {
        alert("请检查输入！\n输入完成后，需将鼠标移出输入框，并点击表格任意位置！");
        return false;
    }

    let d = (Math.sqrt(flowRate / v / 3.14) * 2 * 1000).toFixed(2);

    return [d];
}

function pipeVelocity(rows) {
    let flowRate = parseFloat(rows[2].value) / 3600;
    let d = parseFloat(rows[3].value) / 1000;

    if (isNaN(flowRate) || isNaN(d)) {
        alert("请检查输入！\n输入完成后，需将鼠标移出输入框，并点击表格任意位置！");
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
        alert("请检查输入！\n输入完成后，需将鼠标移出输入框，并点击表格任意位置！");
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
    let dn = rows[0].dn;
    let sch = rows[0].sch;

    if (dn == '' || sch == '') {
        alert("请检查输入！\n输入完成后，需将鼠标移出输入框，并点击表格任意位置！");
        return false;
    }

    const Schs = ["SCH5", "SCH10", "SCH20", "SCH30", "SCH40", "SCH60", "SCH80", "SCH100", "SCH120", "SCH140", "SCH160", "STD", "XS", "XXS", "SCH5S", "SCH10S", "SCH40S", "SCH80S"]
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
    
    let i = Schs.indexOf(sch);
    let d = D0[dn];
    let thk = Thk[dn][i];
    thk = thk == undefined ? dn + "钢管无" + sch : thk;
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
        alert("请检查输入！\n输入完成后，需将鼠标移出输入框，并点击表格任意位置！");
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