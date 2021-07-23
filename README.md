# Neural Networks Verification Comparison

## Building

### Marabou

```
cd marabou
mkdir build
cd build
cmake ..
```

### Neurify

```
cd reluval
sudo apt-get install libopenblas-base
wget https://github.com/xianyi/OpenBLAS/archive/v0.3.6.tar.gz
tar -xzf v0.3.6.tar.gz
cd OpenBLAS-0.3.6
make
make install
mv OpenBLAS-0.3.6 OpenBLAS
make
```

### Reluplex

```
cd reluplex
cd glpk-4.60
./configure_glpk.sh
make
make install
cd ..
cd reluplex
make
cd ..
cd check_properties
make
```

### Runner

```
g++ <runner.cpp> -std=c++17 -lstdc++fs -lpthread
```
