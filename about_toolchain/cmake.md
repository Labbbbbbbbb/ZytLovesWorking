## cmake工程结构

```
F:.
├─.vscode
├─build		#独立于源码之外的build文件夹，防止生成的东西污染源码
│  └─CMakeFiles
│      ├─4.1.1
│      │  └─CompilerIdCXX
│      │      └─tmp
│      ├─CMakeScratch
│      ├─hello.dir
│      └─pkgRedirects
└─c_plus		#源码，里面包含CMakeLists.txt文件
```

在同一个目录 `c_plus` 里新建 `CMakeLists.txt`，内容如下：（示例）

```cmkelists
cmake_minimum_required(VERSION 3.10)

# 工程名 & 语言
project(c_plus LANGUAGES CXX)

# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 生成可执行文件
add_executable(hello hello.cpp)
```

注意⚠️：这里写的是 `hello`，不用写 `.exe`，CMake 会在 Windows 自动加后缀。

## 终端执行cmake

在 `F:\C_plus_tmp_and_git` 目录下执行：

```powershell
# 新建 build 目录，避免源文件夹被污染
mkdir build
cd build

# 让 CMake 用 MinGW  （当然首先要下好mingw）
cmake -G "MinGW Makefiles" ../c_plus

# 编译
mingw32-make
```

编译成功后，你会在 `build` 目录下看到 `hello.exe`。





此外，也可以直接写一个 **PowerShell 脚本**，一键完成 **创建 build → 运行 cmake → 编译 → 执行程序**。

------

###  在工程根目录 `F:\C_plus_tmp_and_git` 新建一个脚本

文件名：`build_and_run.ps1`

内容如下：

```
# 设置路径
$srcDir = "c_plus"
$buildDir = "build"

# 如果指定了 -Clean，就删除并重建 build 文件夹
if ($Clean) {
    if (Test-Path $buildDir) {
        Write-Host "🧹 正在清理 $buildDir ..."
        Remove-Item -Recurse -Force $buildDir
    }
}

# 如果 build 目录不存在就创建
if (!(Test-Path $buildDir)) {
    New-Item -ItemType Directory -Path $buildDir | Out-Null
}

# 进入 build 目录
Set-Location $buildDir

# 运行 cmake，指定 MinGW
cmake -G "MinGW Makefiles" ../$srcDir

if ($LASTEXITCODE -ne 0) {
    Write-Host "❌ CMake 配置失败"
    exit 1
}

# 编译
mingw32-make
if ($LASTEXITCODE -ne 0) {
    Write-Host "❌ 编译失败"
    exit 1
}

# 运行程序
Write-Host "✅ 编译成功，正在运行程序..."
.\hello.exe
```

在 PowerShell 里进入 `F:\C_plus_tmp_and_git`，然后执行：

```
.\build_and_run.ps1
#或
.\build_and_run.ps1 -Clean  #这会先删除 build/ 文件夹，再重新跑 cmake 和 mingw32-make。
```

第一次运行可能会提示执行策略限制，可以先允许当前会话运行脚本：

```
Set-ExecutionPolicy -ExecutionPolicy RemoteSigned -Scope Process
```





问题：中文乱码 

添加了很多chcp或者

```
[Console]::OutputEncoding = [System.Text.Encoding]::UTF8
$OutputEncoding = [System.Text.Encoding]::UTF8
```

之类的都没有解决，最后去下载了powershell7版本，对utf-8支持更完整

[Releases · PowerShell/PowerShell](https://github.com/PowerShell/PowerShell/releases)

下载windowsx64的msi文件  勾选添加环境变量，注意下载完成后可能得输入 **"PowerShell 7"** 或 **"pwsh"** 才能看到，不然会沿用原版本

它的可执行程序是 `pwsh.exe`，和旧的 `powershell.exe` 不冲突。

```
(base) PS F:\C_plus_tmp_and_git> $PSVersionTable

-- Configuring done (0.0s)
-- Generating done (0.0s)
-- Build files have been written to: F:/C_plus_tmp_and_git/build
[100%] Built target hello
编译成功，正在运行程序...  #不乱码了
Hello, World!
```

