#! /usr/bin/bash

# Function to install a package if not already installed
install_if_not_exists() {
    # Check if the package is installed
    if ! dpkg -l | grep -q "^ii  $1 "; then  
        echo "Installing $1..."              # Notify about installation
        sudo apt install "$1" -y              # Install the package
    else
        echo "$1 is already installed."        # Notify if already installed
    fi
}

set -e  # Exit immediately if any command fails

# Automatically get the current user
WEB_USER=$(whoami)  

# Display starting message 
echo "
Starting...
"

sleep 2  # Pause for 2 seconds

# Display the automatically set WEB_USER
echo "Using the current user for the web server (WEB_USER): $WEB_USER"  

# Update the system packages
echo "Updating the system..."  

# Configure pending package configurations
sudo dpkg --configure -a  

# Update package list and upgrade installed packages
sudo apt update && sudo apt upgrade -y  

# Install Git
install_if_not_exists git  

# Configure a swap file if the memory is limited
echo "Configuring the swap file..."  

# Setup swap file
sudo dphys-swapfile setup  

# Enable swap file service
sudo systemctl enable dphys-swapfile  

# Start swap file service
sudo systemctl start dphys-swapfile  

# Install Apache
install_if_not_exists apache2  

# Change ownership of the web directory to the current user
echo "Changing ownership of the web directory to $WEB_USER"  
sudo chown -R "$WEB_USER" /var/www/html  

# Install MariaDB
install_if_not_exists mariadb-server  

# Install PHP 
install_if_not_exists php  

# File to store user information
USER_INFO_FILE="$HOME/.db_user_info"  

# Check if user information file exists
if [ -f "$USER_INFO_FILE" ]; then  
    source "$USER_INFO_FILE"  # Load user info
else
    # Prompt for DB username
    while true; do
        echo "Please enter the username you want for the MariaDB database (DB_USER):"
        read DB_USER

        # Check if username is empty
        if [[ -z "$DB_USER" ]]; then  
            echo "Error: User cannot be empty. Please try again."
        else
            break
        fi
    done

    # Prompt for DB password
    while true; do
        echo "Please enter the password you want for the MariaDB database (DB_PASSWORD):"
        read -s DB_PASSWORD  

        # Check if password is empty
        if [[ -z "$DB_PASSWORD" ]]; then  
            echo "Error: Password cannot be empty. Please try again."
        else
            break
        fi
    done

    # Prompt for DB name
    while true; do
        echo "Please enter the name of the database you want to create (DB_NAME):"
        read DB_NAME

        # Check if database name is empty
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

# Configure phpMyAdmin installation
echo "Configuring phpMyAdmin installation..."  
echo "phpmyadmin phpmyadmin/dbconfig-install boolean true" | sudo debconf-set-selections
echo "phpmyadmin phpmyadmin/mysql/admin-pass password $DB_PASSWORD " | sudo debconf-set-selections
echo "phpmyadmin phpmyadmin/mysql/app-pass password $DB_PASSWORD" | sudo debconf-set-selections
echo "phpmyadmin phpmyadmin/reconfigure-webserver multiselect apache2" | sudo debconf-set-selections

# Install phpMyAdmin
install_if_not_exists phpmyadmin  

# Create the config.php file for database connection
CONFIG_FILE="/var/www/html/config.php"
echo "<?php" | sudo tee $CONFIG_FILE
echo "define('DB_HOST', 'localhost');" | sudo tee -a $CONFIG_FILE
echo "define('DB_USER', '$DB_USER');" | sudo tee -a $CONFIG_FILE
echo "define('DB_PASS', '$DB_PASSWORD');" | sudo tee -a $CONFIG_FILE
echo "define('DB_NAME', '$DB_NAME');" | sudo tee -a $CONFIG_FILE

# Change file ownership and set permissions
sudo chown www-data:www-data /var/www/html/config.php  
sudo chmod 600 /var/www/html/config.php  

# Configure MariaDB
echo "Configuring MariaDB..."  
sudo mysql -u root <<EOF
GRANT ALL PRIVILEGES ON *.* TO '$DB_USER'@'localhost' IDENTIFIED BY '$DB_PASSWORD' WITH GRANT OPTION;  # Grant privileges
FLUSH PRIVILEGES;  # Flush privileges
GRANT INSERT ON *.* TO 'dbaccess'@'localhost' IDENTIFIED BY 'accessPASS';  # Grant access to dbaccess user
FLUSH PRIVILEGES;  # Flush privileges again

-- Create the database and table
CREATE DATABASE $DB_NAME;  # Create the database
USE $DB_NAME;  # Use the database
CREATE TABLE Data (  # Create a table named Data
    id INT AUTO_INCREMENT PRIMARY KEY,
    Time TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    TDS FLOAT,
    pH FLOAT,
    Oxygen FLOAT,
    Conductivity FLOAT,
    Temperature FLOAT
);
EOF

# Download random value SQL script
curl -o test_values.sql https://raw.githubusercontent.com/LordYoni/WaterMonitor/refs/heads/main/Raspberry/test_values.sql  

# Check if phpMyAdmin configuration is already included in Apache
if ! grep -q "Include /etc/phpmyadmin/apache.conf" /etc/apache2/apache2.conf; then  
    echo "Including phpMyAdmin in Apache configuration..."  
    echo "Include /etc/phpmyadmin/apache.conf" | sudo tee -a /etc/apache2/apache2.conf  
fi

# Restart Apache to apply changes
echo "Restarting Apache..."  
sudo service apache2 restart  

# Install website files
curl -o /var/www/html/index.html https://raw.githubusercontent.com/LordYoni/WaterMonitor/refs/heads/main/Raspberry/Website/index.html  
curl -o /var/www/html/script.js https://raw.githubusercontent.com/LordYoni/WaterMonitor/refs/heads/main/Raspberry/Website/script.js  
curl -o /var/www/html/style.css https://raw.githubusercontent.com/LordYoni/WaterMonitor/refs/heads/main/Raspberry/Website/style.css  
curl -o /var/www/html/value.php https://raw.githubusercontent.com/LordYoni/WaterMonitor/refs/heads/main/Raspberry/Website/value.php  
curl -o /var/www/html/assets/logo_fav.png https://raw.githubusercontent.com/LordYoni/WaterMonitor/refs/heads/main/Raspberry/Website/assets/logo_fav.png  
curl -o /var/www/html/assets/logo.jpg https://raw.githubusercontent.com/LordYoni/WaterMonitor/refs/heads/main/Raspberry/Website/assets/logo.jpg  
curl -o /var/www/html/assets/Parc_isle_fond.webp https://raw.githubusercontent.com/LordYoni/WaterMonitor/refs/heads/main/Raspberry/Website/assets/Parc_isle_fond.webp

# Change ownership and set permissions for website files
sudo chown www-data:www-data /var/www/html/index.html /var/www/html/script.js /var/www/html/style.css /var/www/html/value.php  /var/www/html/assets/logo_fav.png /var/www/html/assets/logo.jpg /var/www/html/assets/Parc_isle_fond.webp 
sudo chmod 644 /var/www/html/index.html /var/www/html/script.js /var/www/html/style.css /var/www/html/value.php  /var/www/html/assets/logo_fav.png /var/www/html/assets/logo.jpg /var/www/html/assets/Parc_isle_fond.webp 

# Install pip for Python 3
install_if_not_exists python3-pip  

# Create a Python virtual environment and install dependencies
echo "Creating Python virtual environment and installing dependencies..."  
python3 -m venv $HOME/venv  # Create virtual environment
source $HOME/venv/bin/activate  # Activate virtual environment
pip install pyserial mysql-connector-python python-dotenv  # Install dependencies

# Download run.py to ~/Scripts
echo "Downloading run.py..."  

if [ ! -d "$HOME/Scripts" ]; then  
    mkdir -p "$HOME/Scripts"  # Create Scripts directory if it doesn't exist
fi

# Download the run.py script
curl -o $HOME/Scripts/run.py https://raw.githubusercontent.com/LordYoni/WaterMonitor/refs/heads/main/Raspberry/run.py  

# Create .env file for database credentials
ENV_FILE="$HOME/Scripts/.env"  
echo "DB_USER=dbaccess" > "$ENV_FILE"  
echo "DB_PASSWORD=accessPASS" >> "$ENV_FILE"  
echo "DB_NAME=$DB_NAME" >> "$ENV_FILE"  
echo "DB_HOST=localhost" >> "$ENV_FILE"  
echo "DB_PORT=3306" >> "$ENV_FILE"  

# Change ownership and set permissions for the .env file
sudo chown "$WEB_USER":"$WEB_USER" "$HOME/Scripts/.env"  
sudo chmod 600 "$HOME/Scripts/.env"  

# Download Fastfetch
echo "Downloading Fastfetch..."  
wget https://github.com/fastfetch-cli/fastfetch/releases/download/2.27.1/fastfetch-linux-aarch64.deb  
sudo dpkg -i fastfetch-linux-aarch64.deb  # Install Fastfetch
rm fastfetch-linux-aarch64.deb  # Remove downloaded package

# Add custom aliases to .bashrc
echo "alias venvlaunch='source venv/bin/activate'" >> $HOME/.bashrc  
echo "alias c='clear && fastfetch'" >> $HOME/.bashrc  
echo "alias shutdown='sudo shutdown now'" >> $HOME/.bashrc  
echo "alias reboot='sudo reboot'" >> $HOME/.bashrc  
echo "alias DBtest='mysql -u $DB_USER -p$DB_PASSWORD $DB_NAME < test_values.sql'" >> $HOME/.bashrc  
echo "alias pystart='python3 ~/Scripts/run.py'" >> $HOME/.bashrc  

# Completion message
echo "
All done!
Just use source .bashrc to apply the changes to the current terminal session.
"
