My take on somewhat classic test task for C++ developers.

 [\<demo gif on gfycat\>](https://gfycat.com/CraftySeriousGallinule) 

```
Write program in C++ using Qt that simulates movement of N balls on a plane.
Balls attract to one another with force
```

 [F(r) = 1/r - 1/r^2](http://www.wolframalpha.com/input/?i=1%2Fr+-+1%2Fr%5E2) 

```
r - distance between two balls. Original placement of balls is random.
While app is running, the user can drag balls with mouse pointer, pop them
(via click on a ball) or add new ones (via second mouse button click on free space).

All calculations should be performed in a separate thread. Main UI thread should
only draw the results and interact with the user. Thread communication should be
correct and not result in ill-formed program or UB. Animation of movement should
be smooth. Qt should not be used in calculations thread. Program should be
accompanied by unit-tests proving its correctness.
```
---

For unit-tests I used [Catch-Lib](http://github.com/philsquared/catch) - although the vacancy asked for [Google Test](https://github.com/google/googletest), I did not have any experience with it, and being short on time decided not to devote time to learning it, using a familiar tool instead.
