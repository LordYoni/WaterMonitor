#! /usr/bin/bash

# Variables (customize these)
DB_USER="usrSQL"
DB_PASSWORD="pswdSQL"
WEB_USER="user_name"

echo "Running the server installation script..."

# Update the system
sudo apt update && sudo apt upgrade -y

# Install Apache
sudo apt install apache2 -y
sudo chown -R $WEB_USER /var/www/html

# Install MariaDB
sudo apt install mariadb-server -y

# Install PHP and phpMyAdmin
sudo apt install php phpmyadmin -y

# Configure MariaDB
sudo mysql -u root <<EOF
GRANT ALL PRIVILEGES ON *.* TO '$DB_USER'@'localhost' IDENTIFIED BY '$DB_PASSWORD' WITH GRANT OPTION;
FLUSH PRIVILEGES;
QUIT;
EOF

# Configure Apache to include phpMyAdmin
echo "Include /etc/phpmyadmin/apache.conf" | sudo tee -a /etc/apache2/apache2.conf

# Restart Apache
sudo service apache2 restart

# Install pip and pyserial
sudo apt-get install python3-pip -y

# Create a Python virtual environment and install dependencies
python3 -m venv ~/venv
source venv/bin/activate
pip install pyserial mysql-connector-python

echo "Installation completed!"
