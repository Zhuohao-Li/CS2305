# Cache Lab
**Course Information**: CS359 Computer Architecture, for SJTU CS


This is the second project:  Understanding Cache Memories

# Part A
## work file
csim.c
## complie
```makefile
make clean
make
```

## run small test
```
./csim -s 2 -E 2 -b 2 -t traces/yi2.trace
```
## run csim-ref for test
```
./csim-ref -s 2 -E 2 -b 2 -t traces/yi2.trace
```
## run evaluation
```
 ./test-csim
```

# Part B

## work file
trans.c

## Adding registration
```c
/* Header comment */
char trans_simple_desc[] = "A simple transpose";
void trans_simple(int M, int N, int A[N][M], int B[M][N])
{ /* your transpose code here */ }
```

Then add:
```c
registerTransFuncti       on(transpose_submit, transpose_submit_desc);
```

## Evaluation
```
make
./test-trans -M 32 -N 32
```

## Debuging
```
./csim-ref -v -s 5 -E 1 -b 5 -t trace.f0
```

## Target

• 32 × 32: 8 points if m < 300, 0 points if m > 600

• 64 × 64: 8 points if m < 1, 300, 0 points if m > 2, 000

• 61 × 67: 10 points if m < 2, 000, 0 points if m > 3, 000



## Data Record
### 32*32
blocking 4: hits:1566, misses:487, evictions:455
blocking 8: hits:1710, misses:343, evictions:311
blocking 16: hits:870, misses:1183, evictions:1151 
blocking 8 + read all only once: hits:1766, misses:287, evictions:255 **(best)**

### 64*64
blocking 4: hits:6306, misses:1891, evictions:1859
blocking 8: hits:3474, misses:4723, evictions:4691
blocking 16: hits:3474, misses:4723, evictions:4691
blocking 4 + read all only once: hits:6498, misses:1699, evictions:1667
blocking 8 + read all only once: hits:3586, misses:4611, evictions:4579
-blocking 4 + read all only once + improve: hits:6530, misses:1667, evictions:1635-
blocking 8 + read all only once + improve: hits:9066, misses:1179, evictions:1147 **(best)**

### 61*67
(Blocking 8 Read All Only Once Improved Random Shape transpose): hits:7888, misses:2113, evictions:2081
(Blocking 15 Random Shape transpose): hits:5922, misses:2271, evictions:2239
(Blocking 16 Random Shape transpose): hits:6194, misses:2063, evictions:2031
(Blocking 17 Random Shape transpose): hits:5902, misses:2597, evictions:2565
(Blocking 16 Read All Random Shape transpose): hits:6264, misses:1993, evictions:1961 **(best)**
