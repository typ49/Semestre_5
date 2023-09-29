"use strict";

// Constantes du jeu
const WIDTH = 800;
const HEIGHT = 512;
const GRAVITY = 700;
const FLAP_STRENGTH = -200;
const OBSTACLE_WIDTH = 50;
const OBSTACLE_SPACING = 80;
const OBSTACLE_INTERVAL = 3.5  ; // 2 secondes
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
  }

  update(deltaTime) {
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
  }

  render() {
    this.ctx.clearRect(0, 0, WIDTH, HEIGHT);
    this.bird.render(this.ctx);

    for (let obstacle of this.obstacles) {
      obstacle.render(this.ctx);
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
