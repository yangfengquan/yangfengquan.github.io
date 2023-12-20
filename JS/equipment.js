/**
 * 
 * @param {*} flowRate 
 * @param {*} h 
 * @param {*} rho 
 * @param {*} eta 
 * @returns 
 */
function pump_power(flowRate, h, rho, eta) {
    eta /= 100;
    return flowRate * h * 9.81 * rho / 3600 / eta / 1000;
}