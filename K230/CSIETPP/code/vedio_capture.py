from media.vencoder import *
from media.sensor import *
from media.media import *
from media.display import *
import time, os

def vi_bind_venc_test(file_name, width=1280, height=720):
    print("VENC 测试开始")
    venc_chn = VENC_CHN_ID_0
    width = ALIGN_UP(width, 16)
    venc_payload_type = K_PT_H265

    # 初始化传感器
    sensor = Sensor()
    sensor.reset()
    sensor.set_framesize(width=width, height=height, alignment=12)
    sensor.set_pixformat(Sensor.YUV420SP)

    sensor.set_framesize(width=800, height=480, chn=CAM_CHN_ID_1)
    sensor.set_pixformat(Sensor.YUV420SP, chn=CAM_CHN_ID_1)
    Display.init(Display.ST7701, width=800, height=480, to_ide=True)
    sensor_bind_info = sensor.bind_info(x = 0, y = 0, chn = CAM_CHN_ID_1)
    Display.bind_layer(**sensor_bind_info, layer = Display.LAYER_VIDEO1)



    # 实例化视频编码器
    encoder = Encoder()
    encoder.SetOutBufs(venc_chn, 8, width, height)

    # 绑定camera和venc
    link = MediaManager.link(sensor.bind_info()['src'], (VIDEO_ENCODE_MOD_ID, VENC_DEV_ID, venc_chn))

    # init media manager
    MediaManager.init()

    if (venc_payload_type == K_PT_H264):
        chnAttr = ChnAttrStr(encoder.PAYLOAD_TYPE_H264, encoder.H264_PROFILE_MAIN, width, height)
    elif (venc_payload_type == K_PT_H265):
        chnAttr = ChnAttrStr(encoder.PAYLOAD_TYPE_H265, encoder.H265_PROFILE_MAIN, width, height)

    streamData = StreamData()

    # 创建编码器
    encoder.Create(venc_chn, chnAttr)

    # 开始编码
    encoder.Start(venc_chn)
    # 启动camera
    sensor.run()

    frame_count = 0
    print("save stream to file: ", file_name)

    with open(file_name, "wb") as fo:
        try:
            while True:
                os.exitpoint()
                encoder.GetStream(venc_chn, streamData) # 获取一帧码流

                for pack_idx in range(0, streamData.pack_cnt):
                    stream_data = uctypes.bytearray_at(streamData.data[pack_idx], streamData.data_size[pack_idx])
                    fo.write(stream_data) # 码流写文件
                    print("stream size: ", streamData.data_size[pack_idx], "stream type: ", streamData.stream_type[pack_idx])

                encoder.ReleaseStream(venc_chn, streamData) # 释放一帧码流

                frame_count += 1
                if frame_count >= 2000:
                    break
        except KeyboardInterrupt as e:
            print("user stop: ", e)
        except BaseException as e:
            import sys
            sys.print_exception(e)

    # 停止camera
    sensor.stop()

    Display.deinit()
    # 销毁camera和venc的绑定
    del link
    # 停止编码
    encoder.Stop(venc_chn)
    # 销毁编码器
    encoder.Destroy(venc_chn)
    # 清理buffer
    MediaManager.deinit()
    print("venc_test stop")
image_folder = "/data/video"

try:
    os.stat(image_folder)  # 尝试获取目录信息
except OSError:
    os.mkdir(image_folder)  # 若失败则创建该目录

if __name__ == "__main__":
    os.exitpoint(os.EXITPOINT_ENABLE)
    vi_bind_venc_test("/data/video/test.mp4",1920,1080)  # vi绑定venc示例
