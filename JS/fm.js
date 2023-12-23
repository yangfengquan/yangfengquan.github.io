"use strict"
import unitConverter from "./unitConverter.js"
import { pipe_diameter, pipe_velocity, pipe_weight, pipe_size, pipe_strength, bend_strength, pipe_insultion, water_pipe } from "./pipe2.js"
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
    getSiValue(key){
        return unitConverter(this.getValue(key), this.getUnit(key));
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
    setValueWithSi(key, value){
        this.setValue(key, unitConverter(value, "SI", this.getUnit(key)));
    }
    setValuesWithSi(valuesObj){
        for (const key in valuesObj) {
            if (Object.hasOwnProperty.call(valuesObj, key)) {
                this.setValueWithSi(key, valuesObj[key]);
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
    fillFormValue(){
        for (let i = 0; i < this._form.length; i++) {
            if (this._form[i].required && !document.getElementById(this._form[i].id).value) {
                return this._form[i].name;
            } else {
                this._form[i].value = document.getElementById(this._form[i].id).value;
                if (this._form[i].hasOwnProperty("unit")) {
                    this._form[i].unit = document.getElementById(this._form[i].id + "-unit").value;
                }
            }
        }
        return 0;
    }
    render(){
        this._result.forEach(item => {
            document.getElementById(item.id).value = item.value;
        });
    }
    pipeDiameter() {
        let flowRate = this.getSiValue("flowRate");
        let v = this.getSiValue("v");
        this.setValueWithSi("di", pipe_diameter(flowRate, v));
    }
    pipeVelocity() {
        let flowRate = this.getSiValue("flowRate");
        let di = this.getSiValue("di");
        this.setValueWithSi("v", pipe_velocity(flowRate, di));
    }
    pipeWeight() {
        let d = this.getSiValue("d");
        let delta = this.getSiValue("delta");
        let len = this.getSiValue("len");
        let rho = this.getSiValue("rho");
        let w = pipe_weight(d, delta, rho);
        this.setValuesWithSi(w);    
    }
    pipeSize() {
        let dn = this.getValue("dn");
        let sch = this.getValue("sch");
        let p = pipe_size(dn, sch);
        this.setValuesWithSi(p);
    }
    pipeStrength() {
        let d = this.getSiValue("d");
        let p = this.getSiValue("p");
        let s = this.getSiValue("s");
        let y = this.getValue("y");
        let w = this.getValue("w");
        let phi = this.getValue("phi");
        let c1 = this.getValue("c1") / 100;
        let c2 = this.getSiValue("c2");
        let c3 = this.getSiValue("c3");
        let r = this.getSiValue("r");
    
        let l = pipe_strength(d, p, s, y, w, phi, c1, c2, c3);
        let b = bend_strength(d, p, s, y, w, phi, c1, c2, c3, r);
    
        this.setValuesWithSi(l);
        this.setValuesWithSi(b);
    }
    pipeHInsultion() {
        let d0 = this.getSiValue("d0");
        let t0 = this.getSiValue("t0");
        let ta = this.getSiValue("ta");
        let w = this.getSiValue("w");
        let delta = this.getSiValue("delta");
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

        this.setValuesWithSi(insul);
    }
    pipeCInsultion = this.pipeHInsultion;
    waterPipe() {
        let flowRate = this.getSiValue("flowRate");
        let a_t = this.getSiValue("a_t");
        let a_p = this.getSiValue("a_p");
        let d0 = this.getSiValue("d0");
        let delta = this.getSiValue("delta");
        let ll = this.getSiValue("ll");
        let lf = this.getSiValue("lf");
        let rough = this.getSiValue("rough");
        let insuldelta = this.getSiValue("insuldelta");
        
        let lambdaArgs = [];
        lambdaArgs.push(this.getValue("a"));
        lambdaArgs.push(this.getValue("b"));
        lambdaArgs.push(this.getValue("c"));
        lambdaArgs.push(this.getValue("d"));
        lambdaArgs.push(this.getValue("e"));
        lambdaArgs.push(this.getValue("f"));
        lambdaArgs.push(this.getValue("g"));
        
        let epsilon = this.getValue("epsilon");

        let ta = this.getSiValue("ta");
        let w = this.getSiValue("w");
        
        let di = d0 - 2 * delta;
        let d1 = d0 + 2 * insuldelta;
        let wp = water_pipe(flowRate, a_t, a_p, di, d0, d1, ll, lf, rough, lambdaArgs, epsilon, ta, w);

        this.setValuesWithSi(wp);
    }
    pumpPower() {
        let flowRate = this.getSiValue("flowRate");
        let h = this.getSiValue("h");
        let eta = this.getSiValue("eta") / 100;
        this.setValueWithSi("p", pump_power(flowRate, h, eta));
    }
    waterProps() {
        let name1 = this.getValue("waterArg1");
        let value1 = this.getSiValue("waterValue1");
        let name2 = this.getValue("waterArg2");
        let value2 = this.getSiValue("waterValue2");
        let mode = name1 + name2;
        console.log(name1);
        switch (mode) {
            case "pT":
                this.setValuesWithSi(pT(value1, value2));
                break;
            case "ph":
                this.setValuesWithSi(ph(value1, value2));
                break;
            case "ps":
                this.setValuesWithSi(ps(value1, value2));
                break;
            case "prho":
                this.setValuesWithSi(prho(value1, value2));
                break;
            case "px":
                this.setValuesWithSi(px(value1, value2));
                break;
            case "Tx":
                this.setValuesWithSi(Tx(value1, value2));
                break;
            case "hs":
                this.setValuesWithSi(hs(value1, value2));
                break;
            case "hrho":
                this.setValuesWithSi(hrho(value1, value2));
                break;
            default:
                break;
        }
        console.log(this);
    } 
}
