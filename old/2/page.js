"use strict"

function Router() {
    this.routes = {};
    this.currentUrl = '';
}
Router.prototype.route = function (path, callback) {
    this.routes[path] = callback || function () { };
};
Router.prototype.refresh = function () {
    document.getElementById(this.currentUrl)?.classList.remove("show");
    document.getElementsByClassName("nav")[0].innerHTML = '';
    console.log(this.currentUrl)
    this.currentUrl = location.hash.slice(1) || '/';

    let curPage = document.getElementById(this.currentUrl);
    if (curPage != null) {
        curPage.classList.add("show");
        if (this.currentUrl != "/") {
            document.getElementsByClassName("nav")[0].innerHTML = `
                <li id="run">run</li>
                <li id="open">open</li>
                `;
            document.getElementById("run").addEventListener("click", this.routes[this.currentUrl], false);
        }
    }
};
Router.prototype.init = function () {
    window.addEventListener('load', this.refresh.bind(this), false);
    window.addEventListener('hashchange', this.refresh.bind(this), false);
};
window.Router = new Router();
window.Router.init();

window.Router.route("page1", function () {
    alert("page1");
});

window.Router.route("page2", function () {
    alert("page2");
});