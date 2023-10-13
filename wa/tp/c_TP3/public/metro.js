let myMap = L.map('map').setView([46.603354, 1.888334], 5); // Coordonnées approximatives du centre de la France pour l'initialisation

L.tileLayer('https://{s}.tile.openstreetmap.org/{z}/{x}/{y}.png', {
    maxZoom: 19,
}).addTo(myMap);

function fetchLines() {
    fetch('/lines')
        .then(response => response.json())
        .then(data => {
            for (let couleur in data) {
                L.polyline(data[couleur]).addTo(myMap).on('click', function() {
                    fetchStations(couleur);
                });
            }
        });
}

fetchLines();

let markers = [];

function fetchStations(couleur) {
    // Supprimer les marqueurs précédents
    markers.forEach(marker => myMap.removeLayer(marker));

    fetch(`/stations/${couleur}`)
        .then(response => response.json())
        .then(stations => {
            stations.forEach(station => {
                const marker = L.marker([station.lat, station.long]).addTo(myMap);
                marker.bindPopup(`<strong>${station.nom}</strong><br>${Object.entries(station.lignes).map(([color, num]) => `${color}: ${num}`).join('<br>')}`);
                markers.push(marker);
            });
        });
}

document.addEventListener("DOMContentLoaded", function() {
    // Initialisation de la carte
    const myMap = L.map('map').setView([43.0621, 141.3544], 12); // Coordonnées approximatives du centre de Sapporo

    L.tileLayer('https://{s}.tile.openstreetmap.org/{z}/{x}/{y}.png', {
        maxZoom: 19,
    }).addTo(myMap);

    // Récupération des lignes et affichage sur la carte
    fetch('/lines')
        .then(response => response.json())
        .then(lines => {
            lines.forEach(line => {
                L.polyline(line).addTo(myMap).on('click', function() {
                    fetchStations(line);
                });
            });
        });

    // Fonction pour récupérer les stations d'une ligne spécifique
    let markers = [];
    function fetchStations(couleur) {
        // Supprimer les marqueurs précédents
        markers.forEach(marker => myMap.removeLayer(marker));

        fetch(`/stations/${couleur}`)
            .then(response => response.json())
            .then(stations => {
                stations.forEach(station => {
                    const marker = L.marker([station.latitude, station.longitude]).addTo(myMap);
                    marker.bindPopup(`<strong>${station.nom}</strong><br>${Object.entries(station.lignes).map(([color, num]) => `${color}: ${num}`).join('<br>')}`);
                    markers.push(marker);
                });
            });
    }
});
