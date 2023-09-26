/** ball sort game   |   TP1_WA  |   S5          |
 * Julien Gauthier  |   TP1A    |   2023-2024   |
 */

// Path: ballsort.js

document.addEventListener("DOMContentLoaded", function () {
  // variables globales
  let selectedBall = null;
  let moveCount = 0;
  let gameWin = false;
  let initialBallOrder = [
    "red",
    "blue",
    "red",
    "green",
    "red",
    "blue",
    "green",
    "green",
    "blue",
    "blue",
    "red",
    "green",
  ];
  let currentColorCount = 3;

  // Gestionnaire d'interaction pour les tubes
  function handleTubeInteraction(event) {
    event.preventDefault(); // Pour éviter des comportements par défaut sur certains appareils mobiles

    const tube = event.target.closest(".tube");
    if (!tube || gameWin) return;

    if (selectedBall) {
      const currentBall = selectedBall;
      if (canPlaceBall(tube, selectedBall)) {
        tube.prepend(currentBall);
        currentBall.classList.remove("selected");
        if (tube !== currentBall.originTube) {
          moveCount++;
          document.querySelector("#movements").innerText = moveCount;
        }
        selectedBall = null;
        checkWinCondition();
      }
    } else {
      selectABall(tube);
    }
  }

  // Ajout des gestionnaires d'événements pour les interactions
  document
    .querySelector("main")
    .addEventListener("touchstart", handleTubeInteraction, false);
  document
    .querySelector("main")
    .addEventListener("click", handleTubeInteraction, false);

  function selectABall(tube) {
    if (tube.children.length === 0) return;
    const topBall = tube.firstElementChild;
    selectedBall = topBall;
    selectedBall.originTube = tube;
    topBall.classList.add("selected");
  }

  function canPlaceBall(tube, ball) {
    if (ball.originTube === tube) return true;
    if (tube.children.length == 4) return false;
    const topBall = tube.firstElementChild;
    if (!topBall) return true;
    if (topBall.style.backgroundColor === ball.style.backgroundColor)
      return true;
    return false;
  }

  function hasFourConsecutiveSameColors(balls) {
    let consecutiveCount = 1;

    for (let i = 1; i < balls.length; i++) {
      if (balls[i] === balls[i - 1]) {
        consecutiveCount++;
        if (consecutiveCount === 4) {
          return true;
        }
      } else {
        consecutiveCount = 1;
      }
    }

    return false;
  }

  function generateRandomLevel(colorCount, useInitialBallOrder = false) {
    gameWin = false;
    currentColorCount = colorCount;
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

    if (!useInitialBallOrder) {
      // mélanger les balles
      balls = shuffleArray(balls);

      // Vérifiez si quatre balles consécutives ont la même couleur
      while (hasFourConsecutiveSameColors(balls)) {
        balls = shuffleArray(balls);
      }
      initialBallOrder = balls;
    } else {
      balls = initialBallOrder;
    }

    const existingTubes = document.querySelectorAll(".tube");
    existingTubes.forEach((tube) => tube.remove());
    const tubeContainer = document.querySelector("main");
    for (let i = 0; i < colorCount + 2; i++) {
      const tube = document.createElement("div");
      tube.classList.add("tube");
      tubeContainer.appendChild(tube);
    }
    const tubes = document.querySelectorAll(".tube");
    let ballIndex = 0;
    tubes.forEach((tube) => {
      for (let i = 0; i < 4 && ballIndex < balls.length; ++i) {
        const ball = document.createElement("div");
        ball.setAttribute(
          "style",
          "background-color: " + balls[ballIndex] + ";"
        );
        tube.appendChild(ball);
        ballIndex++;
      }
    });
    moveCount = 0;
    document.querySelector("#movements").innerText = moveCount;
    if (selectedBall) {
      selectedBall.classList.remove("selected");
      selectedBall = null;
    }
  }

  function shuffleArray(array) {
    for (let i = array.length - 1; i > 0; i--) {
      const j = Math.floor(Math.random() * (i + 1));
      [array[i], array[j]] = [array[j], array[i]];
    }
    return array;
  }

  function checkWinCondition() {
    let win = true;
    document.querySelectorAll(".tube").forEach((tube) => {
      if (tube.children.length === 4) {
        const color = tube.children[0].style.backgroundColor;
        for (let ball of tube.children) {
          if (ball.style.backgroundColor !== color) {
            win = false;
            break;
          }
        }
      } else if (tube.children.length > 0) {
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

  document.querySelector("#btnRestart").addEventListener("click", function () {
    if (confirm("Voulez-vous vraiment recommencer?")) {
      gameWin = false;
      generateRandomLevel(currentColorCount, true);
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

  const numColorsInput = document.querySelector("#numColors");
  numColorsInput.addEventListener("input", function () {
    this.select();
  });
  numColorsInput.addEventListener("change", function () {
    if (this.value < 3) {
      this.value = 3;
    } else if (this.value > 10) {
      this.value = 10;
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
