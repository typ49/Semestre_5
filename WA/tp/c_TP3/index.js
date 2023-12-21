/**********************************************************
 *              Chargement des modules
 **********************************************************/
const fs = require("fs");
const readline = require("readline");
const express = require("express");

/*****************************************************
 *             Lancement du serveur web
 *****************************************************/
var app = express();

app.listen(8080, function () {
  console.log("C'est parti ! En attente de connexion sur le port 8080...");
});

app.use(express.static("public"));
app.get("/", function (req, res) {
  res.sendFile(__dirname + "/public/metro.html");
});

/********************************************************
 *          Définition de routes spécifiques
 ********************************************************/
app.get("/lines", function (req, res) {
  console.log("Reçu : GET /lines");
  res.setHeader("Content-type", "application/json");
  const lines = [...new Set(stations.flatMap(station => station.lignes.map(ligne => ligne.couleur)))];
  res.json(lines);
});

app.get("/stations/:couleur", function (req, res) {
  console.log("Reçu : GET /stations/" + req.params.couleur);
  res.setHeader("Content-type", "application/json");
  const filteredStations = stations.filter(station => station.lignes.some(ligne => ligne.couleur === req.params.couleur));
  res.json(filteredStations);
});

/******************************************************************************
 *                      Gestion des stations et des lignes
 ******************************************************************************/
class Ligne {
  constructor(couleur, numero) {
    this.couleur = couleur;
    this.numero = numero;
  }
}

class Station {
  constructor(nom, latitude, longitude, lignes = []) {
    this.nom = nom;
    this.latitude = latitude;
    this.longitude = longitude;
    this.lignes = lignes;
  }
}

let stations = [];

async function processLineByLine(file) {
  try {
    const fileStream = fs.createReadStream(file);

    const rl = readline.createInterface({
      input: fileStream,
      crlfDelay: Infinity,
    });

    for await (const line of rl) {
      const [nom, lat, long, ...ligneParts] = line.split(",");
      const lignes = [];
      for (let i = 0; i < ligneParts.length; i += 2) {
        lignes.push(new Ligne(ligneParts[i], parseInt(ligneParts[i + 1])));
      }
      stations.push(new Station(nom, parseFloat(lat), parseFloat(long), lignes));
    }
  } catch (error) {
    console.error("Erreur lors de la lecture du fichier:", error);
  }
}

processLineByLine("./stations.csv");

