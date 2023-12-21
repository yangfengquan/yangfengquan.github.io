"use strict"
import unitConverter from "./unitConverter.js"
//import * as unit from "./unitConverter.js"
import {pipe_diameter, pipe_velocity, pipe_weight, pipe_size, pipe_strength, bend_strength, pipe_insultion, water_pipe} from "./pipe2.js"
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
                //document.getElementById("warning").innerText = this._form[i].name + "必须填写";
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
        let flowRate = unitConverter(this.getValue("flowRate"), this.getUnit("flowRate"), "SI");
        let v = unitConverter(this.getValue("v"), this.getUnit("v"), "SI");
        this.setValue("di", unitConverter(pipe_diameter(flowRate, v), 'SI', this.getUnit("di"),));    
    }
    pipeVelocity() {
        let flowRate = unitConverter(this.getValue("flowRate"), this.getUnit("flowRate"), "SI");
        let di = unitConverter(this.getValue("di"), this.getUnit("di"), "SI");
        this.setValue("v", unitConverter(pipe_velocity(flowRate, di), 'm/s', this.getUnit("v")));
    }
    pipeWeight() {
        let d = unitConverter(this.getValue("d"), this.getUnit("d"), "SI", );
        let delta = unitConverter(this.getValue("delta"), this.getUnit("delta"), "SI");
        let len = unitConverter(this.getValue("len"), this.getUnit("len"), "SI");
        let rho = unitConverter(this.getValue("rho"), this.getUnit("rho"), "kg/m3");
        let w = pipe_weight(d, delta, rho);
        this.setValue("pw", unitConverter(w, "kg/m", this.getUnit("pw")));
        this.setValue("tw", unitConverter(w * len, "kg", this.getUnit("tw")));        
    }
    pipeSize() {
        let dn = this.getValue("dn");
        let sch = this.getValue("sch");
        let p = pipe_size(dn, sch);
        this.setValue("d", unitConverter(p.d, "SI", this.getUnit("d")));
        this.setValue("delta", unitConverter(p.delta, "SI", this.getUnit("delta")));
        this.setValue("SI", unitConverter(p.m, "kg/m", this.getUnit("SI")));
    }
    pipeStrength() {
        let d = unitConverter(this.getValue("d"), this.getUnit("d"), "SI");
        let p = unitConverter(this.getValue("p"), this.getUnit("p"), "SI");
        let s = unitConverter(this.getValue("s"), this.getUnit("s"), "SI");
        let y = this.getValue("y");
        let w = this.getValue("w");
        let phi = this.getValue("phi");
        let c1 = this.getValue("c1");
        let c2 = unitConverter(this.getValue("c2"), this.getUnit("c2"), "SI");
        let c3 = unitConverter(this.getValue("c3"), this.getUnit("c3"), "SI");
        let r = unitConverter(this.getValue("r"), this.getUnit("r"), "SI");
    
        let l = pipe_strength(d, p, s, y, w, phi, c1, c2, c3);
        let b = bend_strength(d, p, s, y, w, phi, c1, c2, c3, r);
    
        this.setValue("t", unitConverter(l.t, "SI", this.getUnit("t")));
        this.setValue("nt", unitConverter(l.nt, "SI", this.getUnit("nt")));
        this.setValue("t1", unitConverter(b.t1, "SI", this.getUnit("t1")));
        this.setValue("nt1", unitConverter(b.nt1, "SI", this.getUnit("nt1")));
        this.setValue("t2", unitConverter(b.t2, "SI", this.getUnit("t2")));
        this.setValue("nt2", unitConverter(b.nt2, "SI", this.getUnit("nt2")));
        this.setValue("t3", unitConverter(b.t3, "SI", this.getUnit("t3"), ));
        this.setValue("nt3", unitConverter(b.nt3, "SI", this.getUnit("nt3")));
    }
    pipeInsultion() {
        let d0 = unitConverter(this.getUnit(this.getValue("d0"), "d0"), "SI");
        let t0 = unitConverter(this.getValue("t0"), this.getUnit("t0"), "SI");
        let ta = unitConverter(this.getValue("ta"), this.getUnit("ta"), "SI");
        let w = unitConverter(this.getValue("w"), this.getUnit("w"), "SI");
        let delta = unitConverter(this.getValue("delta"), this.getUnit("delta"), "SI");
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
        
        this.setValue("lambda", unitConverter(insul.lambda, 'W/(m*K)', this.getUnit("lambda")));
        this.setValue("alpha", unitConverter(insul.alpha, 'W/(m2*K)', this.getUnit("alpha")));
        this.setValue("ts", unitConverter(insul.ts, "SI", this.getUnit("ts")));
        this.setValue("Q", unitConverter(insul.Q, 'W/m2', this.getUnit("Q")));
        this.setValue("q", unitConverter(insul.q, 'W/m', this.getUnit("q")));
    }
    waterPipe() {
        let flowRate = unitConverter(this.getValue("flowRate"), this.getUnit("flowRate"), "SI");
        let a_t = unitConverter(this.getValue("a_t"), this.getUnit("a_t"), "SI");
        let a_p = unitConverter(this.getValue("a_p"), this.getUnit("a_p"), "SI");
        let d0 = unitConverter(this.getValue("d0"), this.getUnit("d0"), "SI");
        let delta = unitConverter(this.getValue("delta"), this.getUnit("delta"), "SI");
        let ll = unitConverter(this.getValue("ll"), this.getUnit("ll"), "SI");
        let lf = unitConverter(this.getValue("lf"), this.getUnit("lf"), "SI");
        let rough = unitConverter(this.getValue("rough"), this.getUnit("rough"), "SI");
        let insuldelta = unitConverter(this.getValue("insuldelta"), this.getUnit("insuldelta"), "SI");
        
        let lambdaArgs = [];
        lambdaArgs.push(this.getValue("a"));
        lambdaArgs.push(this.getValue("b"));
        lambdaArgs.push(this.getValue("c"));
        lambdaArgs.push(this.getValue("d"));
        lambdaArgs.push(this.getValue("e"));
        lambdaArgs.push(this.getValue("f"));
        lambdaArgs.push(this.getValue("g"));
        
        let epsilon = this.getValue("epsilon");

        let ta = unitConverter(this.getValue("ta"), this.getUnit("ta"), "SI");
        let w = unitConverter(this.getValue("w"), this.getUnit("w"), "SI");
        
        let di = d0 - 2 * delta;
        let d1 = d0 + 2 * insuldelta;
        let wp = water_pipe(flowRate, a_t, a_p, di, d0, d1, ll, lf, rough, lambdaArgs, epsilon, ta, w);
        
        for (const key in wp) {
            if (Object.hasOwnProperty.call(wp, key)) {
                this.setValue(unitConverter(wp[key], "SI", this.getUnit(key)));
            }
        }
    }
    pumpPower() {

    }
}
