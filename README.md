# ProjetToiture
  Un projet de EISE4 Polytech Sorbonne, qui est basé sur une carte Nucleo l432KC.  
  Ce projet permet de mesurer la température et humidité de l'air et de sol, ainsi la couleur et l'intensité de la lumière.  
  Les capteurs utilisés sont:  
   - Un DHT22 pour la température et l'humidité de l'air  
   - Une sonde DS18B20 pour la température du sol  
   - Un capteur d'humidité grove pour l'humidité du sol  
   - Un ADA1334 pour la couleur lumineuse  
   - Un ADA1384 pour l'intensité lumineuse    
   
  Les donées seront envoyées par un module Sigfox et tranmises vers la plateform Ubidots pour les visualiser.  
  Voici le shéma de câblage:  
  ![image](https://github.com/taqinlaodedie/ProjetToiture/blob/master/schema_final.png)
