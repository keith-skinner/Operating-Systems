# Task Description
Implement a solution to the Dining Philosophers problem using the  `pthread` library's mutex and conditional variable functions.  
Recall the pseudocode from the lecture notes:
```
monitor DiningPhilosophers {
  enum { THINKING; HUNGRY; EATING } state[5];
  condition self[5];
  
  void pickup(int i) 
  {
    state[i] = HUNGRY;
    test(i);
    if (state[i] != EATING)
      self[i].wait();
  }
  
  
}
```
