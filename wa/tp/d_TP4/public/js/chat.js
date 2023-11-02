// Établir une connexion Socket.io avec le serveur
const socket = io.connect('http://localhost:8080');

document.addEventListener("DOMContentLoaded", function() {
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
    const aside = document.querySelector("aside");

    let userId = null;
    let userColor = null;

    // Générer une couleur aléatoire
    function getRandomColor() {
        const letters = '0123456789ABCDEF';
        let color = '#';
        for (let i = 0; i < 6; i++) {
            color += letters[Math.floor(Math.random() * 16)];
        }
        return color;
    }

    // Se connecter au chat
    btnConnecter.addEventListener('click', function() {
        userId = document.getElementById("pseudo").value;
        userColor = getRandomColor();
        if(userId) {
            socket.emit('login', {id: userId, color: userColor});
            loginSpan.textContent = userId;
            document.getElementById("radio2").checked = true;
        }
    });

    // Écouter l'événement "bienvenue"
    socket.on('bienvenue', function(id) {
        console.log('Bienvenue, ' + id);
    });

    // Écouter l'événement "message" pour afficher les messages
    socket.on('message', function(msg) {
        afficherMessage(msg);
    });

    // Écouter l'événement "liste" pour mettre à jour la liste des utilisateurs connectés
    socket.on('liste', function(userList) {
        aside.innerHTML = '';
        userList.forEach(user => {
            aside.innerHTML += `<p>${user}</p>`;
        });
    });

    // Écouter l'événement "historiqueMessages" pour afficher les messages précédents
    socket.on('historiqueMessages', function(messages) {
        messages.forEach(msg => {
            afficherMessage(msg);
        });
    });

    // Fonction pour afficher un message
    function afficherMessage(msg) {
        const main = document.querySelector("main");
        const date = new Date(msg.date);
        const formattedDate = `${date.getHours()}:${date.getMinutes()}:${date.getSeconds()}`;
        const messageColor = msg.color || "#000";
        if(msg.type === 'text') {
            main.innerHTML += `<p style="color: ${messageColor};">[${formattedDate}] ${msg.from}: ${msg.text}</p>`;
        } else if(msg.type === 'image') {
            main.innerHTML += `<p style="color: ${messageColor};">[${formattedDate}] ${msg.from}: <img src="${msg.text}" alt="Giphy image" /></p>`;
        }
    }

    // Envoyer un message
    btnEnvoyer.addEventListener('click', function() {
        sendMessage('text', monMessage.value);
    });

    // Envoyer une image Giphy
    btnImage.addEventListener('click', function() {
        document.getElementById("bcImage").style.display = 'block';
    });

    // Rechercher une image Giphy
    btnRechercher.addEventListener('click', function() {
        const query = recherche.value;
        const apiKey = "0X5obvHJHTxBVi92jfblPqrFbwtf1xig";
        fetch(`https://api.giphy.com/v1/gifs/search?api_key=${apiKey}&q=${query}&limit=10`)
        .then(response => response.json())
        .then(data => {
            bcResults.innerHTML = '';
            data.data.forEach(gif => {
                const imgElement = document.createElement('img');
                imgElement.src = gif.images.fixed_height.url;
                imgElement.alt = gif.title;
                imgElement.addEventListener('click', function() {
                    sendMessage('image', gif.images.fixed_height.url);
                    document.getElementById("bcImage").style.display = 'none';
                });
                bcResults.appendChild(imgElement);
            });
        });
    });

    // Fermer le bloc d'images
    btnFermer.addEventListener('click', function() {
        document.getElementById("bcImage").style.display = 'none';
    });

    // Envoyer un message avec la touche Entrée
    monMessage.addEventListener('keydown', function(event) {
        if (event.key === "Enter") {
            if (event.shiftKey) {
                return;
            }
            event.preventDefault();
            sendMessage('text', monMessage.value);
        }
    });

    // Fonction pour envoyer le message
    function sendMessage(type, content) {
        const message = { type, from: userId, to: null, text: content, date: Date.now(), color: userColor };
        socket.emit('message', message);
        if(type === 'text') {
            monMessage.value = '';
        }
    }

    // Se déconnecter
    btnQuitter.addEventListener('click', function() {
        socket.emit('logout');
        document.getElementById("radio1").checked = true;
        console.log('Vous vous êtes déconnecté.');
    });
});
