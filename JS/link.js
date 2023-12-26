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
}

function onLinkToggleDisplay() {
    if (document.getElementById('nav').style.display == 'block') {
        document.getElementById('nav').style.display = 'none'
    } else {
        document.getElementById('nav').style.display = 'block'
    }
}