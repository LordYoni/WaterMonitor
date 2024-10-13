#! /usr/bin/bash

# Function to install a package if not already installed
install_if_not_exists() {
    if ! dpkg -l | grep -q "^ii  $1 "; then
        echo "Installing $1..."
        sudo apt install "$1" -y
    else
        echo "$1 is already installed."
    fi
}

# Exit immediately if any command fails
set -e

# Automatically get the current user
WEB_USER=$(whoami)

# Display Starting message 
echo "
Starting...
"

sleep 2

# Display the automatically set WEB_USER
echo "Using the current user for the web server (WEB_USER): $WEB_USER"

# Update the system
echo "Updating the system..."
sudo dpkg --configure -a
sudo apt update && sudo apt upgrade -y

#Install Git
install_if_not_exists git -y

# Configure a swap file if the memory is limited
echo "Configuring the swap file..."
sudo dphys-swapfile setup
sudo systemctl enable dphys-swapfile
sudo systemctl start dphys-swapfile

# Install Apache
install_if_not_exists apache2
# Change ownership of the web directory to the current user
echo "Changing ownership of the web directory to $WEB_USER"
sudo chown -R "$WEB_USER" /var/www/html

# Install MariaDB
install_if_not_exists mariadb-server

# Install PHP and phpMyAdmin
install_if_not_exists php
install_if_not_exists phpmyadmin

# File to store user information
USER_INFO_FILE="$HOME/.db_user_info"

# Check if user information file exists
if [ -f "$USER_INFO_FILE" ]; then
    source "$USER_INFO_FILE"
else
    # Prompt the user for necessary information for the database
    while true; do
        echo "Please enter the username you want for the MariaDB database (DB_USER):"
        read DB_USER

        if [[ -z "$DB_USER" ]]; then
            echo "Error: User cannot be empty. Please try again."
        else
            break
        fi
    done

    # Prompt for the database password securely
    while true; do
        echo "Please enter the password you want for the MariaDB database (DB_PASSWORD):"
        read -s DB_PASSWORD  

        # Check if password was provided
        if [[ -z "$DB_PASSWORD" ]]; then
            echo "Error: Password cannot be empty. Please try again."
        else
            break
        fi
    done

    # Prompt for the database name
    while true; do
        echo "Please enter the name of the database you want to create (DB_NAME):"
        read DB_NAME

        if [[ -z "$DB_NAME" ]]; then
            echo "Error: Database name cannot be empty. Please try again."
        else
            break
        fi
    done

    # Save user information to file
    echo "DB_USER=$DB_USER" > "$USER_INFO_FILE"
    echo "DB_PASSWORD=$DB_PASSWORD" >> "$USER_INFO_FILE"
    echo "DB_NAME=$DB_NAME" >> "$USER_INFO_FILE"
fi

# Create the config.php file
CONFIG_FILE="/var/www/html/config.php"
echo "<?php" | sudo tee $CONFIG_FILE
echo "define('DB_HOST', 'localhost');" | sudo tee -a $CONFIG_FILE
echo "define('DB_USER', '$DB_USER');" | sudo tee -a $CONFIG_FILE
echo "define('DB_PASS', '$DB_PASSWORD');" | sudo tee -a $CONFIG_FILE
echo "define('DB_NAME', '$DB_NAME');" | sudo tee -a $CONFIG_FILE

sudo chown www-data:www-data /var/www/html/config.php
sudo chmod 600 /var/www/html/config.php


# Configure MariaDB
echo "Configuring MariaDB..."
sudo mysql -u root <<EOF
GRANT ALL PRIVILEGES ON *.* TO '$DB_USER'@'localhost' IDENTIFIED BY '$DB_PASSWORD' WITH GRANT OPTION;
FLUSH PRIVILEGES;

-- Create the database and table

CREATE DATABASE $DB_NAME;
USE $DB_NAME;
CREATE TABLE Data (
    id INT AUTO_INCREMENT PRIMARY KEY,
    Time TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    TDS FLOAT,
    pH FLOAT,
    Oxygen FLOAT,
    Conductivity FLOAT,
    Temperature FLOAT
);
EOF

#Install Random Value SQL Script
curl -o test_values.sql https://raw.githubusercontent.com/LordYoni/WaterMonitor/refs/heads/main/Raspberry/test_values.sql

# Check if phpMyAdmin configuration is already included in Apache
if ! grep -q "Include /etc/phpmyadmin/apache.conf" /etc/apache2/apache2.conf; then
    echo "Including phpMyAdmin in Apache configuration..."
    echo "Include /etc/phpmyadmin/apache.conf" | sudo tee -a /etc/apache2/apache2.conf
fi

# Restart Apache to apply changes
echo "Restarting Apache..."
sudo service apache2 restart

#Install WebSite
curl -o /var/www/html/index.html https://raw.githubusercontent.com/LordYoni/WaterMonitor/refs/heads/main/Raspberry/WebSite/index.html
curl -o /var/www/html/script.js https://raw.githubusercontent.com/LordYoni/WaterMonitor/refs/heads/main/Raspberry/WebSite/script.js
curl -o /var/www/html/style.css https://raw.githubusercontent.com/LordYoni/WaterMonitor/refs/heads/main/Raspberry/WebSite/style.css
curl -o /var/www/html/value.php https://raw.githubusercontent.com/LordYoni/WaterMonitor/refs/heads/main/Raspberry/WebSite/value.php

sudo chown www-data:www-data /var/www/html/index.html /var/www/html/script.js /var/www/html/style.css /var/www/html/value.php
sudo chmod 644 /var/www/html/index.html /var/www/html/script.js /var/www/html/style.css /var/www/html/value.php

# Install pip for Python 3
install_if_not_exists python3-pip

# Create a Python virtual environment and install dependencies
echo "Creating Python virtual environment and installing dependencies..."
python3 -m venv $HOME/venv
source $HOME/venv/bin/activate
pip install pyserial mysql-connector-python python-dotenv

# Download run.py to ~/Scripts
echo "Downloading run.py..."

if [ ! -d "$HOME/Scripts" ]; then
    mkdir -p "$HOME/Scripts"
fi
curl -o $HOME/Scripts/run.py https://raw.githubusercontent.com/LordYoni/WaterMonitor/refs/heads/main/Raspberry/run.py

ENV_FILE="$HOME/Scripts/.env

# Ajouter les informations de la base de données dans le fichier .env
echo "DB_USER=dbaccess" > "$ENV_FILE"
echo "DB_PASSWORD=" >> "$ENV_FILE"
echo "DB_NAME=$DB_NAME" >> "$ENV_FILE"
echo "DB_HOST=localhost" >> "$ENV_FILE"
echo "DB_PORT=3306" >> "$ENV_FILE"

# Download Fastfetch
echo "Downloading Fastfetch..."
wget https://github.com/fastfetch-cli/fastfetch/releases/download/2.27.1/fastfetch-linux-aarch64.deb
sudo dpkg -i fastfetch-linux-aarch64.deb 
rm fastfetch-linux-aarch64.deb

#Custom .bashrc
echo "alias venvlaunch='source venv/bin/activate'" >> $HOME/.bashrc
echo "alias c='clear && fastfetch'" >> $HOME/.bashrc
echo "alias shutdown='sudo shutdown now'" >> $HOME/.bashrc
echo "alias reboot='sudo reboot'" >> $HOME/.bashrc
echo "alias DBtest='mysql -u $DB_USER -p$DB_PASSWORD $DB_NAME < test_values.sql'" >> $HOME/.bashrc
echo "alias neofetch='fastfetch'" >> $HOME/.bashrc
echo "alias webfile='cd /var/www/html'" >> $HOME/.bashrc
echo "alias webupdate='sudo curl -o /var/www/html/index.html https://raw.githubusercontent.com/LordYoni/WaterMonitor/refs/heads/main/Raspberry/WebSite/index.html && sudo curl -o /var/www/html/script.js https://raw.githubusercontent.com/LordYoni/WaterMonitor/refs/heads/main/Raspberry/WebSite/script.js && sudo curl -o /var/www/html/style.css https://raw.githubusercontent.com/LordYoni/WaterMonitor/refs/heads/main/Raspberry/WebSite/style.css && sudo curl -o /var/www/html/value.php https://raw.githubusercontent.com/LordYoni/WaterMonitor/refs/heads/main/Raspberry/WebSite/value.php'" >> $HOME/.bashrc

echo "fastfetch" >> $HOME/.bashrc
source .bashrc

# Clear the terminal screen
clear

# Display completion message 
echo "
Completed !!!
"
sleep 2

# Display fastfetch
fastfetch
