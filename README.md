

# 🚀 Système de Gestion de Commandes Robotisées avec Priorités & Détection d’Obstacles

Un projet complet en **C** simulant la gestion des commandes d’un robot mobile avec :

- Files d’attente **urgente** et **normale**
- Journalisation avancée avec **horodatage cumulatif**
- Détection d’obstacles & contournement automatique
- Construction d’un **arbre binaire** pour identifier les chemins sans obstacle
- Interface console interactive
- Simulation réaliste du comportement d’un robot autonome

---

## 🧠 Fonctionnalités principales

### 🔥 1. Gestion de commandes avec priorité
Le système utilise deux files FIFO :
- **File urgente** → toujours prioritaire
- **File normale** → utilisée seulement si la file urgente est vide

Chaque commande contient :
- Type de mouvement (avancer, reculer, tourner…)
- Durée
- Vitesse
- Priorité

---

### ⚙️ 2. Boucle d’exécution intelligente
La fonction `execution()` :
- Exécute *toujours* les commandes urgentes d’abord
- Passe aux commandes normales uniquement si la file urgente est vide
- Journalise chaque commande
- Simule des obstacles pendant les commandes normales
- Exemple d’affichage :COMMANDE URGENTE!!!![Type: ARRET_URGENCE | Durée=3s | Vitesse=0 | Priorité=URGENTE]

 ---

### 📝 3. Journalisation avec horodatage
Chaque commande est enregistrée dans :  
📄 **journal_robot.txt**

Format :[00:03:12] [URGENTE] AVANCER | duree=2s | vitesse=1.50

L’horodatage avance en fonction de la durée des commandes → simulation réaliste.

---

### 🛑 4. Détection et contournement d’obstacles
La fonction `capteur()` simule un obstacle aléatoire (10% de chance).

En cas d’obstacle, le robot génère automatiquement 3 commandes urgentes :

1. Arrêt d’urgence  
2. Reculer  
3. Tourner à droite  

---

### 🌳 5. Arbre binaire : chemins sans obstacle
Le programme peut construire un arbre où chaque nœud représente une commande avec :
- Un indicateur d’obstacle
- Sous-arbres gauche / droite

La fonction `chemins_OK()` :
- Affiche tous les chemins depuis la racine
- **Uniquement si aucun obstacle n’est rencontré dans le chemin**

Exemple de sortie :
Chemin valide :
[AVANCER...][TOURNER_GAUCHE...][AVANCER...]

---

## 📌 Menu interactif

    1. Ajouter une commande dans les files 
    2. Faire l'exécution
    3. Construire un arbre et afficher chemins sans obstacle 
    4. Quitter


---
## 📷 Aperçu du fonctionnement

### Files d’attente
    URGENTE : [Commande 1] → [Commande 2] → ...  
    NORMALE : [Commande A] → [Commande B] → ...

### Arbre d’exécution
            (Commande 1)
            /      \
    (Commande 2)   (Commande 3)
            |              |
        Obst?           Pas d’obstacle → continue

## 🚧 Améliorations futures possibles

- Vraie lecture capteurs réels (ultrasons, IR…)
- Interface graphique avec SDL / OpenGL
- Gestion multi-robots
- Planification intelligente (A*, Dijkstra…)
- Export JSON des commandes
---
## 👤 Auteur

Projet développé pour démontrer :

- La gestion des priorités en C
- Les structures de données (file, arbre binaire)
- La simulation robotique logicielle
- L’organisation d’un projet modulaire

