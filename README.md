#TANC
##Trailblazing, Autonomous, Navigating Car
####A maze runner. A pathfinder engine, solving foreign mazes in shortest number of moves. 
####Currently in development, maintained by Akiva Lipshitz.

The project began with the idea of building a "*maze runner*" robot, but I soon got carried away with simulating the algorithm to make debugging easier. Now, I want to build a robust pathfinding engine for anyone who wants to build their own maze solving robot and needs an engine to be the robot's brains. Right now, the algorithm works, but it hasn't undergone any rigorous testing and I haven't identified any bugs that I haven't solved. It was a great learning experience implementing my own foreign maze solving algorithm, doing some optimization, and writing a large project, 99.99% in pure ISO C (0.01% C++).

In my algorithm I see hints of A\* and depth-first-search. I implemented a heuristic rating and sorting function for the paths. **What makes this algorithm unique is that it solves for looping mazes, that is, mazes in which there are any number of circular paths. **

I would love for anyone to contribute to this project even a small amount of code, or even write some tests. **I would love even more for some kind hearted person to try and kill my code, tell what is wrong, what mistakes I made, things I overlooked, etc. **. 

I should hope to include in this `README` a complete analysis and reflection of the entire project, from start to finish, quantify what I learned and describe in detail the various smaller algorithms that constitute the larger pathfinding engine. 






