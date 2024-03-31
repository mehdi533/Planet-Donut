## Planet-Donut

For the project Planet Donut, I worked in collaboration with another classmate on developing and implementing various algorithms to control robots in a simulated environment for tasks such as exploration, resource extraction, and communication network establishment.

Example of output:

<img width="610" alt="Screenshot 2024-04-01 at 01 35 46" src="https://github.com/mehdi533/Planet-Donut/assets/113531778/a3116e35-6ef5-4fd9-a30d-a6a9342bdff4">

1. **Connection Algorithm**: Involved using a depth-first search algorithm starting from a communication robot at the base center, traversing through its neighbors and their neighbors sequentially until all have been marked as connected in a vector of robots within the base.

2. **Prospection Algorithm**: The space is divided equally across the abscissa relative to the base center for each prospection robot. These robots move diagonally upwards, ensuring that no resources are missed by covering the minimum radius of the deposits on the ordinate axis. Up to 10 robots are created to ensure complete exploration of the planet.

3. **Drilling Algorithm**: A drilling robot is created for each newly discovered resource deposit. These robots are sent to the deposits, making sure there are no existing drilling robots on the target site.

4. **Transport Algorithm**: Transport robots head to each deposit with a drilling robot. If there are more deposits than transport robots, they prioritize the deposit with the most resources, collecting and returning them to the base for processing. If all deposits are depleted, they return to the base to await further instructions.

5. **Communication Algorithm**: The strategy involves deploying a network of 49 communication robots to cover the entire planet, arranged in 7 lines of 7 robots, each approximately 286 km apart. These robots, once deployed, remain stationary, negating the need for maintenance due to their limited movement range compared to their maximum travel distance.

The approach is designed to be robust, prioritizing the rapid collection of information and resources while minimizing unnecessary robot production. This method allows for efficient exploration and resource extraction, contributing to the base's autonomy.

Here is the report <ins>in French</ins>:

**Description des Algorithmes**

**Algorithme de connexion :**
L’algorithme de connexion emploie un parcours en profondeur, partant du robot de communication au centre de la base et parcourant ses voisins puis les voisins de ses voisins, jusqu'à ce que tous soient marqués comme connectés dans un vecteur de robots dans la base.

**Algorithme de prospection :**
On divise l’espace en parties égales sur l'abscisse, relatives au centre de la base, pour chaque robot de prospection, et on les fait partir en diagonale vers le haut. À chaque fois que le robot revient au même niveau sur l'axe des ordonnées, il a parcouru le rayon minimal des gisements sur cet axe, assurant ainsi qu'aucun gisement n’échappe aux prospecteurs. On crée jusqu'à 10 robots car avec ce nombre, si les robots font quatre fois le tour de la planète, ils peuvent couvrir toute la carte, un peu moins que la distance maximale qu'ils peuvent parcourir.

**Algorithme de forage :**
Pour les robots de forage, on en crée un à chaque fois qu’un nouveau gisement avec des ressources est découvert. Ils sont ensuite envoyés sur les gisements en vérifiant qu’il n’y a pas déjà de robots de forage sur ces sites.

**Algorithme de transport :**
Les robots de transport se dirigent vers chaque gisement où se trouve un robot de forage. Si le nombre de robots de transport est inférieur au nombre de gisements, ils se dirigent vers le gisement le plus riche en ressources jusqu’à ce qu’il soit vidé. Arrivés à destination, ils collectent une quantité de ressources deltaR, si le gisement le permet, et retournent à la base pour livrer les ressources et se maintenir. Si tous les gisements sont épuisés, ils retournent à la base en attendant de nouvelles instructions.

**Algorithme de communication :**
Si les ressources le permettent, notre stratégie vise à étendre le réseau de communication de la base sur toute la planète à l’aide de 49 robots de communication. Ces derniers sont disposés en 7 lignes de 7 robots, avec une distance d'environ 286 km entre chaque ligne. Le réseau est déployé en fonction du centre de la base. La maintenance de ces robots est superflue, car une fois à leur position finale, ils ne bougent plus. De plus, la distance qu'ils parcourent pour atteindre leur emplacement est au maximum d’environ 1414 km, ce qui est inférieur à la distance maximale de 1420 km qu'ils peuvent parcourir.

**Robustesse de l'approche :**
Nous déployons des robots de communication sur tout l'espace dès que les ressources sont suffisantes, ce qui permet de recueillir les informations des robots de prospection rapidement et à moindre coût. Le déploiement des robots de prospection assure la couverture totale de la planète. Dès la découverte d’un gisement, un robot de forage et les robots de transport disponibles sont envoyés. Des robots de transport sont recréés à chaque récupération de ressources, sans jamais dépasser cinq par gisement découvert, pour éviter la création de robots superflus. À chaque gisement découvert encore riche en ressources, on envoie un robot de forage accompagné d'au moins un robot de transport pour en extraire les ressources, contribuant ainsi à l'autonomie de la base.
