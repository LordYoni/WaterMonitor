#! /usr/bin/bash

echo "THIS FILE IS NOT MEANT TO BE EXECUTED!"
exit

# All commands to execute on the host raspberry to setup the server.

sudo apt update
sudo apt upgrade
sudo apt update
sudo apt install apache2
sudo chown -R user_name /var/www/html

sudo apt install mariadb-server

sudo apt install php

sudo apt install phpmyadmin

# "apache2"
# "yes"
#

sudo mysql -u root -p

# "GRANT ALL PRIVILEGES ON *.* TO 'usrSQL'@'localhost' IDENTIFIED BY 'pswdSQL' WITH GRANT OPTION;"
# "quit"

sudo nano /etc/apache2/apache2.conf

# write at the end "Include /etc/phpmyadmin/apache.conf"

sudo service apache2 restart

sudo apt-get install python3-pip

python3 -m venv venv
source venv/bin/activate
pip install pyserial
pip install mysql-connector-python

