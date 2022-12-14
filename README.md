Graham scan is an algorithm that creates the convex hull of a given set of points.
 
Find the point with the lowest y coordinate, sort the rest of the points by angle with the x axis.

Process points in order, adding them to the convex hull if they are on the left of the line between the last two points in the hull.

When a concavity is found (currently processed point is on the right of the line between the last two points), pop points from the convex hull until the point is on the left of the line between the last two points in the hull.

## How to use

Click or hold left mouse button to add points to the set.

Right click to start the algorithm.
