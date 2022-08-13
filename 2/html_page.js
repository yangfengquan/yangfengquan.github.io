import html_pages_data from "./html_pages_data.js"
import { solver } from "./solver.js";
import { currentUrl } from "./Url.js";

function page(url) {
    let page_ele = document.getElementById("page");
    page_ele.innerHTML = '';
    if (Object.hasOwnProperty.call(html_pages_data, url)) {  
        if (screen.width > 800) {
            page_ele.appendChild(createPageForBigScreen(html_pages_data[url]));
        } else {
            page_ele.appendChild(createPageForSmallScreen(html_pages_data[url]));
        }
    }
    else {
        page_ele.innerHTML = '<div style="width: 100%; text-align:center;"><h2>404</h><p>未找到页面</p></div>';
    }
}

function menu() {
    let menu_ele = document.getElementById("menu-ul");
    const menu0 = [
        {text: "管道", pagesBegin: 0},
        {text: "物性", pagesBegin: 7}
    ];
    let curId = 0;
    let pLi_ele;
    let ul_ele;
    for (const key in html_pages_data) {
        if (Object.hasOwnProperty.call(html_pages_data, key)) {
            
            menu0.forEach(item => {
                if (item.pagesBegin == curId) {
                    pLi_ele = document.createElement("li");
                    ul_ele = document.createElement("ul");
                    ul_ele.innerText = item.text;
                    pLi_ele.appendChild(ul_ele);
                    menu_ele.appendChild(pLi_ele);
                }
            });

            let li_ele = document.createElement("li");
            let a_ele = document.createElement("a");
            a_ele.href = "#" + key;
            a_ele.innerText = html_pages_data[key].title;
            li_ele.appendChild(a_ele);
            ul_ele.appendChild(li_ele);
        }
        curId++;
    }
}

function createPageForBigScreen(html_page_data) {
    let page_ele = document.createElement("div");

    let page_title = document.createElement("h3");
    page_title.innerText = html_page_data.title;
    page_ele.appendChild(page_title);

    page_ele.appendChild(createFormTable(html_page_data));


    let p_ele = document.createElement("p");
    let button_ele = document.createElement("button");
    button_ele.innerText = "添加";
    button_ele.addEventListener("click", addRow);
    p_ele.appendChild(button_ele);
    page_ele.appendChild(p_ele);

    return page_ele;
}

function createPageForSmallScreen(html_page_data) {
    let page_ele = document.createElement("div");

    let page_title = document.createElement("h3");
    page_title.innerText = html_page_data.title;
    page_ele.appendChild(page_title);

    page_ele.appendChild(createFormDiv(html_page_data));


    let p_ele = document.createElement("p");
    let button_ele = document.createElement("button");
    button_ele.innerText = "计算";
    button_ele.addEventListener("click", yrun);
    p_ele.appendChild(button_ele);
    page_ele.appendChild(p_ele);

    return page_ele;
}

function createFormTable(html_page_data) {
    let table_ele = document.createElement("table");
    let thead_ele = document.createElement("thead");
    let tbody_ele = document.createElement("tbody");
    let thead_tr_ele = document.createElement("tr");
    let tbody_tr_ele = document.createElement("tr");
    thead_ele.appendChild(thead_tr_ele);
    tbody_ele.appendChild(tbody_tr_ele);
    table_ele.appendChild(thead_ele);
    table_ele.appendChild(tbody_ele);
    for (const key in html_page_data.inputs) {
        if (Object.hasOwnProperty.call(html_page_data.inputs, key)) {
            const element = html_page_data.inputs[key];
            let th_ele = document.createElement("th");
            let td_ele = document.createElement("td");
            th_ele.innerText = element.title + "\n" + (element.unit || '');
            let input_ele = document.createElement(element.tagname);
            input_ele.name = key;
            input_ele.addEventListener("change", function(){
                solver.methods[currentUrl]();
            });
            if (element.tagname == "select") {
                html_page_data.options[key].forEach(option => {
                    let option_ele =document.createElement("option");
                    option_ele.value = option.value;
                    option_ele.innerText = option.text;
                    input_ele.appendChild(option_ele);
                });
            }
            else if (element.tagname == "input") {
                input_ele.type = element.type;
                input_ele.value = element.value;
            }

            
            thead_tr_ele.appendChild(th_ele);
            td_ele.appendChild(input_ele);
            tbody_tr_ele.appendChild(td_ele);
        }
    }

    for (const key in html_page_data.results) {
        if (Object.hasOwnProperty.call(html_page_data.results, key)) {
            const element = html_page_data.results[key];
            let th_ele = document.createElement("th");
            let td_ele = document.createElement("td");
            th_ele.innerHTML = element.title + "<br>" + element.unit;
            
            thead_tr_ele.appendChild(th_ele);
            tbody_tr_ele.appendChild(td_ele);
        }
    }

    //删除按钮
    if (html_page_data.multiple_rows) {
        thead_tr_ele.appendChild(document.createElement("th"));
        let td_ele = document.createElement("td");
        let button_ele = document.createElement("button");
        button_ele.innerText = "删除";
        button_ele.addEventListener("click", delRow);

        td_ele.appendChild(button_ele);
        tbody_tr_ele.appendChild(td_ele);
    }

    if (html_page_data.has_total_row) {
        let tfoot_ele = document.createElement("tfoot");
        let tr_ele = document.createElement("tr");
        tfoot_ele.appendChild(tr_ele);
        table_ele.appendChild(tfoot_ele);
        for (let i = 0; i < html_page_data.inputs.length + html_page_data.results.length; i++) {
            tr_ele.appendChild(document.createElement("td"));
        }
        if (html_page_data.multiple_rows) {
            tr_ele.appendChild(document.createElement("td"));
        }

        tr_ele.cells[0].innerText = "合计";
    }
    
    return table_ele;
}

function createFormDiv(html_page_data) {
    let form_div_ele = document.createElement("div");
    for (const key in html_page_data.inputs) {
        if (Object.hasOwnProperty.call(html_page_data.inputs, key)) {
            const element = html_page_data.inputs[key];
            let p_ele = document.createElement("p");
            let label_ele = document.createElement("label");
            label_ele.innerText = element.title + "\n" + (element.unit || '');
            let input_ele = document.createElement(element.tagname);
            input_ele.name = key;
            if (element.tagname == "select") {
                html_page_data.options[key].forEach(option => {
                    let option_ele =document.createElement("option");
                    option_ele.value = option.value;
                    option_ele.innerText = option.text;
                    input_ele.appendChild(option_ele);
                });
            }
            else if (element.tagname == "input") {
                input_ele.type = element.type;
                input_ele.value = element.value;
            }

            p_ele.appendChild(label_ele);
            p_ele.appendChild(input_ele);
            form_div_ele.appendChild(p_ele);
        }
    }

    return form_div_ele;
}

function addRow(event) {
        
}

function delRow(event) {
    
}
menu();
export {page, addRow, delRow};