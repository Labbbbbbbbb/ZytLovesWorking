Resources:

[X-AnyLabeling/docs/en/get_started.md at main · CVHub520/X-AnyLabeling](https://github.com/CVHub520/X-AnyLabeling/blob/main/docs/en/get_started.md)

安装完成后运行只需

```
python anylabeling/app.py
```





自动追踪模型依赖问题

[X-AnyLabeling/examples/interactive_video_object_segmentation/README.md at main · CVHub520/X-AnyLabeling · GitHub](https://github.com/CVHub520/X-AnyLabeling/blob/main/examples/interactive_video_object_segmentation/README.md)

一、自动标注视频追踪模型Segment Anything 2 Video下载不下来的问题：

去到X-Anylabeling仓库

1. 定位到anylabeling/services/auto_labeling/segment_anything_2_video.py文件

2. 找到build_sam2_camera_predictor方法调用

3. 将其修改为build_sam2_video_predictor

4. 确保所有相关依赖已正确安装 `pip install sam2`

   

二、Sam视频无法自动追踪

```
2025-12-11 16:43:16,453 | WARNING | segment_anything_2_video:predict_shapes:477 - Could not inference model
2025-12-11 16:43:16,454 | WARNING | segment_anything_2_video:predict_shapes:478 - 'SAM2VideoPredictor' object has no attribute 'load_first_frame'
Traceback (most recent call last):
  File "F:\ITS_Log\MachineLearning\Yolo\tools\X-AnyLabeling-env\X-AnyLabeling\anylabeling\services\auto_labeling\segment_anything_2_video.py", line 471, in predict_shapes
    shapes, replace = self.video_process(cv_image, filename)
  File "F:\ITS_Log\MachineLearning\Yolo\tools\X-AnyLabeling-env\X-AnyLabeling\anylabeling\services\auto_labeling\segment_anything_2_video.py", line 407, in video_process
    self.video_predictor.load_first_frame(cv_image)
  File "F:\Anaconda3\envs\x-anylabeling\lib\site-packages\torch\nn\modules\module.py", line 1940, in __getattr__
    raise AttributeError(
AttributeError: 'SAM2VideoPredictor' object has no attribute 'load_first_frame'
```

上面这个疑似是torch和cuda版本不匹配  又换成cuda12.之后：

```
2025-12-11 19:43:37,494 | WARNING | segment_anything_2_video:predict_shapes:484 - Could not inference model
2025-12-11 19:43:37,495 | WARNING | segment_anything_2_video:predict_shapes:485 - cannot import name '_C' from 'sam2' (F:\ITS_Log\MachineLearning\Yolo\tools\X_Anylabeling_sam2_env\segment-anything-2\sam2\__init__.py)
```

回到segment-anything-2仓库的目录下
`pip install -e . --no-binary :all:`

`pip install -e . --no-build-isolation`

一通乱七八糟我也记不清的操作之后（对了，上面说的改源文件的方法和sam2疑似都没有鸟用）似乎突然可以识别了

但是坏处是cuda.available又变成false了现在只能用cpu硬跑像乌龟一样慢

噢重新删掉再下一遍cuda12又好了。。。

```
>>> import torch
>>> print(torch.__version__)
2.5.1+cu121
>>> print(torch.cuda.is_available())
True
```

这次倒是能跑了但是跑一半又寄了

```
ImportError: cannot import name '_C' from 'sam2' (F:\ITS_Log\MachineLearning\Yolo\tools\X_Anylabeling_sam2_env\segment-anything-2\sam2\__init__.py)
2025-12-11 20:40:00,792 | WARNING | segment_anything_2_video:predict_shapes:484 - Could not inference model
2025-12-11 20:40:00,792 | WARNING | segment_anything_2_video:predict_shapes:485 - cannot import name '_C' from 'sam2' (F:\ITS_Log\MachineLearning\Yolo\tools\X_Anylabeling_sam2_env\segment-anything-2\sam2\__init__.py)
Traceback (most recent call last):
  File "F:\ITS_Log\MachineLearning\Yolo\tools\X_Anylabeling_sam2_env\X-AnyLabeling\anylabeling\services\auto_labeling\segment_anything_2_video.py", line 478, in predict_shapes
    shapes, replace = self.video_process(cv_image, filename)
  File "F:\ITS_Log\MachineLearning\Yolo\tools\X_Anylabeling_sam2_env\X-AnyLabeling\anylabeling\services\auto_labeling\segment_anything_2_video.py", line 438, in video_process
    ) = self.video_predictor.add_new_prompt(
  File "F:\Anaconda3\envs\x-anylabeling-sam2\lib\site-packages\torch\utils\_contextlib.py", line 116, in decorate_context
    return func(*args, **kwargs)
  File "F:\ITS_Log\MachineLearning\Yolo\tools\X_Anylabeling_sam2_env\segment-anything-2\sam2\sam2_camera_predictor.py", line 274, in add_new_prompt
    current_out, _ = self._run_single_frame_inference(
  File "F:\ITS_Log\MachineLearning\Yolo\tools\X_Anylabeling_sam2_env\segment-anything-2\sam2\sam2_camera_predictor.py", line 990, in _run_single_frame_inference
    pred_masks_gpu = fill_holes_in_mask_scores(
  File "F:\ITS_Log\MachineLearning\Yolo\tools\X_Anylabeling_sam2_env\segment-anything-2\sam2\utils\misc.py", line 238, in fill_holes_in_mask_scores
    labels, areas = get_connected_components(mask <= 0)
  File "F:\ITS_Log\MachineLearning\Yolo\tools\X_Anylabeling_sam2_env\segment-anything-2\sam2\utils\misc.py", line 61, in get_connected_components
    from sam2 import _C
ImportError: cannot import name '_C' from 'sam2' (F:\ITS_Log\MachineLearning\Yolo\tools\X_Anylabeling_sam2_env\segment-anything-2\sam2\__init__.py)
```

还是这个问题

解决：安装vs2022build tool





不玩了等以后去linux玩

