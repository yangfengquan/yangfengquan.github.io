function createLinkEle(data){
    let navEle = document.getElementById('list')
    Object.keys(data).forEach( key => {
        let liEle = document.createElement('li')
        let aEle = document.createElement('a')
        aEle.href = "fm.html?fm=" + key
        aEle.innerText = data[key].text
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

function onLinkToggleDisplay() {
    if (document.getElementById('nav').style.display == 'block') {
        document.getElementById('nav').style.display = 'none'
    } else {
        document.getElementById('nav').style.display = 'block'
    }
}