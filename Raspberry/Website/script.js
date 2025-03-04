window.onload = function () {
  fetchData();
  setInterval(fetchData, 5000);
};

// Calculate the time since the last update
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

// Calculate the state of the system
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

// Get the color for pH
function getColorpH(pH) {
  if (pH < 5.5) {
    return "red"; // Too acidic
  } else if (pH >= 5.5 && pH <= 7.0) {
    return "green"; // Normal range for a marsh
  } else {
    return "gold"; // Too alkaline
  }
}

// Get the color for TDS
function getColorTDS(TDS) {
  if (TDS < 100) {
    return "green"; // Low TDS (ideal for a marsh)
  } else if (TDS >= 100 && TDS <= 500) {
    return "gold"; // Moderate TDS
  } else {
    return "red"; // High TDS (indication of contamination or problem)
  }
}

// Get the color for Dissolved Oxygen
function getColorOx(Oxygen) {
  if (Oxygen < 4) {
    return "red"; // Low oxygen (problem)
  } else if (Oxygen >= 4 && Oxygen <= 7) {
    return "green"; // Normal oxygen level for a marsh
  } else {
    return "gold"; // Very high oxygen (may indicate excessive photosynthesis)
  }
}

// Get the color for Conductivity
function getColorConductivity(Conductivity) {
  if (Conductivity < 150) {
    return "green"; // Low conductivity (freshwater)
  } else if (Conductivity >= 150 && Conductivity <= 500) {
    return "gold"; // Moderate conductivity (slightly mineralized water)
  } else {
    return "red"; // High conductivity (salty water or contamination)
  }
}

// Get the color for Temperature
function getColorTemp(Temperature) {
  if (Temperature < 15) {
    return "green"; // Low temperature (cool)
  } else if (Temperature >= 15 && Temperature <= 25) {
    return "gold"; // Normal temperature (ideal for aquatic life)
  } else {
    return "red"; // High temperature (may be problematic for fauna/flora)
  }
}

// Get the color for the time since the last update
function getColorTime(lastUpdate) {
  const status = timeSince(lastUpdate);

  if (status.endsWith("m")) {
    return "green"; // Recent update
  } else if (status.endsWith("h")) {
    const hours = parseInt(status.replace("h", ""));
    if (hours < 2) {
      return "gold"; // Updated less than 2 hours ago
    } else {
      return "red"; // Update too old
    }
  } else if (status.endsWith("d")) {
    return "red"; // Very old update (in days)
  }
}

// Get the color for the state (Running / Not running)
function getColorState(state) {
  if (state === "Running") {
    return "green"; // System running
  } else {
    return "red"; // System not running
  }
}

// Determine the overall water quality
function getWaterQuality(latestEntry) {
  const colors = [
    getColorpH(latestEntry.pH),
    getColorTDS(latestEntry.TDS),
    getColorOx(latestEntry.Oxygen),
    getColorConductivity(latestEntry.Conductivity),
    getColorTemp(latestEntry.Temperature),
  ];

  console.log("Parameter colors:", colors);

  const colorCounts = { red: 0, gold: 0, green: 0 };

  colors.forEach((color) => {
    if (colorCounts.hasOwnProperty(color)) {
      colorCounts[color]++;
    }
  });

  console.log("Color counts:", colorCounts);

  if (
    colorCounts.red >= 2 ||
    getColorState(state(latestEntry.Time)) === "red"
  ) {
    console.log("Water quality: Poor");
    return "red"; // Poor water quality
  } else if (
    colorCounts.gold >= 2 ||
    getColorTime(latestEntry.Time) === "gold"
  ) {
    console.log("Water quality: Moderate");
    return "gold"; // Moderate water quality
  } else if (colorCounts.green >= 3) {
    console.log("Water quality: Good");
    return "green"; // Good water quality
  } else {
    console.log("Water quality: Unknown");
    return "unknown"; // Unlikely case, but just in case
  }
}

function updateWaterQualityDisplay(latestEntry) {
  const waterQuality = getWaterQuality(latestEntry);
  console.log(waterQuality);
  const qualityContainer = document.getElementById("quality");
  const qualitySpan = document.getElementById("quality-span");

  if (waterQuality === "red") {
    qualityContainer.className = "quality-container red_border";
    qualitySpan.className = "red";
    qualitySpan.innerHTML = "MAUVAIS";
  } else if (waterQuality === "gold") {
    qualityContainer.className = "quality-container gold_border";
    qualitySpan.className = "gold";
    qualitySpan.innerHTML = "MOYEN";
  } else if (waterQuality === "green") {
    qualityContainer.className = "quality-container green_border";
    qualitySpan.className = "green";
    qualitySpan.innerHTML = "BON";
  } else {
    qualityContainer.className = "unknown";
    qualitySpan.className = "unknown";
  }
}

function fetchData() {
  fetch("value.php")
    .then((response) => {
      if (!response.ok) {
        throw new Error(`HTTP error! Status: ${response.status}`);
      }
      return response.json();
    })
    .then((data) => {
      if (data.length === 0) {
        console.error("No data found.");
        return;
      }

      const latestEntry = data[data.length - 1];
      const dataCards = document.getElementById("info-container");
      dataCards.innerHTML = "";

      const infoArray = [
        {
          label: "pH",
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
          label: "Oxygène",
          value: `${latestEntry.Oxygen} mg/L`,
          icon: "🫧",
          color: getColorOx(latestEntry.Oxygen),
        },
        {
          label: "Conductivité",
          value: `${latestEntry.Conductivity} µS/cm`,
          icon: "⚡",
          color: getColorConductivity(latestEntry.Conductivity),
        },
        {
          label: "Température",
          value: `${latestEntry.Temperature} °C`,
          icon: "🌡️",
          color: getColorTemp(latestEntry.Temperature),
        },
        {
          label: "Actualisation",
          value: timeSince(latestEntry.Time),
          icon: "⏱️",
          color: getColorTime(latestEntry.Time),
        },
        {
          label: "État",
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
