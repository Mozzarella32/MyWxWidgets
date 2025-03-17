# MyWxWidgets
## Description
Collection of things I have written to use for wxWidgets

## Downloading 
Don't forget the submodules
```bash
git clone git@github.com:Mozzarella32/MyWxWidgets.git
git submodule update --init --recursive --remote
cd MyWxWidgets
```

## Building
Go to the root of this Project and run these commands the -j8 flag is for faster compilation and can be omitted
```bash
mkdir build
cd build
cmake ../ && cmake --build . -j8
```
