## Convex hull - Graham's scan

Graham's scan is one of the methods of calculating the convex hull of a point set. The convex hull is the smallest polygon that contains all the points in the set.

![convex hull example](https://github.com/RaduHaulica/Graham-scan/blob/96c97bca1a5eed623869b565c16dbcb43d5b4c2b/graham%20scan/media/graham%20scan%20example.png)

The applications of calculating a convex hull are varied:
- calculating the spread of a contamination based on activated sensors
- optimizing robot motion (rudimentary pathfinding and obstacle avoidance)
- finding out the minimum length of fence required to protect Stonehenge from tourists
- how much the T-1000 has to move in order to dodge the T-800's shotgun shells (avoid collision with a group of objects without having to perform calculations for each individual element in the system)

## Algorithm

The solution is built on a stack, as it requires tracing back previous steps. The algorithm starts by finding the lowest point with the lowest y component in the set, which is marked as the starting point and pushed on the stack. All the other points are sorted based on the angle between the (x axis) and the (line they make with the starting point).

The sorted points are processed in order, adding them to the convex hull (current solution stack) if they are on the left of the line between the last two points in the hull (which means that they don't break the convexity of the solution polygon).

When a concavity is found (currently processed point is on the right of the line between the last two points), points are popped from the convex hull (current solution stack) until the currently processed point is on the left of the line between the last two points in the hull.

## How to use

Click or hold left mouse button to add points to the set.

Right click to start the algorithm visualization.
