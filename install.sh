#!/bin/bash

# Debian/Ubuntu
install_with_apt() {
  sudo apt update
  sudo apt install -y \
    build-essential \
    pkg-config \
    libmosquitto-dev \
    libjson-c-dev \
    libgtk-4-dev
}

# Red Hat/CentOS
install_with_dnf() {
  sudo dnf install -y \
    gcc \
    make \
    pkgconf-pkg-config \
    json-c-devel \
    mosquitto-devel \
    gtk4-devel
}

install_with_pacman() {
  sudo pacman -Sy --noconfirm \
    base-devel \
    pkgconf \
    json-c \
    mosquitto \
    gtk4
}

if [ -f /etc/arch-release ]; then
  install_with_pacman
elif [ -f /etc/debian_version ]; then
  install_with_apt
elif [ -f /etc/redhat-release ] || [ -f /etc/centos-release ]; then
  install_with_dnf
else 
  echo "Your linux distro is not supported currently."
  echo "You need to manually install those packages: gcc, make, pkg-config, json-c, mosquitto, gtk4"
  exit 1
fi

mkdir -p ~/.local/share/hapa-mqtt-client/ 
sudo mkdir -p /usr/share/icons/hapa-mqtt-client
sudo cp style/style.css /usr/share/hapa-mqtt-client/

make -j$(nproc) && sudo make install

cp -rf config/* ~/.local/share/hapa-mqtt-client/
sudo cp -rf icons/* /usr/share/icons/hapa-mqtt-client/

sudo cp hapa-mqtt-client.desktop /usr/share/applications/ 
sudo cp hapa-mqtt-client /usr/bin/

sudo update-desktop-database /usr/share/applications/

echo "====================="
echo "INSTALLATION FINISHED"
echo "====================="
