window.onload = function () {
  fetchData(); // Initial call to load data when the page loads
  setInterval(fetchData, 5000); // Refresh data every 5 seconds
};

function fetchData() {
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

      const latestEntry = data[data.length - 1]; // Get the last entry in the data
      const dataCards = document.getElementById("data-cards");
      dataCards.innerHTML = ""; // Clear previous cards

      // Array containing the information to display in the cards
      const infoArray = [
        { label: "PH", value: `${latestEntry.pH}`, icon: "💧" },
        { label: "TDS", value: `${latestEntry.TDS} ppm`, icon: "⚡" },
        { label: "Oxygen", value: `${latestEntry.Oxygen} mg/L`, icon: "🫧" },
        {
          label: "Conductivity",
          value: `${latestEntry.Conductivity} µS/cm`,
          icon: "⚡",
        },
        {
          label: "Temperature",
          value: `${latestEntry.Temperature} °C`,
          icon: "🌡️",
        },
        { label: "Last update", value: "30m", icon: "⏱️" },
        { label: "Status", value: "Running", icon: "✔️" },
      ];

      // Create a card for each piece of information
      infoArray.forEach((info) => {
        const card = `
          <div class="mb-4">
            <div class="card shadow-sm">
              <div class="card-body text-center">
                <h5 class="card-title">${info.icon} ${info.label}</h5>
                <p class="card-text">${info.value}</p>
              </div>
            </div>
          </div>
        `;
        dataCards.innerHTML += card;
      });
    })
    .catch((error) => {
      console.error("Error fetching data:", error);
    });
}
