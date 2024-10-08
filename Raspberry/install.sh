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

# Display ASCII art header
echo "
 ________  _________  ________  ________  _________  ___  ________   ________
|\   ____\|\___   ___\\   __  \|\   __  \|\___   ___\\  \|\   ___  \|\   ____\
\ \  \___|\|___ \  \_\ \  \|\  \ \  \|\  \|___ \  \_\ \  \ \  \\ \  \ \  \___|
 \ \_____  \   \ \  \ \ \   __  \ \   _  _\   \ \  \ \ \  \ \  \\ \  \ \  \  ___
  \|____|\  \   \ \  \ \ \  \ \  \ \  \\  \|   \ \  \ \ \  \ \  \\ \  \ \  \|\  \ ___ ___ ___
    ____\_\  \   \ \__\ \ \__\ \__\ \__\\ _\    \ \__\ \ \__\ \__\\ \__\ \_______\\__\\__\\__\
   |\_________\   \|__|  \|__|\|__|\|__|\|__|    \|__|  \|__|\|__| \|__|\|_______\|__\|__\|__|
   \|_________|
"

sleep 2

# Display the automatically set WEB_USER
echo "Using the current user for the web server (WEB_USER): $WEB_USER"

# Update the system
echo "Updating the system..."
sudo apt update && sudo apt upgrade -y

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

# Configure MariaDB
echo "Configuring MariaDB..."
sudo mysql -u root <<EOF
GRANT ALL PRIVILEGES ON *.* TO '$DB_USER'@'localhost' IDENTIFIED BY '$DB_PASSWORD' WITH GRANT OPTION;
FLUSH PRIVILEGES;
QUIT;
EOF

# Check if phpMyAdmin configuration is already included in Apache
if ! grep -q "Include /etc/phpmyadmin/apache.conf" /etc/apache2/apache2.conf; then
    echo "Including phpMyAdmin in Apache configuration..."
    echo "Include /etc/phpmyadmin/apache.conf" | sudo tee -a /etc/apache2/apache2.conf
fi

# Restart Apache to apply changes
echo "Restarting Apache..."
sudo service apache2 restart

# Install pip for Python 3
install_if_not_exists python3-pip

# Create a Python virtual environment and install dependencies
echo "Creating Python virtual environment and installing dependencies..."
python3 -m venv $HOME/venv
source $HOME/venv/bin/activate
pip install pyserial mysql-connector-python

# Download run.py to ~/Scripts
echo "Downloading run.py..."

if [ ! -d "$HOME/Scripts" ]; then
    mkdir -p "$HOME/Scripts"
fi
cd "$HOME/Scripts"
curl -o run.py https://raw.githubusercontent.com/LordYoni/WaterMonitor/refs/heads/main/Raspberry/run.py


# Clear the terminal screen
clear

# Display completion message with ASCII art
echo "
 ________  ________  _____ ______   ________  ___       _______  _________  _______   ________  ___  ___  ___
|\   ____\|\   __  \|\   _ \  _   \|\   __  \|\  \     |\  ___ \|\___   ___\\  ___ \ |\   ___ \|\  \|\  \|\  \
\ \  \___|\ \  \|\  \ \  \\\__\ \  \ \  \|\  \ \  \    \ \   __/\|___ \  \_\ \   __/|\ \  \_|\ \ \  \ \  \ \  \
 \ \  \    \ \  \\\  \ \  \\|__| \  \ \   ____\ \  \    \ \  \_|/__  \ \  \ \ \  \_|/_\ \  \ \\ \ \  \ \  \ \  \
  \ \  \____\ \  \\\  \ \  \    \ \  \ \  \___|\ \  \____\ \  \_|\ \  \ \  \ \ \  \_|\ \ \  \_\\ \ \__\ \__\ \__\
   \ \_______\ \_______\ \__\    \ \__\ \__\    \ \_______\ \_______\  \ \__\ \ \_______\ \_______\|__|\|__|\|__|
    \|_______|\|_______|\|__|     \|__|\|__|     \|_______|\|_______|   \|__|  \|_______|\|_______|   ___  ___  ___
                                                                                                     |\__\|\__\|\__\
                                                                                                     \|__|\|__|\|__|
"


