# PhotoZ
CCD controller project for Dr. Meyer Jackson's hVOS voltage imaging system. This repository tracks the transition from PhotoZ OG to NIDAQ / Little Dave Camera

# Log / Discussion

## Meeting 11/8 with Sarwagya

Memory allocation issue
- Recommend restructuring `DataArray.cpp` to not initialize and use so many pointers to the `Data` class
- Set bin size to high to decrase amount of mem allocated for binning, thus isolating the raw data allocation issue. See if that issue is solvable before tackling the binned mem allocation
- There's debugging output to measure amount of mem allocated
- Also consider re-organizing the responsibilities between DataArray and Data? Might not be worth doing.
