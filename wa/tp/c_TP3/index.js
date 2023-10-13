/**********************************************************
 *              Chargement des modules
 **********************************************************/

// FileSystem : lecture de fichiers
const fs = require("fs");
const readline = require("readline");

// Express : serveur web
var express = require("express");

/*****************************************************
 *             Lancement du serveur web
 *****************************************************/
var app = express();

app.listen(8080, function () {
  console.log("C'est parti ! En attente de connexion sur le port 8080...");
});

// Configuration d'express pour utiliser le répertoire "public"
app.use(express.static("public"));
// par défaut, envoie le fichier index.html
app.get("/", function (req, res) {
  res.sendFile(__dirname + "/public/metro.html");
});

/********************************************************
 *          Définition de routes spécifiques
 ********************************************************/

app.get("/lines", function (req, res) {
  console.log("Reçu : GET /lines");
  res.setHeader("Content-type", "application/json");
  res.json({ TODO: "à compléter " });
});

app.get("/stations/:couleur", function (req, res) {
  console.log("Reçu : GET /stations/" + req.params.couleur);
  res.setHeader("Content-type", "application/json");
  res.json({ TODO: "à compléter " });
});

/******************************************************************************
 *                      Gestion des stations et des lignes
 ******************************************************************************/

// lecture et initialisation de l'ensemble des stations
// source : https://nodejs.org/api/readline.html#readline_example_read_file_stream_line_by_line
(async function processLineByLine(file) {
  const fileStream = fs.createReadStream(file);

  const rl = readline.createInterface({
    input: fileStream,
    crlfDelay: Infinity,
  });

  for await (const line of rl) {
    console.log(`Line from file: ${line}`); // <--- TODO remplacer
  }
})("./stations.csv");
