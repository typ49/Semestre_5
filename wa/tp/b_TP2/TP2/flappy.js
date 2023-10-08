(function () {
  "use strict";

  // Constants for the game and sprites
  const SPRITE_SHEET_URL = "flappy.png";
  const SPRITE_SHEET_WIDTH = 1024;
  const SPRITE_SHEET_HEIGHT = 1024;
  const WIDTH = 800;
  const HEIGHT = 512;
  const GRAVITY = 700;
  const FLAP_STRENGTH = -200;
  const OBSTACLE_WIDTH = 50;
  const OBSTACLE_SPACING = 80;
  const OBSTACLE_INTERVAL = 3.5;
  const OBSTACLE_SPEED = 70;

  // Constants for the sprites
  // - Background:
  const SPRITE_BACKGROUND_WIDTH = 288;
  const SPRITE_BACKGROUND_HEIGHT = 512;
  const SPRITE_BACKGROUND_DAY_X = 0;
  const SPRITE_BACKGROUND_DAY_Y = 0;
  const SPRITE_BACKGROUND_NIGHT_X = 292;
  const SPRITE_BACKGROUND_NIGHT_Y = 0;

  // - Pipes:
  const SPRITE_PIPE_WIDTH = 52;
  const SPRITE_PIPE_HEIGHT = 320;
  const SPRITE_PIPE_TOP_X = 112;
  const SPRITE_PIPE_TOP_Y = 646;
  const SPRITE_PIPE_BOTTOM_X = 168;
  const SPRITE_PIPE_BOTTOM_Y = 646;

  // - Floor:
  const SPRITE_FLOOR_WIDTH = 335;
  const SPRITE_FLOOR_HEIGHT = 112;
  const SPRITE_FLOOR_X = 584;
  const SPRITE_FLOOR_Y = 0;

  // - Birds:
  const WING_STATE_DOWN = -1;
  const WING_STATE_MIDDLE = 0;
  const WING_STATE_UP = 1;

  const SPRITE_BIRD_WIDTH = 36;
  const SPRITE_BIRD_HEIGHT = 26;
  const SPRITE_BIRD_YELLOW_1_X = 6;
  const SPRITE_BIRD_YELLOW_1_Y = 981;
  const SPRITE_BIRD_YELLOW_2_X = 62;
  const SPRITE_BIRD_YELLOW_2_Y = 981;
  const SPRITE_BIRD_YELLOW_3_X = 118;
  const SPRITE_BIRD_YELLOW_3_Y = 981;

  const GAME_STATES = {
    START: "START",
    PLAYING: "PLAYING",
    PAUSED: "PAUSED",
    GAME_OVER: "GAME_OVER",
    SCORES: "SCORES",
  };

  const SPRITE_IMAGE = new Image();
  SPRITE_IMAGE.src = SPRITE_SHEET_URL;
  SPRITE_IMAGE.onload = function () {
    console.log("Sprite sheet loaded");
  };

  class Bird {
    constructor() {
      this.x = WIDTH / 2 - 18;
      this.y = HEIGHT / 2 - 13;
      this.height = 26;
      this.width = 36;
      this.speed = 0;
      this.frame = 0;
      this.frameCounter = 0;
    }

    update(deltaTime) {
      this.speed += GRAVITY * deltaTime;
      this.y += this.speed * deltaTime;
  
      // Empêcher l'oiseau de dépasser le bas de l'écran
      if (this.y + this.height > HEIGHT - SPRITE_FLOOR_HEIGHT) {
          this.y = HEIGHT - this.height - SPRITE_FLOOR_HEIGHT;
          this.speed = 0;
      }
  
      // Empêcher l'oiseau de dépasser le haut de l'écran
      if (this.y < 0) {
          this.y = 0;
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
      let cadres = [
        { x: SPRITE_BIRD_YELLOW_1_X, y: SPRITE_BIRD_YELLOW_1_Y },
        { x: SPRITE_BIRD_YELLOW_2_X, y: SPRITE_BIRD_YELLOW_2_Y },
        { x: SPRITE_BIRD_YELLOW_3_X, y: SPRITE_BIRD_YELLOW_3_Y },
      ];

      let spriteX = cadres[this.frame].x;
      let spriteY = cadres[this.frame].y;

      this.frameCounter++;
      if (this.frameCounter > 10) {
        this.frame = (this.frame + 1) % cadres.length;
        this.frameCounter = 0;
      }

      ctx.drawImage(
        SPRITE_IMAGE,
        spriteX,
        spriteY,
        SPRITE_BIRD_WIDTH,
        SPRITE_BIRD_HEIGHT,
        this.x,
        this.y,
        this.width,
        this.height
      );
    }
  }

  class Obstacle {
    constructor() {
      this.topHeight =
        Math.random() * (HEIGHT - 2 * SPRITE_FLOOR_HEIGHT - OBSTACLE_SPACING) +
        SPRITE_FLOOR_HEIGHT;
      this.bottomHeight = HEIGHT - this.topHeight - OBSTACLE_SPACING;
      this.x = WIDTH;
      this.counted = false;
    }

    update(deltaTime) {
      this.x -= OBSTACLE_SPEED * deltaTime;
    }

    render(ctx) {
      // Dessin du tuyau du haut
      ctx.drawImage(
        SPRITE_IMAGE,
        SPRITE_PIPE_TOP_X,
        SPRITE_PIPE_TOP_Y + SPRITE_PIPE_HEIGHT - this.topHeight, // Ajustement de la position Y de la source
        SPRITE_PIPE_WIDTH,
        this.topHeight, // Utilisation de topHeight comme hauteur
        this.x,
        0,
        OBSTACLE_WIDTH,
        this.topHeight
      );

      // Dessin du tuyau du bas
      ctx.drawImage(
        SPRITE_IMAGE,
        SPRITE_PIPE_BOTTOM_X,
        SPRITE_PIPE_BOTTOM_Y,
        SPRITE_PIPE_WIDTH,
        this.bottomHeight, // Utilisation de bottomHeight comme hauteur
        this.x,
        HEIGHT - this.bottomHeight,
        OBSTACLE_WIDTH,
        this.bottomHeight
      );
    }
  }

  class Game {
    constructor(ctx) {
      this.backgroundX = 0;
      this.floorX = 0;
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
      this.state = "GAME_OVER"; // arrêter le jeu
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
      this.backgroundX -= OBSTACLE_SPEED * 0.5 * deltaTime;
      if (this.backgroundX <= -WIDTH) {
        this.backgroundX = 0;
      }

      this.floorX -= OBSTACLE_SPEED * deltaTime;
      if (this.floorX <= -WIDTH) {
        this.floorX = 0;
      }
    }

    render() {
      this.ctx.clearRect(0, 0, WIDTH, HEIGHT);

      // Affichage du sprite de l'arrière-plan avec parallaxe
      this.ctx.drawImage(
        SPRITE_IMAGE,
        SPRITE_BACKGROUND_DAY_X,
        SPRITE_BACKGROUND_DAY_Y,
        SPRITE_BACKGROUND_WIDTH,
        SPRITE_BACKGROUND_HEIGHT,
        this.backgroundX,
        0,
        WIDTH,
        HEIGHT
      );
      this.ctx.drawImage(
        SPRITE_IMAGE,
        SPRITE_BACKGROUND_DAY_X,
        SPRITE_BACKGROUND_DAY_Y,
        SPRITE_BACKGROUND_WIDTH,
        SPRITE_BACKGROUND_HEIGHT,
        this.backgroundX + WIDTH,
        0,
        WIDTH,
        HEIGHT
      );

      // Affichage de l'oiseau
      this.bird.render(this.ctx);

      // Affichage des obstacles (tuyaux)
      for (let obstacle of this.obstacles) {
        obstacle.render(this.ctx);
      }

      // Affichage du score
      this.ctx.fillStyle = "black";
      this.ctx.font = "20px Arial";
      this.ctx.textAlign = "left";
      this.ctx.fillText("Score: " + this.score, 10, 30);

      // affiche des commandes : pause, saut et retour au menu
      this.ctx.fillText("P: pause", 10, 60);
      this.ctx.fillText("Space: saut", 10, 90);

      // Affichage du sprite du sol en mouvement
      this.ctx.drawImage(
        SPRITE_IMAGE,
        SPRITE_FLOOR_X,
        SPRITE_FLOOR_Y,
        SPRITE_FLOOR_WIDTH,
        SPRITE_FLOOR_HEIGHT,
        this.floorX,
        HEIGHT - SPRITE_FLOOR_HEIGHT,
        WIDTH,
        SPRITE_FLOOR_HEIGHT
      );
      this.ctx.drawImage(
        SPRITE_IMAGE,
        SPRITE_FLOOR_X,
        SPRITE_FLOOR_Y,
        SPRITE_FLOOR_WIDTH,
        SPRITE_FLOOR_HEIGHT,
        this.floorX + WIDTH,
        HEIGHT - SPRITE_FLOOR_HEIGHT,
        WIDTH,
        SPRITE_FLOOR_HEIGHT
      );

      // Gestion de l'affichage en fonction de l'état du jeu
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
    }

    displayStartMenu() {
      this.ctx.fillStyle = "rgba(0, 0, 0, 0.5)";
      this.ctx.fillRect(0, 0, WIDTH, HEIGHT);
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
      this.ctx.fillStyle = "rgba(0, 0, 0, 0.5)";
      this.ctx.fillRect(0, 0, WIDTH, HEIGHT);
      let topScores = JSON.parse(localStorage.getItem("topScores") || "[]");
      this.ctx.fillStyle = "white";
      this.ctx.font = "20px Arial";
      this.ctx.textAlign = "center";
      this.ctx.fillText("Top 10 Scores:", WIDTH / 2, HEIGHT / 2 - 120);
      this.ctx.fillText(
        "press Backspace to return",
        WIDTH / 2,
        HEIGHT / 2 + 190
      );
  
      // Positions x fixes pour le rang, le nom et le score
      const rankX = WIDTH / 4;
      const nameX = rankX + 50; // Ajustez selon la largeur maximale du rang
      const scoreX = nameX + 250; // Ajustez selon la largeur maximale du nom
  
      this.ctx.textAlign = "left";
      for (let i = 0; i < topScores.length; i++) {
          let scoreText = `${i + 1}.`;
          let nameText = topScores[i].name;
          let finalScoreText = topScores[i].score.toString();
  
          this.ctx.fillText(scoreText, rankX, HEIGHT / 2 - 90 + i * 25);
          this.ctx.fillText(nameText, nameX, HEIGHT / 2 - 90 + i * 25);
  
          // Dessiner des points pour relier le nom au score
          let endNameX = nameX + this.ctx.measureText(nameText).width;
          for (let j = endNameX + 5; j < scoreX - 5; j += 10) {
              this.ctx.fillText(".", j, HEIGHT / 2 - 90 + i * 25);
          }
  
          this.ctx.textAlign = "right"; // Pour aligner les scores à droite
          this.ctx.fillText(finalScoreText, scoreX, HEIGHT / 2 - 90 + i * 25);
          this.ctx.textAlign = "left"; // Réinitialiser l'alignement pour les autres éléments
      }
  }
  

    displayGameOver() {
      // Afficher le message "Game Over"
      this.ctx.fillStyle = "rgba(0, 0, 0, 0.5)";
      this.ctx.fillRect(0, 0, WIDTH, HEIGHT);
      this.ctx.fillStyle = "white";
      this.ctx.font = "30px Arial";
      this.ctx.textAlign = "center";
      this.ctx.fillText("Game Over", WIDTH / 2, HEIGHT / 2 - 150);
      this.ctx.fillText("press S to restart", WIDTH / 2, HEIGHT / 2 + 180);

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
      if (e.code === "Space" && game.state === GAME_STATES.PLAYING) {
        game.bird.flap();
      }
      switch (game.state) {
        case GAME_STATES.START:
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
        case GAME_STATES.PLAYING:
          if (e.code === "Space") {
            game.bird.flap();
          } else if (e.code === "KeyP") {
            game.state = "PAUSED";
          }
          break;
        case GAME_STATES.PAUSED:
          if (e.code === "KeyP") {
            game.state = "PLAYING";
          }
          break;
        case GAME_STATES.GAME_OVER:
          if (e.code === "KeyS") {
            game.resetGame();
            game.state = "START";
          }
          break;
        case GAME_STATES.SCORES:
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
      let deltaTime = Math.min((now - lastTime) / 1000, 0.1); // Limite à 0.1 seconde
      game.update(deltaTime);
      game.render();
      lastTime = now;
    })();
  });
})();
