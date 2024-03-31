# Planet-Donut
Repository containing the code and information about project Planet Donut

In the Planet Donut, I worked in collaboration with another classmate on developing and implementing various algorithms to control robots in a simulated environment for tasks such as exploration, resource extraction, and communication network establishment.

1. **Connection Algorithm**: Involved using a depth-first search algorithm starting from a communication robot at the base center, traversing through its neighbors and their neighbors sequentially until all have been marked as connected in a vector of robots within the base.

2. **Prospection Algorithm**: The space is divided equally across the abscissa relative to the base center for each prospection robot. These robots move diagonally upwards, ensuring that no resources are missed by covering the minimum radius of the deposits on the ordinate axis. Up to 10 robots are created to ensure complete exploration of the planet.

3. **Drilling Algorithm**: A drilling robot is created for each newly discovered resource deposit. These robots are sent to the deposits, making sure there are no existing drilling robots on the target site.

4. **Transport Algorithm**: Transport robots head to each deposit with a drilling robot. If there are more deposits than transport robots, they prioritize the deposit with the most resources, collecting and returning them to the base for processing. If all deposits are depleted, they return to the base to await further instructions.

5. **Communication Algorithm**: The strategy involves deploying a network of 49 communication robots to cover the entire planet, arranged in 7 lines of 7 robots, each approximately 286 km apart. These robots, once deployed, remain stationary, negating the need for maintenance due to their limited movement range compared to their maximum travel distance.

The approach is designed to be robust, prioritizing the rapid collection of information and resources while minimizing unnecessary robot production. This method allows for efficient exploration and resource extraction, contributing to the base's autonomy.

The project also faced challenges, particularly in managing state retention across sessions due to improper variable scope handling, which was later corrected by adjusting the variable's accessibility and scope.

In conclusion, the project successfully implemented a graphical interface and operational strategy for managing robotic tasks in a simulated environment, with a recognition that while the strategy is effective, there is potential for further optimization.

Example of use:
<img width="610" alt="Screenshot 2024-04-01 at 01 35 46" src="https://github.com/mehdi533/Planet-Donut/assets/113531778/a3116e35-6ef5-4fd9-a30d-a6a9342bdff4">
