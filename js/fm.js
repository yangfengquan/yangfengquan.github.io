"use strict"
import unitConverter from "./unitConverter.js"
import { pipe_diameter, pipe_velocity, pipe_weight, pipe_size, pipe_strength, bend_strength, resistace, reynolds, pipe_lDp, pipe_fDpl, pipe_insultion, water_pipe } from "./pipe.js"
import { pump_power } from "./equipment.js";
import { pT, ph, ps, prho, px, Tx, hs, hrho } from "./xsteam.js"

export default class Fm {
    constructor(form, result){
        this._form = [...form];
        this._result = [...result];
    }
    
    getValue(key){
        let d = this._form.concat(this._result);
        for (let i = 0; i < d.length; i++) {
            if (d[i].id == key) {
                if (d[i].type == "number") {
                    return parseFloat(d[i].value);
                } else {
                    return d[i].value;
                }
            }
        }
        return false;
    }
    getBaseUnitValue(key){
        return unitConverter(this.getValue(key), this.getUnit(key), "baseUnit");
    }
    getUnit(key){
        let d = this._form.concat(this._result);
        for (let i = 0; i < d.length; i++) {
            if (d[i].id == key) {
                if (d[i].unit != "") {
                    return d[i].unit;
                } else {
                    return '';
                }
            }
        }
        return '';
    }
    setValue(key, value){
        for (let i = 0; i < this._form.length; i++) {
            if (this._form[i].id === key) {
                this._form[i].value = value;
                return true;
            }
        }

        for (let i = 0; i < this._result.length; i++) {
            if (this._result[i].id === key) {
                this._result[i].value = value;
                return true;
            }
        }
        return false;
    }
    setValueWithBaseUnit(key, value){
        this.setValue(key, unitConverter(value, "baseUnit", this.getUnit(key)));
    }
    setValuesWithBaseUnit(valuesObj){
        for (const key in valuesObj) {
            if (Object.hasOwnProperty.call(valuesObj, key)) {
                this.setValueWithBaseUnit(key, valuesObj[key]);
            }
        }
    }
    setUnit(key, unit){
        for (let i = 0; i < this._form.length; i++) {
            if (this._form[i].id === key) {
                this._form[i].unit = unit;
                return true;
            }
        }

        for (let i = 0; i < this._result.length; i++) {
            if (this._result[i].id === key) {
                this._result[i].unit = unit;
                return true;
            }
        }
        return false;
    }
    
    pipeDiameter() {
        let flowRate = this.getBaseUnitValue("flowRate");
        let v = this.getBaseUnitValue("v");
        this.setValueWithBaseUnit("di", pipe_diameter(flowRate, v));
    }
    pipeVelocity() {
        let flowRate = this.getBaseUnitValue("flowRate");
        let di = this.getBaseUnitValue("di");
        this.setValueWithBaseUnit("v", pipe_velocity(flowRate, di));
    }
    pipeWeight() {
        let d = this.getBaseUnitValue("d");
        let delta = this.getBaseUnitValue("delta");
        let len = this.getBaseUnitValue("len");
        let rho = this.getBaseUnitValue("rho");
        let w = pipe_weight(d, delta, rho);console.log(w);
        this.setValueWithBaseUnit("pw", w);
        this.setValueWithBaseUnit("tw", w * len);   
    }
    pipeSize() {
        let dn = this.getValue("dn");
        let sch = this.getValue("sch");
        let p = pipe_size(dn, sch);
        this.setValuesWithBaseUnit(p);
    }
    pipeStrength() {
        let d = this.getBaseUnitValue("d");
        let p = this.getBaseUnitValue("p");
        let s = this.getBaseUnitValue("s");
        let y = this.getValue("y");
        let w = this.getValue("w");
        let phi = this.getValue("phi");
        let c1 = this.getValue("c1") / 100;
        let c2 = this.getBaseUnitValue("c2");
        let c3 = this.getBaseUnitValue("c3");
        let r = this.getBaseUnitValue("r");
    
        let l = pipe_strength(d, p, s, y, w, phi, c1, c2, c3);
        let b = bend_strength(d, p, s, y, w, phi, c1, c2, c3, r);
    
        this.setValuesWithBaseUnit(l);
        this.setValuesWithBaseUnit(b);
    }
    pipeResistance() {
        let fluidName = this.getValue("fluidName");
        let flowRate = this.getBaseUnitValue("flowRate");
        let a_p = this.getBaseUnitValue("a_p");
        let t = this.getBaseUnitValue("t");
        let d0 = this.getBaseUnitValue("d0");
        let delta = this.getBaseUnitValue("delta");
        let llen = this.getBaseUnitValue("llen");
        let flen = this.getBaseUnitValue("flen");
        let rough = this.getBaseUnitValue("rough");
        
        let di = d0 - 2 * delta;

        let rho = Module.PropsSI("D", "P", a_p, "T", t, fluidName);
        let mu = Module.PropsSI("VISCOSITY", "P", a_p, "T", t, fluidName);

        let ve = pipe_velocity(flowRate, di);
        let ldp = pipe_lDp(flowRate, rough, di, llen, rho, mu);
        let fdp = pipe_fDpl(flowRate, rough, di, flen, rho, mu);
        let dp = ldp + fdp;
        let b_p = a_p - dp;
        let re = reynolds(di, ve, rho, mu);
        let lambda = resistace(re, di, rough);
        this.setValueWithBaseUnit("dp", dp);
        this.setValueWithBaseUnit("ldp", ldp);
        this.setValueWithBaseUnit("fdp", fdp);
        this.setValueWithBaseUnit("b_p", b_p);
        this.setValueWithBaseUnit("ve", ve);
        this.setValueWithBaseUnit("re", re);
        this.setValueWithBaseUnit("lambda", lambda);

    }
    pipeHInsultion() {
        let d0 = this.getBaseUnitValue("d0");
        let t0 = this.getBaseUnitValue("t0");
        let ta = this.getBaseUnitValue("ta");
        let w = this.getBaseUnitValue("w");
        let delta = this.getBaseUnitValue("delta");
        let d1 = d0 + 2 * delta;
        let epsilon = this.getValue("epsilon");
        
        let lambdaArgs = [];
        lambdaArgs.push(this.getValue("a"));
        lambdaArgs.push(this.getValue("b"));
        lambdaArgs.push(this.getValue("c"));
        lambdaArgs.push(this.getValue("d"));
        lambdaArgs.push(this.getValue("e"));
        lambdaArgs.push(this.getValue("f"));
        lambdaArgs.push(this.getValue("g"));

        let insul = pipe_insultion(t0, ta, w, d0, d1, lambdaArgs, epsilon);

        this.setValuesWithBaseUnit(insul);
    }
    pipeCInsultion = this.pipeHInsultion;
    waterPipe() {
        let flowRate = this.getBaseUnitValue("flowRate");
        let a_t = this.getBaseUnitValue("a_t");
        let a_p = this.getBaseUnitValue("a_p");
        let d0 = this.getBaseUnitValue("d0");
        let delta = this.getBaseUnitValue("delta");
        let ll = this.getBaseUnitValue("llen");
        let le = this.getBaseUnitValue("flen");
        let rough = this.getBaseUnitValue("rough");
        let insuldelta = this.getBaseUnitValue("insuldelta");
        
        let lambdaArgs = [];
        lambdaArgs.push(this.getValue("a"));
        lambdaArgs.push(this.getValue("b"));
        lambdaArgs.push(this.getValue("c"));
        lambdaArgs.push(this.getValue("d"));
        lambdaArgs.push(this.getValue("e"));
        lambdaArgs.push(this.getValue("f"));
        lambdaArgs.push(this.getValue("g"));
        
        let epsilon = this.getValue("epsilon");

        let ta = this.getBaseUnitValue("ta");
        let w = this.getBaseUnitValue("w");
        
        let di = d0 - 2 * delta;
        let d1 = d0 + 2 * insuldelta;
        let wp = water_pipe(flowRate, a_t, a_p, di, d0, d1, ll, le, rough, lambdaArgs, epsilon, ta, w);

        this.setValuesWithBaseUnit(wp);
    }
    pumpPower() {
        let flowRate = this.getBaseUnitValue("flowRate");
        let h = this.getBaseUnitValue("h");
        let eta = this.getBaseUnitValue("eta") / 100;
        this.setValueWithBaseUnit("p", pump_power(flowRate, h, eta));
    }
    waterProps() {
        let name1 = this.getValue("propArg1");
        let value1 = this.getBaseUnitValue("propValue1");
        let name2 = this.getValue("propArg2");
        let value2 = this.getBaseUnitValue("propValue2");
        let mode = name1 + name2;console.log(mode, value1,value2);
        console.log(px(value1, value2));
        switch (mode) {
            case "pT":
                this.setValuesWithBaseUnit(pT(value1, value2));
                break;
            case "ph":
                this.setValuesWithBaseUnit(ph(value1, value2));
                break;
            case "ps":
                this.setValuesWithBaseUnit(ps(value1, value2));
                break;
            case "prho":
                this.setValuesWithBaseUnit(prho(value1, value2));
                break;
            case "px":
                this.setValuesWithBaseUnit(px(value1, value2));
                break;
            case "Tx":
                this.setValuesWithBaseUnit(Tx(value1, value2));
                break;
            case "hs":
                this.setValuesWithBaseUnit(hs(value1, value2));
                break;
            case "hrho":
                this.setValuesWithBaseUnit(hrho(value1, value2));
                break;
            default:
                break;
        }
    }
    pureProps() {
        let fluidName = this.getValue("fluidName");
        let arg1 = this.getValue("propArg1");
        let v1 = this.getBaseUnitValue("propValue1");
        let arg2 = this.getValue("propArg2");
        let v2 = this.getBaseUnitValue("propValue2");
        v1 = arg1 === "P" ? unitConverter(v1, "base", "PaA") : v1;
        v2 = arg2 === "P" ? unitConverter(v2, "base", "PaA") + 101325 : v2;
        this._result.forEach(item => {
            if (item.id === "P") {
                this.setValueWithBaseUnit(item.id, unitConverter(Module.PropsSI(item.id, arg1, v1, arg2, v2, fluidName), "PaA", "base"))
            } else {
                this.setValueWithBaseUnit(item.id, Module.PropsSI(item.id, arg1, v1, arg2, v2, fluidName));
            }    
        })       
    }
}
