import {page} from "./html_page.js"

var currentUrl = '';
window.addEventListener("load", setCurrentUrl, false);
window.addEventListener("hashchange", setCurrentUrl, false);

function setCurrentUrl() {
    currentUrl = location.hash.slice(1) || '/';
    page(currentUrl);
}

export {currentUrl};