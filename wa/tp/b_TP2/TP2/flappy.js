"use strict";

// Constantes du jeu
const WIDTH = 800;
const HEIGHT = 512;
const GRAVITY = 700;
const FLAP_STRENGTH = -200;
const OBSTACLE_WIDTH = 50;
const OBSTACLE_SPACING = 80;
const OBSTACLE_INTERVAL = 3.5;
const OBSTACLE_SPEED = 70;

class Bird {
  constructor() {
    this.x = WIDTH / 2 - 18;
    this.y = HEIGHT / 2 - 13;
    this.height = 26;
    this.width = 36;
    this.speed = 0;
  }

  update(deltaTime) {
    this.speed += GRAVITY * deltaTime;
    this.y += this.speed * deltaTime;

    if (this.y < 0) {
      this.y = 0;
      this.speed = 0;
    }
    if (this.y + this.height > HEIGHT) {
      this.y = HEIGHT - this.height;
      this.speed = 0;
    }
  }

  flap() {
    this.speed = FLAP_STRENGTH;
  }

  collidesWith(obstacle) {
    return (
      this.x < obstacle.x + OBSTACLE_WIDTH &&
      this.x + this.width > obstacle.x &&
      (this.y < obstacle.topHeight ||
        this.y + this.height > HEIGHT - obstacle.bottomHeight)
    );
  }

  render(ctx) {
    ctx.strokeStyle = "black";
    ctx.strokeRect(this.x, this.y, this.width, this.height);
  }
}

class Obstacle {
  constructor() {
    this.topHeight = Math.random() * (HEIGHT - OBSTACLE_SPACING);
    this.bottomHeight = HEIGHT - this.topHeight - OBSTACLE_SPACING;
    this.x = WIDTH;
    this.counted = false;
  }

  update(deltaTime) {
    this.x -= OBSTACLE_SPEED * deltaTime;
  }

  render(ctx) {
    ctx.fillStyle = "green";
    ctx.fillRect(this.x, 0, OBSTACLE_WIDTH, this.topHeight);
    ctx.fillRect(
      this.x,
      HEIGHT - this.bottomHeight,
      OBSTACLE_WIDTH,
      this.bottomHeight
    );
  }
}

class Game {
  constructor(ctx) {
    this.ctx = ctx;
    this.bird = new Bird();
    this.obstacles = [];
    this.obstacleTimer = 0;
    this.score = 0;
    this.state = "START";
    this.menuOptionSelected = 0;
    this.menuOptions = ["Start", "Top Scores"];
  }

  endGame() {
    this.gameOver = true;
    this.bird.speed = 0;
    this.bird.y = HEIGHT / 2 - this.bird.height / 2;
    this.obstacles = [];
    this.obstacleTimer = 0;
    this.score = 0;
  }

  resetGame() {
    this.bird = new Bird();
    this.obstacles = [];
    this.obstacleTimer = 0;
    this.score = 0;
  }

  checkAndSaveScore() {
    let topScores = JSON.parse(localStorage.getItem("topScores") || "[]");
    if (topScores.length < 10 || this.score > topScores[9].score) {
      let playerName = prompt(
        "Félicitations ! Vous avez atteint le Top 10. Entrez votre pseudo :"
      );
      topScores.push({ name: playerName, score: this.score });
      topScores.sort((a, b) => b.score - a.score);
      if (topScores.length > 10) {
        topScores.pop();
      }
      localStorage.setItem("topScores", JSON.stringify(topScores));
    }
  }

  update(deltaTime) {
    if (this.state !== "PLAYING") {
      return;
    }
    this.bird.update(deltaTime);

    this.obstacleTimer += deltaTime;
    if (this.obstacleTimer >= OBSTACLE_INTERVAL) {
      this.obstacles.push(new Obstacle());
      this.obstacleTimer -= OBSTACLE_INTERVAL;
    }

    for (let i = this.obstacles.length - 1; i >= 0; i--) {
      this.obstacles[i].update(deltaTime);
      if (this.obstacles[i].x + OBSTACLE_WIDTH < 0) {
        this.obstacles.splice(i, 1);
      }
    }
    for (let obstacle of this.obstacles) {
      if (!obstacle.counted && this.bird.x > obstacle.x + OBSTACLE_WIDTH) {
        this.score++;
        obstacle.counted = true;
      }
      if (this.bird.collidesWith(obstacle)) {
        this.checkAndSaveScore();
        this.state = "GAME_OVER"; // arrêter le jeu
        break;
      }
    }
  }

  render() {
    this.ctx.clearRect(0, 0, WIDTH, HEIGHT);
    this.bird.render(this.ctx);

    for (let obstacle of this.obstacles) {
      obstacle.render(this.ctx);
    }

    switch (this.state) {
      case "START":
        this.displayStartMenu();
        break;
      case "PAUSED":
        this.displayMessage("Pause");
        break;
      case "GAME_OVER":
        this.displayGameOver();
        break;
      case "SCORES":
        this.displayTopScores();
        break;
    }

    // score
    this.ctx.fillStyle = "black";
    this.ctx.font = "20px Arial";
    this.ctx.textAlign = "left";
    this.ctx.fillText("Score: " + this.score, 10, 30);
  }

  displayStartMenu() {
    this.ctx.fillStyle = "white";
    this.ctx.font = "30px Arial";
    this.ctx.textAlign = "center";
    this.ctx.fillText("Flappy Bird", WIDTH / 2, HEIGHT / 2 - 100);

    for (let i = 0; i < this.menuOptions.length; i++) {
      let posY = HEIGHT / 2 + i * 60; // Ajustez la position Y pour espacer les options

      // Si l'option est sélectionnée, affichez un cadre autour
      if (i === this.menuOptionSelected) {
        this.ctx.strokeStyle = "yellow";
        this.ctx.strokeRect(WIDTH / 2 - 150, posY - 30, 300, 40);
        this.ctx.fillStyle = "yellow";
      } else {
        this.ctx.fillStyle = "white";
      }
      this.ctx.fillText(this.menuOptions[i], WIDTH / 2, posY);
    }
  }

  displayMessage(message) {
    this.ctx.fillStyle = "rgba(0, 0, 0, 0.5)";
    this.ctx.fillRect(0, 0, WIDTH, HEIGHT);
    this.ctx.fillStyle = "white";
    this.ctx.font = "30px Arial";
    this.ctx.textAlign = "center";
    this.ctx.fillText(message, WIDTH / 2, HEIGHT / 2);
  }

  displayTopScores() {
    let topScores = JSON.parse(localStorage.getItem("topScores") || "[]");
    this.ctx.fillStyle = "white";
    this.ctx.font = "20px Arial";
    this.ctx.textAlign = "center";
    this.ctx.fillText(
      "Top 10 Scores:\tpress Backspace to return",
      WIDTH / 2,
      HEIGHT / 2 - 120
    );

    this.ctx.textAlign = "left";
    for (let i = 0; i < topScores.length; i++) {
      let scoreText = `${i + 1}.`.padEnd(4);
      let nameText = topScores[i].name.padEnd(20, ".");
      let finalScoreText = topScores[i].score.toString().padStart(5);
      this.ctx.fillText(
        scoreText + nameText + finalScoreText,
        WIDTH / 4,
        HEIGHT / 2 - 90 + i * 25
      );
    }

    this.ctx.textAlign = "center";
    this.ctx.fillText(
      "press Backspace to return",
      WIDTH / 2,
      HEIGHT / 2 + 120
    );
  }

  displayGameOver() {
    // Afficher le message "Game Over"
    this.ctx.fillStyle = "rgba(0, 0, 0, 0.5)";
    this.ctx.fillRect(0, 0, WIDTH, HEIGHT);
    this.ctx.fillStyle = "white";
    this.ctx.font = "30px Arial";
    this.ctx.textAlign = "center";
    this.ctx.fillText(
      "Game Over - press 's' to restart",
      WIDTH / 2,
      HEIGHT / 2 - 150
    );

    // Afficher les scores
    let topScores = JSON.parse(localStorage.getItem("topScores") || "[]");
    this.ctx.font = "20px Arial";
    this.ctx.fillText("Top 10 Scores:", WIDTH / 2, HEIGHT / 2 - 120);

    for (let i = 0; i < topScores.length; i++) {
      this.ctx.fillText(
        `${i + 1}. ${topScores[i].name}: ${topScores[i].score}`,
        WIDTH / 2,
        HEIGHT / 2 - 90 + i * 25
      );
    }
  }
}

document.addEventListener("DOMContentLoaded", function () {
  const canvas = document.getElementById("cvs");
  const ctx = canvas.getContext("2d");
  const game = new Game(ctx);

  document.addEventListener("keydown", function (e) {
    if (e.code === "Space") {
      game.bird.flap();
    }
    switch (game.state) {
      case "START":
        if (e.code === "ArrowUp" && game.menuOptionSelected > 0) {
          game.menuOptionSelected--;
        } else if (
          e.code === "ArrowDown" &&
          game.menuOptionSelected < game.menuOptions.length - 1
        ) {
          game.menuOptionSelected++;
        } else if (e.code === "Space" || e.code === "Enter") {
          if (game.menuOptionSelected === 0) {
            game.state = "PLAYING";
            game.resetGame();
          } else if (game.menuOptionSelected === 1) {
            game.state = "SCORES"; // Ajoutez un nouvel état pour afficher les scores
          }
        }
        break;
      case "PLAYING":
        if (e.code === "Space") {
          game.bird.flap();
        } else if (e.code === "KeyP") {
          game.state = "PAUSED";
        }
        break;
      case "PAUSED":
        if (e.code === "KeyP") {
          game.state = "PLAYING";
        }
        break;
      case "GAME_OVER":
        if (e.code === "KeyS") {
          game.resetGame();
          game.state = "START";
        }
        break;
      case "SCORES":
        if (e.code === "Backspace") {
          game.state = "START";
        }
        break;
    }
  });

  let lastTime = Date.now();
  (function loop() {
    requestAnimationFrame(loop);
    let now = Date.now();
    let deltaTime = (now - lastTime) / 1000; // Convertir en secondes
    game.update(deltaTime);
    game.render();
    lastTime = now;
  })();
});
