<div align="center">

# Sim Racing Wheel

**Volant de simulation type F1, conçu de zéro : modélisation 3D, PCB custom, firmware Arduino.**

Fusion 360 · PCB 2 couches · Arduino Nano · 18 LEDs RGB · écran OLED · palettes à charnière

<img src="Picture/FrontView_1.png" width="90%" />

</div>

> **🚧 Work in progress** — la mécanique et l'électronique sont finalisées, le firmware est encore à consolider.

---

## Description

Volant de simulation automobile inspiré des monoplaces de F1, entièrement conçu et fabriqué maison. La coque est modélisée dans Fusion 360 et imprimée en 3D, l'électronique repose sur un PCB deux couches dessiné pour l'occasion, et l'ensemble se connecte en USB-C.

Le volant intègre une barre de 18 LEDs RGB adressables faisant office de compte-tours, un écran OLED central pour la télémétrie, deux encodeurs rotatifs, des boutons poussoirs, et deux palettes de changement de vitesse montées sur charnières.

## Conception 3D

Modélisé sous Fusion 360 en corps séparés : coque principale, face avant, supports de palettes, charnières et palettes gauche/droite.

| Vue de face | Vue trois-quarts |
|:---:|:---:|
| <img src="Picture/FrontSideView_1.png" width="440"/> | <img src="Picture/BackSideView_1.png" width="440"/> |

**Face arrière** — nervures de rigidification, logements de vis et passages de câbles vers les palettes.

<div align="center">
<img src="Picture/InsideView_1.png" width="88%" />
</div>

**Dessin technique coté**

<div align="center">
<img src="Picture/dessin-technique-volant.png" width="88%" />
</div>

---

## Électronique

PCB deux couches dessiné sous EasyEDA, fichiers Gerber et pick-and-place prêts pour la production.

| Réf. | Qté | Composant | Rôle |
|---|:---:|---|---|
| U1 | 1 | Arduino Nano | Contrôleur, interface USB HID |
| LED1-18 | 18 | WS2812B-V5/W | Barre de compte-tours RGB adressable |
| OLED1 | 1 | HS13L03W2C01 | Affichage télémétrie |
| SW1-2 | 2 | ALPS EC11E09244BS | Encodeurs rotatifs cliquables |
| SWITCH1-2 | 2 | K6-6275D03 | Boutons poussoirs |
| USBC1 | 1 | MC-311D | Connecteur USB-C |
| C1-18 | 18 | 100 nF | Découplage, un par LED |
| R1 | 1 | 470 Ω | Résistance de ligne de données WS2812B |
| R2-5 | 4 | 220 Ω | Limitation de courant |

Nomenclature complète, empreintes et références LCSC dans `PCB/BOM_Board1_PCB1_2024-10-15.xlsx`.

Le découplage à 100 nF par LED et la résistance série de 470 Ω sur la ligne de données suivent les recommandations du fabricant WS2812B — sans elles, une chaîne de 18 LEDs devient instable.

---

## Structure du projet

```
├── 3D/
│   ├── steering-wheel v12.stl / .3mf   # Modèle courant
│   ├── Plan steerign wheel.pdf         # Plan coté
│   └── Previous/                       # Itérations v11 à v13
├── PCB/
│   ├── Gerber_PCB1_2024-10-15.zip      # Fichiers de fabrication
│   ├── BOM_Board1_PCB1_2024-10-15.xlsx # Nomenclature
│   ├── PickAndPlace_PCB1_2024-10-15.*  # Placement composants
│   ├── PCB-steeringWheel.epro          # Projet EasyEDA
│   └── contour-volant.dxf              # Contour du PCB
├── Programmation/
│   ├── logo-binaire.txt                # Bitmap du logo pour l'OLED
│   └── g1177.png                       # Source du logo
└── Picture/                            # Rendus et dessins techniques
```

## Fabrication

**Impression 3D** — imprimer `3D/steering-wheel v12.3mf`. PLA ou PETG, couches de 0.2 mm, remplissage 25 % minimum sur la coque qui encaisse les efforts de direction, supports nécessaires sur les logements de palettes.

**PCB** — envoyer `PCB/Gerber_PCB1_2024-10-15.zip` à un fabricant (JLCPCB, PCBWay). Pour l'assemblage automatisé, joindre le BOM et le fichier pick-and-place, les références LCSC y sont déjà renseignées.

**Logo OLED** — `logo-binaire.txt` contient le bitmap converti en littéraux binaires, à copier directement dans un tableau `PROGMEM` du firmware.

## Reste à faire

- [ ] Publier le firmware Arduino (actuellement archivé en zip, non versionné)
- [ ] Documenter le mapping des boutons et encodeurs
- [ ] Photos du volant assemblé
- [ ] Câblage des palettes et choix des switches
- [ ] Support de fixation sur base de force feedback

---

<div align="center">

**Vandeput Corentin** · [devworks.be](https://devworks.be)

</div>
