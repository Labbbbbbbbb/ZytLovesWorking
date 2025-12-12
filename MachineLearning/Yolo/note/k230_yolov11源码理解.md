ps:还有一部分的学习记录放在k230文件夹 写在cyt那个资料的注释里面

```
#来自k230的固件 /sdcard/libs/YOLO.py
class YOLO11(AIBase):
    def __init__(self,task_type="detect",mode="video",kmodel_path="",labels=[],rgb888p_size=[320,320],model_input_size=[320,320],display_size=[1920,1080],conf_thresh=0.5,nms_thresh=0.45,mask_thresh=0.5,max_boxes_num=50,debug_mode=0):
        if task_type not in ["classify","detect","segment","obb"]:
            raise Exception("Please select the correct task_type parameter, including 'classify', 'detect', 'segment','obb'.")
        super().__init__(kmodel_path,model_input_size,rgb888p_size,debug_mode)
        self.task_type=task_type
        self.mode=mode
        self.kmodel_path=kmodel_path
        self.labels=labels
        self.class_num=len(labels)
        if mode=="video":
            self.rgb888p_size=[ALIGN_UP(rgb888p_size[0],16),rgb888p_size[1]]
        else:
            self.rgb888p_size=[rgb888p_size[0],rgb888p_size[1]]
        self.model_input_size=model_input_size
        self.display_size=[ALIGN_UP(display_size[0],16),display_size[1]]

        self.conf_thresh=conf_thresh
        self.nms_thresh=nms_thresh
        self.mask_thresh=mask_thresh
        self.max_boxes_num=max_boxes_num
        self.debug_mode=debug_mode

        self.scale=1.0
        self.colors=get_colors(len(self.labels))
        self.masks=None
        if self.task_type=="segment":
            if self.mode=="image":
                self.masks=np.zeros((1,self.rgb888p_size[1],self.rgb888p_size[0],4),dtype=np.uint8)
            elif self.mode=="video":
                self.masks=np.zeros((1,self.display_size[1],self.display_size[0],4),dtype=np.uint8)
        # Ai2d实例，用于实现模型预处理
        self.ai2d=Ai2d(self.debug_mode)
        # 设置Ai2d的输入输出格式和类型
        self.ai2d.set_ai2d_dtype(nn.ai2d_format.NCHW_FMT,nn.ai2d_format.NCHW_FMT,np.uint8, np.uint8)

    # 配置预处理操作，这里使用了resize，Ai2d支持crop/shift/pad/resize/affine，具体代码请打开/sdcard/app/libs/AI2D.py查看
    def config_preprocess(self,input_image_size=None):
        with ScopedTiming("set preprocess config",self.debug_mode > 0):
            # 初始化ai2d预处理配置，默认为sensor给到AI的尺寸，您可以通过设置input_image_size自行修改输入尺寸
            ai2d_input_size=input_image_size if input_image_size else self.rgb888p_size
            if self.task_type=="classify":
                top,left,m=center_crop_param(self.rgb888p_size)
                self.ai2d.crop(left,top,m,m)
            elif self.task_type=="detect":
                # 计算padding参数
                top,bottom,left,right,self.scale=letterbox_pad_param(self.rgb888p_size,self.model_input_size)
                # 配置padding预处理
                self.ai2d.pad([0,0,0,0,top,bottom,left,right], 0, [128,128,128])
            elif self.task_type=="segment":
                top,bottom,left,right,scale=letterbox_pad_param(self.rgb888p_size,self.model_input_size)
                self.ai2d.pad([0,0,0,0,top,bottom,left,right], 0, [128,128,128])
            elif self.task_type=="obb":
                # 计算padding参数
                top,bottom,left,right,self.scale=letterbox_pad_param(self.rgb888p_size,self.model_input_size)
                # 配置padding预处理
                self.ai2d.pad([0,0,0,0,top,bottom,left,right], 0, [128,128,128])
            self.ai2d.resize(nn.interp_method.tf_bilinear, nn.interp_mode.half_pixel)
            # build参数包含输入shape和输出shape
            self.ai2d.build([1,3,ai2d_input_size[1],ai2d_input_size[0]],[1,3,self.model_input_size[1],self.model_input_size[0]])

    def postprocess(self,results):
        with ScopedTiming("postprocess",self.debug_mode > 0):
            if self.task_type=="classify":
                scores=results[0][0]
                max_score=np.max(scores)
                res_idx=np.argmax(scores)
                cls_res=(-1,0.0)
                # 如果类别分数大于阈值，返回当前类别和分数
                if max_score>self.conf_thresh:
                    cls_res=(res_idx,max_score)
                return cls_res
            elif self.task_type=="detect":
                new_result=results[0][0].transpose()
                if self.mode=="image":
                    det_res = aidemo.yolov8_det_postprocess(new_result.copy(),[self.rgb888p_size[1],self.rgb888p_size[0]],[self.model_input_size[1],self.model_input_size[0]],[self.rgb888p_size[1],self.rgb888p_size[0]],len(self.labels),self.conf_thresh,self.nms_thresh,self.max_boxes_num)
                elif self.mode=="video":
                    det_res = aidemo.yolov8_det_postprocess(new_result.copy(),[self.rgb888p_size[1],self.rgb888p_size[0]],[self.model_input_size[1],self.model_input_size[0]],[self.display_size[1],self.display_size[0]],len(self.labels),self.conf_thresh,self.nms_thresh,self.max_boxes_num)
                return det_res
            elif self.task_type=="segment":
                new_result=results[0][0].transpose()
                if self.mode=="image":
                    seg_res = aidemo.yolov8_seg_postprocess(new_result.copy(),results[1][0],[self.rgb888p_size[1],self.rgb888p_size[0]],[self.model_input_size[1],self.model_input_size[0]],[self.rgb888p_size[1],self.rgb888p_size[0]],len(self.labels),self.conf_thresh,self.nms_thresh,self.mask_thresh,self.masks)
                elif self.mode=="video":
                    seg_res = aidemo.yolov8_seg_postprocess(new_result.copy(),results[1][0],[self.rgb888p_size[1],self.rgb888p_size[0]],[self.model_input_size[1],self.model_input_size[0]],[self.display_size[1],self.display_size[0]],len(self.labels),self.conf_thresh,self.nms_thresh,self.mask_thresh,self.masks)
                return seg_res
            elif self.task_type=="obb":
                new_result=results[0][0].transpose()
                if self.mode=="image":
                    obb_res = aidemo.yolo_obb_postprocess(new_result.copy(),[self.rgb888p_size[1],self.rgb888p_size[0]],[self.model_input_size[1],self.model_input_size[0]],[self.rgb888p_size[1],self.rgb888p_size[0]],len(self.labels),self.conf_thresh,self.nms_thresh,self.max_boxes_num)
                elif self.mode=="video":
                    obb_res = aidemo.yolo_obb_postprocess(new_result.copy(),[self.rgb888p_size[1],self.rgb888p_size[0]],[self.model_input_size[1],self.model_input_size[0]],[self.display_size[1],self.display_size[0]],len(self.labels),self.conf_thresh,self.nms_thresh,self.max_boxes_num)
                return obb_res

    def draw_result(self,res,img):
        with ScopedTiming("draw result",self.debug_mode > 0):
            if self.mode=="video":
                if self.task_type=="classify":
                    ids,score=res[0],res[1]
                    if ids!=-1:
                        img.clear()
                        mes=self.labels[ids]+" {0:.3f}".format(score)
                        img.draw_string_advanced(5,5,32,mes,color=(0,255,0))
                    else:
                        img.clear()
                elif self.task_type=="detect":
                    if res:
                        img.clear()
                        for i in range(len(res[0])):
                            x, y, w, h = map(lambda x: int(round(x, 0)), res[0][i])
                            img.draw_rectangle(x,y, w, h, color=self.colors[res[1][i]],thickness=4)
                            img.draw_string_advanced( x , y-50,32," " + self.labels[res[1][i]] + " {0:.3f}".format(res[2][i]) , color=self.colors[res[1][i]])
                    else:
                        img.clear()
                elif self.task_type=="segment":
                    if res[0]:
                        img.clear()
                        mask_img=image.Image(self.display_size[0], self.display_size[1], image.ARGB8888,alloc=image.ALLOC_REF,data=self.masks)
                        img.copy_from(mask_img)
                        dets,ids,scores = res[0],res[1],res[2]
                        for i, det in enumerate(dets):
                            x1, y1, w, h = map(lambda x: int(round(x, 0)), det)
                            img.draw_string_advanced(x1,y1-50,32, " " + self.labels[int(ids[i])] + " {0:.3f}".format(scores[i]) , color=self.colors[int(ids[i])])
                    else:
                        img.clear()
                elif self.task_type=="obb":
                    if res:
                        img.clear()
                        for i in range(len(res[0])):
                            x1, y1, x2,y2,x3,y3,x4,y4 = map(lambda x: int(round(x, 0)), res[0][i])
                            img.draw_line(int(x1),int(y1),int(x2),int(y2),color=self.colors[res[1][i]],thickness=4)
                            img.draw_line(int(x2),int(y2),int(x3),int(y3),color=self.colors[res[1][i]],thickness=4)
                            img.draw_line(int(x3),int(y3),int(x4),int(y4),color=self.colors[res[1][i]],thickness=4)
                            img.draw_line(int(x4),int(y4),int(x1),int(y1),color=self.colors[res[1][i]],thickness=4)
                            img.draw_string_advanced(x1, y1,24,str(res[1][i]) , color=self.colors[res[1][i]])
                    else:
                        img.clear()
                else:
                    pass
            elif self.mode=="image":
                if self.task_type=="classify":
                    ids,score=res[0],res[1]
                    if ids!=-1:
                        mes=self.labels[ids]+" {0:.3f}".format(score)
                        img.draw_string_advanced(5,5,32,mes,color=(0,255,0))
                    img.compress_for_ide()
                elif self.task_type=="detect":
                    if res:
                        for i in range(len(res[0])):
                            x, y, w, h = map(lambda x: int(round(x, 0)), res[0][i])
                            img.draw_rectangle(x,y, w, h, color=self.colors[res[1][i]],thickness=4)
                            img.draw_string_advanced( x , y-50,32," " + self.labels[res[1][i]] + " {0:.3f}".format(res[2][i]) , color=self.colors[res[1][i]])
                    img.compress_for_ide()
                elif self.task_type=="segment":
                    if res[0]:
                        mask_rgb=self.masks[0,:,:,1:4]
                        mask_img=image.Image(self.rgb888p_size[0], self.rgb888p_size[1], image.RGB888,alloc=image.ALLOC_REF,data=mask_rgb.copy())
                        dets,ids,scores = res[0],res[1],res[2]
                        for i, det in enumerate(dets):
                            x, y, w, h = map(lambda x: int(round(x, 0)), det)
                            mask_img.draw_string_advanced(x,y-50,32, " " + self.labels[int(ids[i])] + " {0:.3f}".format(scores[i]) , color=self.colors[int(ids[i])])
                        mask_img.compress_for_ide()
                elif self.task_type=="obb":
                    if res:
                        for i in range(len(res[0])):
                            x1, y1, x2,y2,x3,y3,x4,y4 = map(lambda x: int(round(x, 0)), res[0][i])
                            img.draw_line(int(x1),int(y1),int(x2),int(y2),color=self.colors[res[1][i]],thickness=4)
                            img.draw_line(int(x2),int(y2),int(x3),int(y3),color=self.colors[res[1][i]],thickness=4)
                            img.draw_line(int(x3),int(y3),int(x4),int(y4),color=self.colors[res[1][i]],thickness=4)
                            img.draw_line(int(x4),int(y4),int(x1),int(y1),color=self.colors[res[1][i]],thickness=4)
                            img.draw_string_advanced(x1, y1,24,str(res[1][i]) , color=self.colors[res[1][i]])
                    img.compress_for_ide()
                else:
                    pass
```

### `postprocess()` 决定了 `res` 的结构

源码显示：

```
def postprocess(self, results):
    if self.task_type=="classify":
        ...
        return cls_res
    elif self.task_type=="detect":
        ...
        return det_res
    elif self.task_type=="segment":
        ...
        return seg_res
    elif self.task_type=="obb":
        ...
        return obb_res
```

所以 `res` 的内容就是：

- `task_type="classify"` → `res = cls_res`
- `task_type="detect"` → `res = det_res`
- `task_type="segment"` → `res = seg_res`
- `task_type="obb"` → `res = obb_res`

------

### 各类型 `res` 的结构

#### (1) 分类 (`classify`)

```
cls_res = (-1, 0.0)  # 默认没有检测到
# 如果有类别置信度 > conf_thresh:
cls_res = (res_idx, max_score)
```

- 返回一个 **二元组** `(类别索引, 置信度)`
- 例如 `(3, 0.92)` → 第 3 类，置信度 0.92

------

#### (2) 检测 (`detect`)

```
det_res = aidemo.yolov8_det_postprocess(...)
```

- 这是调用 `aidemo` 里的函数处理模型输出得到的结果
- 从 `draw_result()` 的使用方式可以看出：

```
for i in range(len(res[0])):
    x, y, w, h = res[0][i]   # 框的位置
    label_id = res[1][i]     # 类别索引
    score = res[2][i]        # 置信度
```

- 所以 `det_res` 是一个 **三元素列表**：

```
res = [
    [[x1,y1,w1,h1], [x2,y2,w2,h2], ...],  # 检测框
    [label1, label2, ...],                # 类别索引
    [score1, score2, ...]                 # 置信度
]
```

------

#### (3) 分割 (`segment`)

- `seg_res` 同样是三元素列表：

```
dets, ids, scores = res[0], res[1], res[2]
```

- 另外还会有一个 **mask**，存储分割结果图像：`self.masks`
- `res[0]` → 检测框
- `res[1]` → 类别索引
- `res[2]` → 置信度

------

#### (4) OBB（旋转框检测）

- 和检测类似，但每个框是四个顶点：

```
x1,y1,x2,y2,x3,y3,x4,y4 = res[0][i]
label_id = res[1][i]
```

- 所以 `obb_res` 结构：

```
res = [
    [[x1,y1,x2,y2,x3,y3,x4,y4], ...],  # 每个旋转框四个点
    [label1, label2, ...],              # 类别索引
]
```

### 修改模型输入尺寸

在创建 YOLO11 对象时，可以指定：

```
yolo = YOLO11(model_input_size=[640,640])   #默认是320*320
```

- 这会把模型的输入固定为 640x640
- 同时，预处理 resize、padding 也会自动匹配

> 输入必须是 **固定尺寸**（H×W），因为卷积后的特征图需要确定的大小来生成预测框。
>
> 通常 H 和 W **可以不同**，例如可以是 640×480 或 320×224，但很多实现都用 1:1（正方形），方便训练和推理。
>
> 注意：如果模型权重本身训练时是 320x320，改输入尺寸可能会影响精度。