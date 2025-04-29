Todo as of 22 april lunch:
  - ~~clean up macros for consitency, reduce redundancy~~
  - consider if you have all the benchmarks you want (say, additional erase overrides, for example)
  - ~~edit CMAKE lists for conditional compilation of each of the different tests, (set, vector, etc. )~



## Benchmarking
    - remember: $idf.py menuconfig -> Component config -> ESP System Settings -> Disable watchdog timer
    - remember: Increase main stack size:
        - $idf.py menuconfig -> componenet ->config - Esp system settings-> Main Task Stack Size -> 8192 
