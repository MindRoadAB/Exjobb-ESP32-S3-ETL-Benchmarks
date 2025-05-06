# Benchmarks

The purpose of this is to perform some of the basic operations one frequently encounters with the C++ containers std::string, std::vector, std::unordered_map, std::unordered_set, and std::deque and compare the cpu cycles required to perform these operations compared to their Embedded Template Library counterparts.

Assuming you have installed esp-idf according to the installation instructions from the previous page, after cloning this repository (replace with your own path):
```cd path/to/my/repo/.../Exjobb-ESP32-S3-ETL-Benchmarks/Benchmarks```

Setup the project:
```idf.py set-target <my device>```

Where ```<my_device>``` is an esp32 device. This project has been tested on ESP32-S3 model as well as a base ESP-32.
For example, using an ESP32-S3 I do:
```
idf.py set-target esp32s3
```

Then:

```
idf.py menuconfig
```
1) And go to: Component config --> ESP System Settings --> Main task stack size: 8192
2) Then go to Component config --> ESP System Settings --> Enable Task Watchdog Timer and turn it OFF.
3) Exit and save(Q, and then Y).

## You can run an individual benchmark test as follows:
```
idf.py -DUSE_ETL=<0|1> -DTYPE=<string | vector | map | set | deque> build flash monitor
```
```-DUSE_ETL=0``` is to NOT use Embedded Template Library and ```-DUSE_ETL=1``` IS to use the Embedded Template Library. Similarily, giving one of ```string, vector, map, set, deque``` will determine which program is compiled and run. 

## To run the entire test suite:
From the project root (aka: Exjobb-ESP32-ETL-Benchmarks/Benchmarks) run:
```
python3 main/test.py
```
If you are missing any dependencies, it will let you know :-)

## To Plot the results:
You will need ```pandas``` and ```matplotlib```
Checkout:
- https://pandas.pydata.org/docs/getting_started/install.html
- https://matplotlib.org/stable/install/index.html

run:
```
python3 main/plot.py
```

