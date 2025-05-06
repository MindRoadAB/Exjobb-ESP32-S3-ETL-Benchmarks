# Heap fragmentation monitoring
This program simulates an embedded weather station, sort of.
There are several sensors which "gather" data and "transmit" it onward at regular time intervals. 

Periodically, an "extreme weather event" occurs and triggers more rapid reading of data. 

The purpose of this is to compare how using buffers that consist of ```std::deque``` which use dynamic memory allocation compare to using the Embedded Template Library-based ```etl::deque``` which has a maximum size and does not dynamically allocate memory.

The program uses FreeRTOS tasks (assuming dual core esp-32s) and events and semaphores to coordinate work amongst the different tasks.

Assuming you have installed esp-idf according to the installation instructions from the previous page, after cloning this repository (replace with your own path):
```cd path/to/my/repo/.../Exjobb-ESP32-S3-ETL-Benchmarks/heap_task_tracking```

Setup the project:
```
idf.py set-target <my device>
```

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
2) Component config --> FreeRTOS --> Kernel --> configUSE_TRACE_FACILITY

## To run an instance:
You can run one of four versions of this program, which will run indefinitely:
```
idf.py -DUSE_ETL=<0|1> -DUSE_STATIC=<0|1> build flash monitor
```

```-DUSE_ETL=1``` determines that Embedded Template Library will be used for the buffers in the program. ```-DUSE_ETL=0``` uses the default libstdc++ version of string and deque for data and buffers. 

```-DUSE_STATIC=0``` will create tasks (dynamic allocation) with ```xTaskCreatePinnedToCore``` and ```-DUSE_STATIC=1``` will allocate tasks statically with ```xTaskCreateStaticPinnedToCore```.

## To run the simulation
You can run all four versions in a row (time for each program to run is set inside ```main/simulation.py``` near the top).

Do:
```
python3 main/simulation.py
```

## To plot the results:
You will need ```pandas``` and ```matplotlib```
Checkout:
- https://pandas.pydata.org/docs/getting_started/install.html
- https://matplotlib.org/stable/install/index.html

run:
```
python3 main/plot.py
```
