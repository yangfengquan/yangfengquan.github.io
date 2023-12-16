var response = {}
var fm = ''
var curFormDataObj = {}
const dataPath = 'data.json';

(function () {
    const urlSearchParams = new URLSearchParams(window.location.search)
    fm = urlSearchParams.get('fm')
    const xhr = new XMLHttpRequest()
    xhr.open('GET', dataPath, true)
    xhr.send()
    xhr.onreadystatechange = function() {
        if (xhr.readyState === 4 && xhr.status === 200) {
            response = JSON.parse(xhr.responseText)
            createNavlist(response)
            if (fm != '' && fm != null) {
                createMainEle()
                document.title = response[fm].text + '-' + document.title
                var metaEle = document.getElementsByTagName('meta')
                metaEle['keywords'].setAttribute('content', response[fm].text)
            } else {
                var content = ''

                Object.keys(response).forEach( key => {
                    content += response[key].text + ','
                })

                //content = content.slice(0, -1)
                content += '纯物质物性计算'
                var metaEle = document.getElementsByTagName('meta')
                metaEle['keywords'].setAttribute('content', content)
            }
        }
    }
})()

function createNavlist(data){
    let navEle = document.getElementById('list')
    Object.keys(response).forEach( key => {
        let liEle = document.createElement('li')
        let aEle = document.createElement('a')
        aEle.href = "?fm=" + key
        aEle.innerText = response[key].text
        liEle.appendChild(aEle)
        navEle.appendChild(liEle)
    })
    let liEle1 = document.createElement('li')
    let aEle1 = document.createElement('a')
    aEle1.href = "./props.html"
    aEle1.innerText = '纯物质物性计算'
    liEle1.appendChild(aEle1)
    navEle.appendChild(liEle1)
}

function ontoggledisplay() {
    if (document.getElementById('nav').style.display == 'block') {
        document.getElementById('nav').style.display = 'none'
    } else {
        document.getElementById('nav').style.display = 'block'
    }
}

function createMainEle() {            
    if (fm == '' || fm == null) {
        return
    }

    curFormDataObj = response[fm]

    let tabEle = document.getElementById('tab')
    tabEle.innerHTML = ''
    let titleEle = document.createElement('h2')
    titleEle.innerText = curFormDataObj.text
    tabEle.appendChild(titleEle)

    let formEle = document.createElement('form')
    form(curFormDataObj.form, formEle)
    let warningEle = document.createElement('p')
    warningEle.id = 'warning'
    warningEle.style.color = '#ff0000'
    formEle.appendChild(warningEle)
    tabEle.appendChild(formEle)

    //添加<br>，表单项连续为inline，实现换行
    if (fm == 'waterprops') {
        let brEle = document.createElement('br')
        formEle.insertBefore(brEle, formEle.childNodes[3])
    }

    let calcBtn = document.createElement('button')
    calcBtn.innerText = '运算'
    calcBtn.addEventListener('click', calc)
    tabEle.appendChild(calcBtn)

    let pdfBtn = document.createElement('button')
    pdfBtn.innerText = '计算书'
    pdfBtn.addEventListener('click', pdf)
    tabEle.appendChild(pdfBtn)

    let resTitleEle = document.createElement('h2')
    resTitleEle.innerText = '结果'
    tabEle.appendChild(resTitleEle)

    let resultEle = document.createElement('form')
    form(curFormDataObj.result, resultEle)
    tabEle.appendChild(resultEle)

    //手机端生成表单后，隐藏导航列表
    if (window.screen.width <= 600) {
        document.getElementById('nav').style.display = 'none'
    }
}