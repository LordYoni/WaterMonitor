window.onload = function () {
  fetchData(); // Initial call to load data when the page loads
  setInterval(fetchData, 5000); // Refresh data every 5 seconds
};

// Function to calculate the time since the last update
function timeSince(lastUpdate) {
  const now = new Date();
  const updateTime = new Date(lastUpdate.replace(" ", "T"));
  const diffInMs = now - updateTime;
  const diffInMinutes = Math.floor(diffInMs / 1000 / 60);

  if (diffInMinutes < 60) {
    return `${diffInMinutes}m`;
  } else if (diffInMinutes < 1440) {
    const diffInHours = Math.floor(diffInMinutes / 60);
    return `${diffInHours}h`;
  } else {
    const diffInDays = Math.floor(diffInMinutes / 60 / 24);
    return `${diffInDays}d`;
  }
}

// Function to calculate the state of the system
function state(lastUpdate) {
  const now = new Date();
  const updateTime = new Date(lastUpdate.replace(" ", "T"));
  const diffInMs = now - updateTime;
  const diffInHours = diffInMs / 1000 / 60 / 60;

  if (diffInHours < 3) {
    return "Running";
  } else {
    return "Not running";
  }
}

// Fonction pour obtenir la couleur du pH
function getColorpH(pH) {
  if (pH < 5.5) {
    return "red"; // Trop acide
  } else if (pH >= 5.5 && pH <= 7.0) {
    return "green"; // Gamme normale pour un marais
  } else {
    return "gold"; // Trop alcalin
  }
}

// Fonction pour obtenir la couleur du TDS
function getColorTDS(TDS) {
  if (TDS < 100) {
    return "green"; // Faible TDS (idéal pour un marais)
  } else if (TDS >= 100 && TDS <= 500) {
    return "gold"; // TDS modéré
  } else {
    return "red"; // TDS élevé (indication d'une contamination ou d'un problème)
  }
}

// Fonction pour obtenir la couleur de l'Oxygène dissous
function getColorOx(Oxygen) {
  if (Oxygen < 4) {
    return "red"; // Faible en oxygène (problème)
  } else if (Oxygen >= 4 && Oxygen <= 7) {
    return "green"; // Oxygène dans la norme pour un marais
  } else {
    return "gold"; // Oxygène très élevé (peut indiquer une photosynthèse excessive)
  }
}

// Fonction pour obtenir la couleur de la Conductivité
function getColorConductivity(Conductivity) {
  if (Conductivity < 150) {
    return "green"; // Faible conductivité (eau douce)
  } else if (Conductivity >= 150 && Conductivity <= 500) {
    return "gold"; // Conductivité modérée (eau légèrement minéralisée)
  } else {
    return "red"; // Haute conductivité (eau salée ou contamination)
  }
}

// Fonction pour obtenir la couleur de la Température
function getColorTemp(Temperature) {
  if (Temperature < 15) {
    return "green"; // Température basse (frais)
  } else if (Temperature >= 15 && Temperature <= 25) {
    return "gold"; // Température normale (idéal pour la vie aquatique)
  } else {
    return "red"; // Température élevée (peut être problématique pour la faune/flore)
  }
}

// Fonction pour obtenir la couleur du temps écoulé depuis la dernière mise à jour
function getColorTime(lastUpdate) {
  const status = timeSince(lastUpdate);

  if (status.endsWith("m")) {
    return "green"; // Mise à jour récente
  } else if (status.endsWith("h")) {
    const hours = parseInt(status.replace("h", ""));
    if (hours < 2) {
      return "gold"; // Mise à jour il y a moins de 2 heures
    } else {
      return "red"; // Mise à jour trop ancienne
    }
  } else if (status.endsWith("d")) {
    return "red"; // Mise à jour très ancienne (en jours)
  }
}

// Fonction pour obtenir la couleur de l'état (Running / Not running)
function getColorState(state) {
  if (state === "Running") {
    return "green"; // Système en fonctionnement
  } else {
    return "red"; // Système non fonctionnel
  }
}

// Fonction pour déterminer la qualité globale de l'eau
function getWaterQuality(latestEntry) {
  // Obtenons les couleurs des différentes données (sauf le temps et l'état)
  const colors = [
    getColorpH(latestEntry.pH),
    getColorTDS(latestEntry.TDS),
    getColorOx(latestEntry.Oxygen),
    getColorConductivity(latestEntry.Conductivity),
    getColorTemp(latestEntry.Temperature),
  ];

  // Log des couleurs pour voir si elles sont correctes
  console.log("Couleurs des paramètres:", colors);

  // Compter les occurrences de chaque couleur
  const colorCounts = { red: 0, gold: 0, green: 0 };

  // Incrémenter les compteurs correctement
  colors.forEach((color) => {
    if (colorCounts.hasOwnProperty(color)) {
      colorCounts[color]++;
    }
  });

  // Log du comptage des couleurs
  console.log("Comptage des couleurs:", colorCounts);

  // Déterminer la qualité globale de l'eau en fonction des couleurs
  if (
    colorCounts.red >= 2 ||
    getColorState(state(latestEntry.Time)) === "red"
  ) {
    console.log("Qualité de l'eau : Mauvaise");
    return "red"; // Eau de mauvaise qualité
  } else if (
    colorCounts.gold >= 2 ||
    getColorTime(latestEntry.Time) === "gold"
  ) {
    console.log("Qualité de l'eau : Modérée");
    return "gold"; // Eau de qualité modérée
  } else if (colorCounts.green >= 3) {
    console.log("Qualité de l'eau : Bonne");
    return "green"; // Eau de bonne qualité
  } else {
    console.log("Qualité de l'eau : Inconnue");
    return "unknow"; // Cas peu probable, mais au cas où
  }
}

function updateWaterQualityDisplay(latestEntry) {
  const waterQuality = getWaterQuality(latestEntry); // Récupère la qualité de l'eau
  console.log(waterQuality);
  // Récupère les éléments HTML où appliquer la classe
  const qualityContainer = document.getElementById("quality");
  const qualitySpan = document.getElementById("quality-span");

  // Appliquer les couleurs et classes en fonction de la qualité de l'eau
  if (waterQuality === "red") {
    qualityContainer.className = "quality-container red_border"; // Applique la classe "red" à quality-container
    qualitySpan.className = "red"; // Applique la classe "red" à quality-span
    qualitySpan.innerHTML = "MAUVAIS";
  } else if (waterQuality === "gold") {
    qualityContainer.className = "quality-container gold_border"; // Applique la classe "gold" à quality-container
    qualitySpan.className = "gold"; // Applique la classe "gold" à quality-span
    qualitySpan.innerHTML = "MOYEN";
  } else if (waterQuality === "green") {
    qualityContainer.className = "quality-container green_border"; // Applique la classe "green" à quality-container
    qualitySpan.className = "green"; // Applique la classe "green" à quality-span
    qualitySpan.innerHTML = "BON";
  } else {
    // Cas où la qualité de l'eau est indéterminée
    qualityContainer.className = "unknown"; // Optionnel, si vous avez une classe pour "Unknown"
    qualitySpan.className = "unknown"; // Optionnel, si vous avez une classe pour "Unknown"
  }
}

function fetchData() {
  /*const latestEntry = {pH: 7.2,TDS: 1,Oxygen: 3,Conductivity: 1200,Temperature: 1,Time: "2025-03-04 21:35:00"};*/
  fetch("value.php")
    .then((response) => {
      if (!response.ok) {
        throw new Error(`HTTP error! Status: ${response.status}`);
      }
      return response.json(); // Parse JSON data from the response
    })
    .then((data) => {
      if (data.length === 0) {
        console.error("No data found.");
        return;
      }

      const latestEntry = data[data.length - 1];
      const dataCards = document.getElementById("info-container");
      dataCards.innerHTML = ""; // Clear previous cards

      // Array containing the information to display in the cards
      const infoArray = [
        {
          label: "PH",
          value: `${latestEntry.pH}`,
          icon: "💧",
          color: getColorpH(latestEntry.pH),
        },
        {
          label: "TDS",
          value: `${latestEntry.TDS} ppm`,
          icon: "💎",
          color: getColorTDS(latestEntry.TDS),
        },
        {
          label: "Oxygen",
          value: `${latestEntry.Oxygen} mg/L`,
          icon: "🫧",
          color: getColorOx(latestEntry.Oxygen),
        },
        {
          label: "Conductivity",
          value: `${latestEntry.Conductivity} µS/cm`,
          icon: "⚡",
          color: getColorConductivity(latestEntry.Conductivity),
        },
        {
          label: "Temperature",
          value: `${latestEntry.Temperature} °C`,
          icon: "🌡️",
          color: getColorTemp(latestEntry.Temperature),
        },
        {
          label: "Last update",
          value: timeSince(latestEntry.Time),
          icon: "⏱️",
          color: getColorTime(latestEntry.Time),
        },
        {
          label: "Status",
          value: state(latestEntry.Time),
          icon: state(latestEntry.Time) === "Running" ? "✔️" : "❌",
          color: getColorState(state(latestEntry.Time)),
        },
      ];

      // Create a card for each piece of information
      infoArray.forEach((info) => {
        const card = `
            <div class="card ${info.color}">${info.icon} ${info.label} : ${info.value}</div>
          `;
        dataCards.innerHTML += card;
      });
      updateWaterQualityDisplay(latestEntry);
    })
    .catch((error) => {
      console.error("Fetch error:", error);
    });
}
