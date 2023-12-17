//-----------Var Inits--------------
let confetti = [];
const confettiCount = 300;
const gravity = 0.5;
const terminalVelocity = 5;
const drag = 0.075;
const colors = [
    { front: 'red', back: 'darkred' },
    { front: 'green', back: 'darkgreen' },
    { front: 'blue', back: 'darkblue' },
    { front: 'yellow', back: 'darkyellow' },
    { front: 'orange', back: 'darkorange' },
    { front: 'pink', back: 'darkpink' },
    { front: 'purple', back: 'darkpurple' },
    { front: 'turquoise', back: 'darkturquoise' }
  ];

//-----------Functions--------------
resizeCanvas = () => {
  confetti.forEach(confetto => {
    confetto.position.x = Math.random() * window.innerWidth;
    confetto.position.y = window.innerHeight - 1;
  });
};

initConfetti = () => {
    const container = document.getElementById("canvas");
  
    for (let i = 0; i < confettiCount; i++) {
      let colorIndex = Math.floor(Math.random() * colors.length);
      let color = colors[colorIndex];
  
      // Vérifier si l'objet color est défini
      if (!color) {
        console.error('Couleur non définie pour l\'index', colorIndex);
        continue; // Passer à la prochaine itération de la boucle
      }
  
      let confetto = {
        color: color,
        dimensions: {
          x: Math.random() * 10 + 10,
          y: Math.random() * 20 + 10
        },
        position: {
          x: Math.random() * window.innerWidth,
          y: window.innerHeight - 1
        },
        velocity: {
          x: Math.random() * 50 - 25,
          y: Math.random() * -50
        },
        element: document.createElement("div")
      };
  
      confetto.element.style.position = "absolute";
      confetto.element.style.backgroundColor = confetto.color.front;
      confetto.element.style.width = `${confetto.dimensions.x}px`;
      confetto.element.style.height = `${confetto.dimensions.y}px`;
  
      container.appendChild(confetto.element);
      confetti.push(confetto);
    }
  };
  
  

render = () => {
  confetti.forEach((confetto, index) => {
    // Apply forces to velocity
    confetto.velocity.x -= confetto.velocity.x * drag;
    confetto.velocity.y = Math.min(confetto.velocity.y + gravity, terminalVelocity);

    // Set position
    confetto.position.x += confetto.velocity.x;
    confetto.position.y += confetto.velocity.y;

    // Delete confetti when out of frame
    if (confetto.position.y >= window.innerHeight) {
      confetto.element.remove();
      confetti.splice(index, 1);
    } else {
      confetto.element.style.left = `${confetto.position.x}px`;
      confetto.element.style.top = `${confetto.position.y}px`;
    }
  });

  // Fire off another round of confetti
  if (confetti.length <= 10) initConfetti();

  window.requestAnimationFrame(render);
};

document.addEventListener('DOMContentLoaded', function() {
    //---------Execution--------
    initConfetti();
    render();

    //----------Resize----------
    window.addEventListener('resize', resizeCanvas);

    //------------Click------------
    window.addEventListener('click', initConfetti);
});