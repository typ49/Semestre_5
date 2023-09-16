// un tube = class "tube"
// une balle = classe "tube" > div


document.addEventListener("DOMContentLoaded", function() {
    
    // récupère la balle la plus haute du tube cliqué
    // function onClickTube() {
    //     let ball = this.childNodes[1];

    //     ball.classList.add("selected");
    // }

    // un seul tube peut etre selectionné
    function onClickTube () {
        let tube = document.getElementsByClassName("tube")
        if (this.childNodes[1] == undefined) {
            console.log("vide");
        }
        else {
            let ball = this.childNodes[1];
            if (ball.classList.contains("selected")) {
                ball.classList.remove("selected");
                console.log("deselectionné");
                
            } else {
                for (let i=0 ; i < tube.length ; i++) {
                    if (tube[i].childNodes[1].classList.contains("selected")) {
                        tube[i].childNodes[1].classList.remove("selected");
                    }
                }
                ball.classList.add("selected");
                console.log("selectionné");
            }
        }
    }

    // replace la balle selectionné :
    // dans un tube vide
    // ou dans un tube ou il y a de la place et la balle en dessous est de la meme couleur
    // ou dans sont tube de base
    function replaceBall() {
        let selectedBall = document.getElementsByClassName("selected");



    }
    let tube = document.getElementsByClassName("tube");

    for (let i = 0; i < tube.length; i++) {
        tube[i].addEventListener("click", onClickTube);
    }

});