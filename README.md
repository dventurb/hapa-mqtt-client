<div align ="center">
    <!--LOGO-->
    <a href="github.com/dventurb/hapa-mqtt-client">
    <img src="https://github.com/dventurb/hapa-mqtt-client/blob/main/icons/apps/64/hapa-app-icon.png" alt="Logo" width="64" height="64">
</a>

<!--PROJECT NAME-->
<h1>HAPA MQTT Client</h1>

<!--DESCRIPTION-->
<p align="center">
    Desktop MQTT Client in C using GTK, developed for Hapa IoT in internship.
</p>

<!--BADGES-->
<p>
     <img src="https://img.shields.io/badge/platform-linux-lightgrey" />
     <img src="https://img.shields.io/badge/GTK-4.0-blue" />
</p>

<h3>
    <a href="#about-project">About Project</a>
    <span . </span>
    <a href="#installation">Installation</a>
    <span .</span>
    <a href="#dependencies">Dependencies</a>
    <span .</span>
    <a href="#features">Features</a>
    <span .</span>
    <a href="#project-layout">Project Layout</a>
</h3>
</div>

![screenshot](https://i.imgur.com/Fqh3lva.gif)


## About Project
> _HAPA MQTT Client is a lightweight tool for mananing MQTT connections, topics, and payloads. This project was developed as part of internship at HAPA IoT, with aim of improving skills in MQTT protocol and GTK development using C. 
> _The design and some features are inspired by some MQTT clients like  [MQTT Explorer] (https://github.com/thomasnordquist/MQTT-Explorer) and [MQTTX] (https://github.com/emqx/MQTTX).

## Installation 

```bash 
    git clone https://github.com/dventurb/hapa-mqtt-client.git
    cd hapa-mqtt-client
    chmod +x install.sh 
    ./install.sh
```

## Dependencies
    All dependencies are automatically installed by the install script (instal.sh).
        - **GTK 4** (`libgtk-4-dev`) 
        - **Mosquitto** (`libmosquitto-dev`)
        - **JSON-C** (`libjson-c-dev`)
        - **Build tools**: `gcc`, `make`, `pkg-config`

## Features 
    - Modern UI with GTK 4 
    - Manage multiple MQTT connections 
    - Payload viewer in real-time 
    - Inspired by [MQTT Explorer] (https://github.com/thomasnordquist/MQTT-Explorer) and [MQTTX] (https://github.com/emqx/MQTTX) 

## Project Layout 
    hapa-mqtt-client/
    ├── src/            application source code 
    ├── inc/            header files 
    ├── config/         configuration files (e.g. settings.json)
    ├── icons/          application icons 
    ├── style/          css theme
    └── TODO.md         future tasks and ideas

