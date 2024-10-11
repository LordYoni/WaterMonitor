function fetchData() {
  fetch("value.php")
    .then((response) => {
      if (!response.ok) {
        throw new Error(`HTTP error! Status: ${response.status}`);
      }
      return response.json(); // Parse the JSON data from the response
    })
    .then((data) => {
      const tableBody = document
        .getElementById("data-table")
        .getElementsByTagName("tbody")[0];
      tableBody.innerHTML = ""; // Clear the table body

      // Add data in the table
      data.forEach((row) => {
        const newRow = tableBody.insertRow();
        newRow.insertCell(0).innerText = row.id;
        newRow.insertCell(1).innerText = row.Time;
        newRow.insertCell(2).innerText = row.TDS;
        newRow.insertCell(3).innerText = row.pH;
        newRow.insertCell(4).innerText = row.Oxygen;
        newRow.insertCell(5).innerText = row.Conductivity;
        newRow.insertCell(6).innerText = row.Temperature;
      });
    })
    .catch((error) => {
      console.error("Error fetching data:", error);
    });
}

// Fetch data on page load
window.onload = fetchData;
