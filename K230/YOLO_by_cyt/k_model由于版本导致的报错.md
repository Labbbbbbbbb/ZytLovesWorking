# k_model.py由于版本导致的报错：

added by zyt）

```
  File "F:\ITS_Log\K230\CSIETPP\yolo11\test_yolo11\classify\to_kmodel.py", line 24, in parse_model_input_output
    input_dict['dtype'] = onnx.mapping.TENSOR_TYPE_TO_NP_TYPE[onnx_type.elem_type]
AttributeError: module 'onnx' has no attribute 'mapping'
```

```
python to_kmodel.py --target k230 --model ../../runs/classify/train/weights/best.onnx --dataset ../test --input_width 224 --input_height 224 --ptq_option 0
WARNING: The argument `input_shapes` is deprecated. Please use `overwrite_input_shapes` and/or `test_input_shapes`
instead. An error will be raised in the future.
```





关于mapping的问题：由于onnx1.19没有这个属性，所以可以自己加一组键值对映射，然后把原来24行那句改掉

```
ONNX_DTYPE_MAP = {
    1: np.float32,
    2: np.uint8,
    3: np.int8,
    4: np.uint16,
    5: np.int16,
    6: np.int32,
    7: np.int64,
    9: np.bool_,
    10: np.float16,
    11: np.float64,   # double
}

#原来的24行mapping那句
input_dict['dtype'] = ONNX_DTYPE_MAP.get(onnx_type.elem_type, np.float32)

```

关于input_shapes:

```
onnx_model, check = onnxsim.simplify(onnx_model, input_shapes=input_shapes)

---改成--->>>>

onnx_model, check = onnxsim.simplify(onnx_model, overwrite_input_shapes=input_shapes)
```



转kmodel的时候如果遇到这个：

```
  File "F:\Anaconda3\envs\CSIETPP\lib\site-packages\onnxsim\onnx_simplifier.py", line 199, in simplify
    model_opt_bytes = C.simplify(
RuntimeError: Nodes in a graph must be topologically sorted, however input '/model.23/Mul_output_0' of node:
name: /model.23/Slice OpType: Slice
 is not output of any previous nodes.
```

需要重新导出onnx并在这个命令后面加一个opset=17

```
 yolo export model=runs/detect/train5/weights/best.pt format=onnx imgsz=320 opset=17
```

导出新的onnx后再重新转kmodel



Classify完整可用的kmodel如下：detect不要直接套

```
import os
import argparse
import numpy as np
from PIL import Image
import onnxsim
import onnx
import nncase
import shutil


ONNX_DTYPE_MAP = {
    1: np.float32,
    2: np.uint8,
    3: np.int8,
    4: np.uint16,
    5: np.int16,
    6: np.int32,
    7: np.int64,
    9: np.bool_,
    10: np.float16,
    11: np.float64,   # double
}

def parse_model_input_output(model_file,input_shape):
    onnx_model = onnx.load(model_file)
    input_all = [node.name for node in onnx_model.graph.input]
    input_initializer = [node.name for node in onnx_model.graph.initializer]
    input_names = list(set(input_all) - set(input_initializer))
    input_tensors = [
        node for node in onnx_model.graph.input if node.name in input_names]

    # input
    inputs = []
    for _, e in enumerate(input_tensors):
        onnx_type = e.type.tensor_type
        input_dict = {}
        input_dict['name'] = e.name
        input_dict['dtype'] = ONNX_DTYPE_MAP.get(onnx_type.elem_type, np.float32)
        input_dict['shape'] = [(i.dim_value if i.dim_value != 0 else d) for i, d in zip(
            onnx_type.shape.dim, input_shape)]
        inputs.append(input_dict)

    return onnx_model, inputs


def onnx_simplify(model_file, dump_dir,input_shape):
    onnx_model, inputs = parse_model_input_output(model_file,input_shape)
    onnx_model = onnx.shape_inference.infer_shapes(onnx_model)
    input_shapes = {}
    for input in inputs:
        input_shapes[input['name']] = input['shape']

    onnx_model, check = onnxsim.simplify(onnx_model, overwrite_input_shapes=input_shapes)
    assert check, "Simplified ONNX model could not be validated"

    model_file = os.path.join(dump_dir, 'simplified.onnx')
    onnx.save_model(onnx_model, model_file)
    return model_file


def read_model_file(model_file):
    with open(model_file, 'rb') as f:
        model_content = f.read()
    return model_content

def generate_data_ramdom(shape, batch):
    data = []
    for i in range(batch):
        data.append([np.random.randint(0, 256, shape).astype(np.uint8)])
    return data


def generate_data(shape, batch, calib_dir):
    img_paths = [os.path.join(calib_dir, p) for p in os.listdir(calib_dir)]
    data = []
    for i in range(batch):
        assert i < len(img_paths), "calibration images not enough."
        img_data = Image.open(img_paths[i]).convert('RGB')
        img_data = img_data.resize((shape[3], shape[2]), Image.BILINEAR)
        img_data = np.asarray(img_data, dtype=np.uint8)
        img_data = np.transpose(img_data, (2, 0, 1))
        data.append([img_data[np.newaxis, ...]])
    return np.array(data)

def main():
    parser = argparse.ArgumentParser(prog="nncase")
    parser.add_argument("--target", default="k230",type=str, help='target to run,k230/cpu')
    parser.add_argument("--model",type=str, help='model file')
    parser.add_argument("--dataset", type=str, help='calibration_dataset')
    parser.add_argument("--input_width", type=int, default=224, help='input_width')
    parser.add_argument("--input_height", type=int, default=224, help='input_height')
    parser.add_argument("--ptq_option", type=int, default=0, help='ptq_option:0,1,2,3,4,5')

    args = parser.parse_args()

    # 模型的输入shape，维度要跟input_layout一致
    input_shape=[1,3,args.input_height,args.input_width]

    dump_dir = 'tmp'
    if not os.path.exists(dump_dir):
        os.makedirs(dump_dir)

    # onnx simplify
    model_file = onnx_simplify(args.model, dump_dir,input_shape)

    # compile_options
    compile_options = nncase.CompileOptions()
    compile_options.target = args.target
    # preprocess
    # 是否采用模型做预处理
    compile_options.preprocess = True
    compile_options.swapRB = False
    # 输入图像的shape
    compile_options.input_shape = input_shape
    # 模型输入格式‘uint8’或者‘float32’
    compile_options.input_type = 'uint8'

    # 如果输入是‘uint8’格式，输入反量化之后的范围
    compile_options.input_range = [0, 1]

    # 预处理的mean/std值，每个channel一个,这里注意，分类任务和检测任务Norm的参数不同
    compile_options.mean = [0,0,0]  #gray有图像，bgr没有图像
    compile_options.std = [1,1,1]

    # 设置输入的layout，onnx默认‘NCHW’即可
    compile_options.input_layout = "NCHW"
    # compile_options.output_layout = "NCHW"

    # compile_options.dump_ir = True
    # compile_options.dump_asm = True
    # compile_options.dump_dir = dump_dir
    compile_options.quant_type = 'uint8'

    # compiler
    compiler = nncase.Compiler(compile_options)

    # import
    model_content = read_model_file(model_file)
    import_options = nncase.ImportOptions()
    compiler.import_onnx(model_content, import_options)

    # ptq_options
    ptq_options = nncase.PTQTensorOptions()
    ptq_options.samples_count = 20

    if args.ptq_option == 0:
        ptq_options.calibrate_method = 'NoClip'
        ptq_options.w_quant_type = 'uint8'
        ptq_options.quant_type = 'uint8'
    elif args.ptq_option == 1:
        ptq_options.calibrate_method = 'NoClip'
        ptq_options.w_quant_type = 'int16'
        ptq_options.quant_type = 'uint8'
    elif args.ptq_option == 2:
        ptq_options.calibrate_method = 'NoClip'
        ptq_options.w_quant_type = 'uint8'
        ptq_options.quant_type = 'int16'
    elif args.ptq_option == 3:
        ptq_options.calibrate_method = 'Kld'
        ptq_options.w_quant_type = 'uint8'
        ptq_options.quant_type = 'uint8'
    elif args.ptq_option == 4:
        ptq_options.calibrate_method = 'Kld'
        ptq_options.w_quant_type = 'int16'
        ptq_options.quant_type = 'uint8'
    elif args.ptq_option == 5:
        ptq_options.calibrate_method = 'Kld'
        ptq_options.w_quant_type = 'uint8'
        ptq_options.quant_type = 'int16'
    else:
        pass

    #ptq_options.set_tensor_data(generate_data_ramdom(input_shape, ptq_options.samples_count))
    ptq_options.set_tensor_data(generate_data(input_shape, ptq_options.samples_count, args.dataset))
    compiler.use_ptq(ptq_options)

    # compile
    compiler.compile()

    # kmodel
    kmodel = compiler.gencode_tobytes()
    base,ext=os.path.splitext(args.model)
    kmodel_name=base+".kmodel"
    with open(kmodel_name, 'wb') as f:
        f.write(kmodel)

    if os.path.exists("./tmp"):
        shutil.rmtree("./tmp")
    if os.path.exists("./gmodel_dump_dir"):
        shutil.rmtree("./gmodel_dump_dir")

if __name__ == '__main__':
    main()

```

