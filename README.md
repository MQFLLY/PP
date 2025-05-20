# Fluctuation-enhanced Scheme for Solving $k$-Partition Problems via Population Protocols
This project serves as the code repository for the paper titled *_Fluctuation-enhanced scheme for solving k-partition problems via population protocols_*.
## What is Population Protocol?
Population protocol is a swarm intelligence computing protocol.
## Project Overview
This project designs a population protocol to efficiently solve the $k$-partition problem.
## Configuration Environment
The code is configured for the Linux system. Please run it under Linux and install necessary dependencies such as cmake and g++.
``` bash
mkdir -p build
cd build && cmake ..
make
./PopulationProtocol
```

## Code Functions
Calculate the theoretical value of the expected number of interactions under specific $n$ and $k$.

Calculate the experimental value of the expected number of interactions under specific $n$ and $k$.

Calculate the protocol value of the expected number of interactions under Yasumi et al.'s protocol.

Plot the statistical values using Python.
## Interaction Mode
The code in this project is interactive, making it extremely simple and convenient to use.
