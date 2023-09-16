/* ball sort game   |   TP1   |   S5          |
 * Julien Gauthier  |   TP1A  |   2023-2024   |
 */

// Path: ballsort.js

document.addEventListener("DOMContentLoaded", function () {
  let selectedBall = null;
  let moveCount = 0;
  let gameWin = false;
  let gameOver = false;

  // Sélectionner une balle
  document.querySelector("main").addEventListener("click", function (event) {
    const tube = event.target.closest(".tube");
    if (!tube) return;
    if (gameWin) return;

    if (selectedBall) {
      const currentBall = selectedBall;
      if (canPlaceBall(tube, selectedBall)) {
        tube.prepend(currentBall);
        currentBall.classList.remove("selected");
        currentBall.classList.add("bounce");
        setTimeout(() => currentBall.classList.remove("bounce"), 1000); // Retirer l'animation après 1s
        selectedBall = null;
        if (tube !== currentBall.originTube) {
          moveCount++;
          document.querySelector("#movements").innerText = moveCount;
        }
        checkWinCondition();
      }
    } else {
      selectABall(tube);
    }
  });

  function selectABall(tube) {
    if (tube.children.length === 0) return;
    const topBall = tube.firstElementChild;
    selectedBall = topBall;
    selectedBall.originTube = tube;
    topBall.classList.add("selected");
  }

  function canPlaceBall(tube, ball) {

    // si le tube est le tube d'origine de la ball, autoriser le placement
    if (ball.originTube === tube) return true;

    // Vérifiez si le tube est plein
    if (tube.children.length == 4) return false;

    const topBall = tube.firstElementChild;
    if (!topBall) return true; // Si le tube est vide, la balle peut être placée

    if (topBall.style.backgroundColor === ball.style.backgroundColor)
      return true; // Si les couleurs correspondent, la balle peut être placée

    return false; // Sinon, la balle ne peut pas être placée
  }

  function generateRandomLevel(colorCount) {
    const colors = [
      "red",
      "blue",
      "green",
      "yellow",
      "violet",
      "orange",
      "grey",
      "turquoise",
      "lightblue",
      "chartreuse",
    ];
    let balls = [];

    // créez une liste de balles basée sur le nombre de couleurs sélectionné
    for (let i = 0; i < colorCount; ++i) {
      for (let j = 0; j < 4; ++j) {
        balls.push(colors[i]);
      }
    }

    // mélanger les balles
    balls = shuffleArray(balls);

    // supprimer les tubes existants
    const existingTubes = document.querySelectorAll(".tube");
    existingTubes.forEach((tube) => tube.remove());

    // créer les nouveaux tubes
    const tubeContainer = document.querySelector("main");
    for (let i = 0; i < colorCount + 2; i++) {
      const tube = document.createElement("div");
      tube.classList.add("tube");
      tubeContainer.appendChild(tube);
    }

    //distribuer les balles
    const tubes = document.querySelectorAll(".tube");
    let ballIndex = 0;
    tubes.forEach((tube) => {
      for (let i = 0; i < 4 && ballIndex < balls.length; ++i) {
        const ball = document.createElement("div");
        ball.setAttribute(
          "style",
          "background-color: " + balls[ballIndex] + ";"
        );
        //console.log(balls[ballIndex]);
        tube.appendChild(ball);
        ballIndex++;
      }
    });

    // réinitialise le compteur de mouvements
    moveCount = 0;
    document.querySelector("#mouvements").innerText = moveCount;
  }

  function shuffleArray(array) {
    for (let i = array.length - 1; i > 0; i--) {
      const j = Math.floor(Math.random() * (i + 1));
      [array[i], array[j]] = [array[j], array[i]];
    }
    return array;
  }

  function checkWinCondition() {
    if (gameOver) {
      setTimeout(() => {
        alert("Plus de coup disponible... Vous avez perdu =(");
      }, 600);
    }
    let win = true;
    document.querySelectorAll(".tube").forEach((tube) => {
      if (tube.children.length === 4) {
        // Vérifiez si le tube est complètement rempli
        const color = tube.children[0].style.backgroundColor;
        for (let ball of tube.children) {
          if (ball.style.backgroundColor !== color) {
            win = false;
            break;
          }
        }
      } else if (tube.children.length > 0) {
        // Si le tube n'est pas vide et n'est pas complètement rempli, le jeu n'est pas gagné
        win = false;
      }
    });
    if (win) {
      gameWin = true;
      setTimeout(() => {
        alert("Vous avez gagné!");
      }, 600);
    }
  }

  function isMoveAvailable() {
    const tubes = document.querySelectorAll(".tube");

    for (let tube of tubes) {
      if (tube.children.length === 0) {
        // Si un tube est vide, un coup est disponible
        return true;
      }

      const topBallColor = tube.firstElementChild.style.backgroundColor;

      for (let targetTube of tubes) {
        if (tube === targetTube) continue; // Ne vérifiez pas le même tube

        if (targetTube.children.length === 0) {
          // Si le tube cible est vide, un coup est disponible
          return true;
        }

        const targetTopBallColor =
          targetTube.firstElementChild.style.backgroundColor;

        if (topBallColor === targetTopBallColor) {
          // Si les couleurs des balles du dessus correspondent, un coup est disponible
          gameOver = false;
        }
      }
    }

    // Si aucune des conditions ci-dessus n'est remplie, aucun coup n'est disponible
    gameOver = true;
  }

  // Gestion du menu
  document.querySelector("#btnRestart").addEventListener("click", function () {
    if (confirm("Voulez-vous vraiment recommencer?")) {
      location.reload();
    }
  });

  document.querySelector("#cbDarkmode").addEventListener("change", function () {
    document.body.classList.toggle("darkmode");
    localStorage.setItem("darkMode", this.checked);
  });

  document.querySelector("#btnRandom").addEventListener("click", function () {
    if (
      confirm(
        "Voulez-vous vraiment générer un nouveau puzzle? Cela va réinitialiser le score"
      )
    ) {
      const colorCount = parseInt(document.querySelector("#numColors").value);
      generateRandomLevel(colorCount);
    }
  });

  window.onload = function () {
    const darkMode = localStorage.getItem("darkMode");
    if (darkMode && darkMode === "true") {
      document.body.classList.add("darkmode");
      document.querySelector("#cbDarkmode").checked = true;
    }
  };
});
