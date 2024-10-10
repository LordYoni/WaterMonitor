<?php
require 'config.php';

// Create a new mysqli instance
$mysqli = new mysqli(DB_HOST, DB_USER, DB_PASS, DB_NAME);

// Check for a connection error
if ($mysqli->connect_error) {
    die("Connection failed: " . $mysqli->connect_error);
}

// Execute the query to select all entries from the Data table
$dataQuery = $mysqli->query("SELECT * FROM Data ORDER BY id DESC");

// Check if the query was successful
if (!$dataQuery) {
    die("Query failed: " . $mysqli->error);
}

// Initialize an array to store the results
$data = [];

// Fetch all rows and add them to the data array
while ($row = $dataQuery->fetch_assoc()) {
    $data[] = $row;
}

// Set the content type to JSON
header('Content-Type: application/json');

// Encode the data array as JSON and output it
echo json_encode($data);

// Close the database connection
$mysqli->close();
