document.addEventListener("DOMContentLoaded", function () {
  // Initialisation de la carte
  const myMap = L.map("map");

  L.tileLayer("https://{s}.tile.openstreetmap.org/{z}/{x}/{y}.png", {
    maxZoom: 19,
  }).addTo(myMap);

  let markers = []; // Pour stocker les marqueurs des stations

  function fetchLines() {
    fetch("/lines")
      .then((response) => response.json())
      .then((lineColors) => {
        let latitudes = [];
        let longitudes = [];

        lineColors.forEach((color) => {
          fetch(`/stations/${color}`)
            .then((response) => response.json())
            .then((stations) => {
              // Trier les stations en fonction de leur numéro sur la ligne
              stations.sort((a, b) => {
                const aLigne = a.lignes.find(
                  (ligne) => ligne.couleur === color
                );
                const bLigne = b.lignes.find(
                  (ligne) => ligne.couleur === color
                );
                return aLigne.numero - bLigne.numero;
              });

              const lineCoords = stations.map((station) => [
                station.latitude,
                station.longitude,
              ]);
              latitudes.push(...lineCoords.map((coord) => coord[0]));
              longitudes.push(...lineCoords.map((coord) => coord[1]));

              const polyline = L.polyline(lineCoords, { color: color, weight:5 }).addTo(
                myMap
              );
              polyline.on("click", function () {
                // Suppression des marqueurs précédents
                markers.forEach((marker) => myMap.removeLayer(marker));
                markers = [];

                // Ajout de nouveaux marqueurs pour les stations de cette ligne
                stations.forEach((station) => {
                  const marker = L.marker([
                    station.latitude,
                    station.longitude,
                  ]).addTo(myMap);
                  markers.push(marker);

                  // Ajout d'une popup au marqueur
                  const popupContent =
                    `<strong>${station.nom}</strong><br>` +
                    station.lignes
                      .map(
                        (ligne) => `
        <div style="display: inline-block; width: 20px; height: 20px; background-color: ${ligne.couleur}; color: white; text-align: center; line-height: 20px; margin-right: 5px; border-radius: 4px;">
            ${ligne.numero}
        </div>
    `
                      )
                      .join("");
                  marker.bindPopup(popupContent);
                });
              });
            });
        });

        // Centrage de la carte
        setTimeout(() => {
          let maxLat = Math.max(...latitudes);
          let minLat = Math.min(...latitudes);
          let maxLon = Math.max(...longitudes);
          let minLon = Math.min(...longitudes);

          let centerLat = (maxLat + minLat) / 2;
          let centerLon = (maxLon + minLon) / 2;

          myMap.setView([centerLat, centerLon], 12);
        }, 1000);
      });
  }

  fetchLines();
});
