from PyCameraList.camera_device import list_video_devices

cameras = list_video_devices()
print(dict(cameras))