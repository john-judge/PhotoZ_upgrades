## Referencess

[DAP programming language documentation](http://www.mstarlabs.com/docs/manuals/DAPL3000.PDF)

[DAP C++ interface](http://www.mstarlabs.com/docs/manuals/DAPIO32.PDF)

[EDT C++ interface](https://edt.com/api/group__dv.html)

[Camera configuration](https://edt.com/downloads/ad_config_guide/)

## Lil' Joe Preconfigured Programs

Program No. | Hz | Shape
--- | --- | ---
0 | 1000 | 80x80
1 | 500 | 80x80
2 | 125 | 80x80
3 | 40 | 80x80
4 | 10000 | 80x12
5 | 3000 | 40x40
6 | 5000 | 26x26
7 | 2000 | 80x80

## Lil' Dave Preconfigured Programs

Program No. | Hz | Shape
--- | --- | ---
0 | 200 | 2048x1024
1 | 2000 | 2048x100
2 | 1000 | 1024x320
3 | 2000 | 1024x160
4 | 2000 | 512x160
5 | 4000 | 512x80
6 | 5000 | 256x60
7 | 7500 | 256x40

## Branches

OG-photoz: contains the unaltered version of PhotoZ for the hexagonal camera

new_take: has a new way to acquire images (FP diodes don't work though). Might be easier to synchronize in some situations.

drag-select: started work on being able to select multiple diodes at once and group together for analysis. Requires extensive refactoring to make it work, however.

## Other Information

Neuroplex IDL source code can be found in the C:/RSI/IDL56/np folder (I think) on the Joe computer

EDT/PDV API code with examples can be found at C:/EDT/pdv

The documentation folder in this repo has the PhotoZ user manual and architecture docs, Camera datasheet, and a short test program



Contact: peter.luick@gmail.com
