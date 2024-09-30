#! /usr/bin/bash

# Variables (à personnaliser)
DB_USER="usrSQL"
DB_PASSWORD="pswdSQL"
WEB_USER="user_name"

# Mettre à jour le système
sudo apt update && sudo apt upgrade -y

# Installer Apache
sudo apt install apache2 -y
sudo chown -R $WEB_USER /var/www/html

# Installer MariaDB
sudo apt install mariadb-server -y

# Installer PHP et phpMyAdmin
sudo apt install php phpmyadmin -y

# Configurer MariaDB
sudo mysql -u root <<EOF
GRANT ALL PRIVILEGES ON *.* TO '$DB_USER'@'localhost' IDENTIFIED BY '$DB_PASSWORD' WITH GRANT OPTION;
FLUSH PRIVILEGES;
QUIT;
EOF

# Configurer Apache pour inclure phpMyAdmin
echo "Include /etc/phpmyadmin/apache.conf" | sudo tee -a /etc/apache2/apache2.conf

# Redémarrer Apache
sudo service apache2 restart

# Installer pip et pyserial
sudo apt-get install python3-pip -y

# Créer un environnement virtuel Python et installer les dépendances
python3 -m venv venv
source venv/bin/activate
pip install pyserial mysql-connector-python

echo "Installation terminée !"
