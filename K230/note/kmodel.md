资料:[K230 YOLO 大作战 — CanMV K230](https://www.kendryte.com/k230_canmv/zh/main/zh/example/ai/YOLO大作战.html)



运行` python to_kmodel.py --target k230 --model ../../runs/train/exp12/weights/best.onnx --dataset ../test --input_width 640 --input_height 480 --ptq_option 0`时报错

```
ImportError: DLL load failed while importing _nncase: 找不到指定的模块。
```



解决：（文件放在同级目录下

![image-20250727010746280](.\assets\image-20250727010746280.png)

继续报错:

```
You must install or update .NET to run this application.

App: F:\Anaconda3\envs\yolov5_cq09\python.exe
Architecture: x64
Framework: 'Microsoft.NETCore.App', version '7.0.0' (x64)
.NET location: C:\Program Files\dotnet\

The following frameworks were found:
  6.0.16 at [C:\Program Files\dotnet\shared\Microsoft.NETCore.App]
  6.0.27 at [C:\Program Files\dotnet\shared\Microsoft.NETCore.App]
  8.0.14 at [C:\Program Files\dotnet\shared\Microsoft.NETCore.App]

Learn more:
https://aka.ms/dotnet/app-launch-failed

To install missing framework, download:
https://aka.ms/dotnet-core-applaunch?framework=Microsoft.NETCore.App&framework_version=7.0.0&arch=x64&rid=win-x64&os=win10
Traceback (most recent call last):
  File "to_kmodel.py", line 7, in <module>
    import nncase
  File "F:\Anaconda3\envs\yolov5_cq09\lib\site-packages\nncase\__init__.py", line 46, in <module>
    _initialize()
  File "F:\Anaconda3\envs\yolov5_cq09\lib\site-packages\nncase\__init__.py", line 43, in _initialize
    _nncase.initialize(compiler_path)
RuntimeError: Failed to initialize hostfxr: 0x80008096.
```



说是还要下载`7.0`的`.net runtime`

https://builds.dotnet.microsoft.com/dotnet/Runtime/7.0.20/dotnet-runtime-7.0.20-win-x64.exe

继续报错：

```
warn: Nncase.Hosting.PluginLoader[0]
      NNCASE_PLUGIN_PATH is not set.
to_kmodel.py:25: DeprecationWarning: `mapping.TENSOR_TYPE_TO_NP_TYPE` is now deprecated and will be removed in a future release.To silence this warning, please use `helper.tensor_dtype_to_np_dtype` instead.
  input_dict['dtype'] = onnx.mapping.TENSOR_TYPE_TO_NP_TYPE[onnx_type.elem_type]
WARNING: The argument `input_shapes` is deprecated. Please use `overwrite_input_shapes` and/or `test_input_shapes`
instead. An error will be raised in the future.
to_kmodel.py:66: DeprecationWarning: BILINEAR is deprecated and will be removed in Pillow 10 (2023-07-01). Use Resampling.BILINEAR instead.
  img_data = img_data.resize((shape[3], shape[2]), Image.BILINEAR)
```



` Nncase.Hosting.PluginLoader[0] NNCASE_PLUGIN_PATH is not set.`的问题可以通过手动添加环境变量解决，在编辑环境变量那里添加一个变量`NNCASE_PLUGIN_PATH`，值是对应`nncase`的地址

但是最后一步遇到警告啥的其实不影响出结果，去`onnx`的同级目录看一下是不是已经生成了`kmodel`





第一次环境配好之后只需要运行下面这些：(注意修改路径)

```
# 导出onnx，pt模型路径请自行选择
python export.py --weight runs/train/exp/weights/best.pt --imgsz 640 --batch 1 --include onnx
cd test_yolov5/detect
# 转换kmodel,onnx模型路径请自定义，生成的kmodel在onnx模型同级目录下
python to_kmodel.py --target k230 --model ../../runs/train/exp/weights/best.onnx --dataset ../test --input_width 640 --input_height 480 --ptq_option 0
```

