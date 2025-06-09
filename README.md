# MyWxWidgets
## Description
Collection of things I have written to use for wxWidgets

## Downloading and Building
```bash
git clone --recurse-submodules --shallow-submodules --jobs 8 https://github.com/Mozzarella32/MyWxWidgets.git
cd MyWxWidgets
mkdir build
cd build
cmake ../ && cmake --build . -j$(nproc)
```
