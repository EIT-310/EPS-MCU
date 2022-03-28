# EPS-MCU

Firmware for EPS MCU on AAUSAT6

## INSTALL
Follow the intrunctions to install Zephyr on the official [Getting Started Guide](https://docs.zephyrproject.org/latest/getting_started/index.html)

It is recommended to follow the instructions for **Installing within a virtual environment** for both Linux and Windows, as a global installation could mess with other python package dependencies.

Make sure to install the zephyr base in `$HOME/zephyrproject`. The installation should look like the following:
```
$HOME
└── zephyrproject
    ├──.venv        
    ├──.west
    ├── bootloader
    ├── modules
    ├── tools
    ├── west
    └── zephyr
```
If you do not see the `.venv` folder, it means you did not install in a virtual environment.


When installing toolchain use **Zephyr SDK** for Linux and **GNU ARM Embedded Toolchain** for Windows.

When finished installing, make sure the following environment variables are set:
```
ZEPHYR_BASE                 = $HOME/zephyrproject/zephyr
ZEPHYR_TOOLCHAIN_VARIANT    = "zephyr" (Linux) or "gnuarmemb" (Windows)
ZEPHYR_TOOLCHAIN_PATH       = <path to SDK location>          (Linux)
GNUARMEMB_TOOLCHAIN_PATH    = <path to toolchain location>    (Windows)
```

Lastly you need to install `OpenOCD` version 0.11.0 or higher:

* For Ubuntu 21.04 or higher this can be done with `sudo apt install openocd`.
* For ealier Ubuntu versions, install the xPack version using [this guide](https://xpack.github.io/openocd/install/)
* For Windows download the latest binary from [here](https://gnutoolchains.com/arm-eabi/openocd/)

For VSCode to load Zephyr correctly, remember to activate the virtual python environment before opening the workspace like so:
```
EPS-MCU$ source ~/zephyrproject/.venv/bin/activate
EPS-MCU$ code .
``` 