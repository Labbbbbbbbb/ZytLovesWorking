## launch.json配置

```
{
    "version": "0.2.0",
    "configurations": [
        {
            "cwd": "${workspaceRoot}",			//openocd版本
            "type": "cortex-debug",
            "request": "launch",
            "name": "openocd",
            "servertype": "openocd",
            "executable": "build\\Debug\\F405RGT6.elf",		//.elf前面填自己的工程名
            "runToEntryPoint": "main",
            "configFiles": [
                "interface/cmsis_dap.cfg",
                "target/stm32f4x.cfg"			//根据自己使用的板子型号改，f1则改成f1x
            ],
            "toolchainPrefix": "arm-none-eabi"
        },
        {
            "cwd": "${workspaceRoot}",		//stlink版本
            "type": "cortex-debug",
            "request": "launch",
            "name": "stlink",
            "servertype": "openocd",
            "executable": "build\\Debug\\F405RGT6.elf",
            "runToEntryPoint": "main",
            "configFiles": [
                "interface/stlink.cfg",
                "target/stm32f4x.cfg"
            ],
            "toolchainPrefix": "arm-none-eabi"
        }
    ]
}
```

