Todo as of 17 april end of day:
  - clean up macros for consitency, reduce redundancy
  - consider if you have all the benchmarks you want (say, additional erase overrides, for example)
  - move common/common.hpp to a file called common/macros.hpp or something like that
  - in common/common.hpp, add definitions that are global, to avoid using-directives collisions
  - edit CMAKE lists for conditional compilation of each of the different tests, (set, vector, etc. )
  - 
