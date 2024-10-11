function fetchData() {
  fetch("value.php")
    .then((response) => {
      if (!response.ok) {
        throw new Error(`HTTP error! Status: ${response.status}`);
      }
      return response.json();
    })
    .then((data) => {
      const dataCards = document.getElementById("data-cards");
      dataCards.innerHTML = "";
      data.forEach((row) => {
        const card = `
          <div class="col-sm-6 col-md-4 col-lg-3 mb-4">
            <div class="card shadow-sm">
              <div class="card-body">
                <h5 class="card-title">ID: ${row.id}</h5>
                <p class="card-text">
                  <strong>Time:</strong> ${row.Time} <br />
                  <strong>TDS:</strong> ${row.TDS} ppm <br />
                  <strong>pH:</strong> ${row.pH} <br />
                  <strong>Oxygen:</strong> ${row.Oxygen} mg/L <br />
                  <strong>Conductivity:</strong> ${row.Conductivity} µS/cm <br />
                  <strong>Temp:</strong> ${row.Temperature} °C
                </p>
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

window.onload = function () {
  fetchData();
  setInterval(fetchData, 5000);
};
