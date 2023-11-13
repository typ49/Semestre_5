// Établir une connexion Socket.io avec le serveur
const socket = io.connect("http://localhost:8080");

document.addEventListener("DOMContentLoaded", function () {
  const btnConnecter = document.getElementById("btnConnecter");
  const btnEnvoyer = document.getElementById("btnEnvoyer");
  const btnImage = document.getElementById("btnImage");
  const btnRechercher = document.getElementById("btnRechercher");
  const btnFermer = document.getElementById("btnFermer");
  const btnQuitter = document.getElementById("btnQuitter");
  const bcResults = document.getElementById("bcResults");
  const monMessage = document.getElementById("monMessage");
  const loginSpan = document.getElementById("login");
  const recherche = document.getElementById("recherche");
  const main = document.querySelector("main");
  const aside = document.querySelector("aside");

  let userId = null;

  // Se connecter au chat
  btnConnecter.addEventListener("click", function () {
    userId = document.getElementById("pseudo").value.trim();
    if (userId) {
      socket.emit("login", userId); // Envoyer uniquement l'identifiant de l'utilisateur
      loginSpan.textContent = userId;
      document.getElementById("radio2").checked = true;
      // Afficher un message de bienvenue dans le chat
      afficherMessage({
        from: "Serveur",
        text: "Bienvenue " + userId + " !",
        date: Date.now(),
        type: "system",
      });
    } else {
      alert("Veuillez entrer un pseudo.");
    }
  });

  // Écouter l'événement "bienvenue"
  // À la connexion, afficher le message de bienvenue et demander l'historique des messages
  socket.on("bienvenue", function () {
    afficherMessage({
      from: "[admin]",
      text: "Bienvenue " + userId + " !",
      date: Date.now(),
      type: "text",
    });
    socket.emit("demandeHistorique"); // Demande l'historique des messages au serveur
  });

  // Écouter l'événement "message" pour afficher les messages
  socket.on("message", function (msg) {
    afficherMessage(msg);
  });

  // Écouter l'événement "liste" pour mettre à jour la liste des utilisateurs connectés
  socket.on("liste", function (liste) {
    aside.innerHTML = "";
    liste.forEach((user) => {
      const userElement = document.createElement("div");
      userElement.textContent = user; // 'user' est maintenant un identifiant (String)
      aside.appendChild(userElement);
    });
  });

  // Écouter l'événement "historiqueMessages" pour afficher les messages précédents
  socket.on("historiqueMessages", function (messages) {
    messages.forEach((msg) => {
      afficherMessage(msg);
    });
  });

  // Fonction pour afficher un message
function afficherMessage(msg) {
  const date = new Date(msg.date);
  const formattedDate = `[${date.getHours()}:${date
    .getMinutes()
    .toString()
    .padStart(2, "0")}]`;
  let messageClass = "";
  let messageContent = msg.text;

  if (msg.from === "Serveur") {
    messageClass = "system"; // Classe CSS pour les messages du serveur
  } else if (msg.from === userId) {
    messageClass = "moi"; // Classe CSS pour les messages de l'utilisateur actuel
  } else if (msg.text.startsWith("@")) {
    messageClass = "mp"; // Classe CSS pour les messages tagués
    const nameEndIndex = msg.text.indexOf(' ');
    const name = nameEndIndex !== -1 ? msg.text.substring(1, nameEndIndex) : msg.text.substring(1);
    messageContent = `(to ${name}) : ` + (nameEndIndex !== -1 ? msg.text.substring(nameEndIndex + 1) : '');
  } else if (msg.from === "[admin]") {
    messageClass = "admin"; // Classe CSS pour les messages de l'administrateur
  }

  if (msg.type === "image") {
    messageContent = `<img src="${msg.text}" alt="Image" style="max-width: 200px; max-height: 200px;" />`;
  }

  const messageElement = `<p class="${messageClass}">${formattedDate} ${msg.from}: ${messageContent}</p>`;
  main.innerHTML += messageElement;
  main.scrollTop = main.scrollHeight; // Défilement automatique vers le bas
}


  // Envoyer un message
  btnEnvoyer.addEventListener("click", function () {
    let messageContent = monMessage.value.trim();
    let messageType = "text";
    if (messageContent.startsWith("@")) {
      messageType = "mp"; // Message tagué
    }
    sendMessage(messageType, messageContent);
  });

  // Fonction pour envoyer le message
  function sendMessage(type, content) {
    const message = {
      from: userId,
      text: content,
      date: Date.now(),
      type: type,
    };
    socket.emit("message", message);
    monMessage.value = ""; // Clear input after sending
  }

  // Envoyer une image Giphy
  btnImage.addEventListener("click", function () {
    document.getElementById("bcImage").style.display = "block";
  });

  // Rechercher une image Giphy
  btnRechercher.addEventListener("click", function () {
    const query = recherche.value;
    const apiKey = "0X5obvHJHTxBVi92jfblPqrFbwtf1xig";
    fetch(
      `https://api.giphy.com/v1/gifs/search?api_key=${apiKey}&q=${query}&limit=10`
    )
      .then((response) => response.json())
      .then((data) => {
        bcResults.innerHTML = "";
        data.data.forEach((gif) => {
          const imgElement = document.createElement("img");
          imgElement.src = gif.images.fixed_height.url;
          imgElement.alt = gif.title;
          imgElement.addEventListener("click", function () {
            sendMessage("image", gif.images.fixed_height.url);
            document.getElementById("bcImage").style.display = "none";
          });
          bcResults.appendChild(imgElement);
        });
      });
  });

  // Fermer le bloc d'images
  btnFermer.addEventListener("click", function () {
    document.getElementById("bcImage").style.display = "none";
  });

  // Envoyer un message avec la touche Entrée
  monMessage.addEventListener("keydown", function (event) {
    if (event.key === "Enter") {
      if (event.shiftKey) {
        return;
      }
      event.preventDefault();
      sendMessage("text", monMessage.value);
    }
  });

  // Se déconnecter
  btnQuitter.addEventListener("click", function () {
    if (userId) {
      socket.emit("logout", userId);
      loginSpan.textContent = "";
      userId = null;
      document.getElementById("radio1").checked = true;
      console.log("Vous vous êtes déconnecté.");
    }
  });
});
