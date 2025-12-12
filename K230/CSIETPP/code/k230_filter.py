from libs.PipeLine import PipeLine
from libs.YOLO import YOLO11
from libs.Utils import *
from media.display import *
import os,sys,gc
import ulab.numpy as np
import image
import utime
import machine
#from scipy.optimize import linear_sum_assignment
def iou_matrix(boxes1, boxes2):
    """
    boxes1: N x 4  => [x1,y1,x2,y2]
    boxes2: M x 4
    return: N x M IOU matrix
    """
    import ulab.numpy as np

    n = len(boxes1)
    m = len(boxes2)

    iou = np.zeros((n, m))

    for i in range(n):
        x1a, y1a, x2a, y2a = boxes1[i]
        area_a = (x2a - x1a) * (y2a - y1a)
        for j in range(m):
            x1b, y1b, x2b, y2b,score2 = boxes2[j]
            area_b = (x2b - x1b) * (y2b - y1b)
            # 交集
            xx1 = max(x1a, x1b)
            yy1 = max(y1a, y1b)
            xx2 = min(x2a, x2b)
            yy2 = min(y2a, y2b)

            w = max(0, xx2 - xx1)
            h = max(0, yy2 - yy1)
            inter = w * h

            union = area_a + area_b - inter
            if union > 0:
                iou[i, j] = inter / union
            else:
                iou[i, j] = 0

    return iou
'''
def draw_id_on_screen(tid, x1, y1, x2, y2, osd_layer_img):
    """
    tid: track id
    x1,y1,x2,y2: bbox
    osd_layer_img: pl.osd_img
    """
    # 1. 绘制矩形框
    color = (255, 0, 0)  # 红色
    width = x2 - x1
    height = y2 - y1
    osd_layer_img.draw_rectangle(x1, y1, width, height, color)

    # 2. 绘制 ID 文字
    text = f"ID:{tid}"
    osd_layer_img.draw_string_advanced(x1, y1 - 12, 12, text, color=color)
'''

def draw_id_on_osd(tid, x1, y1, x2, y2, osd_img):
    # 绘制矩形框
    color = (255, 0, 0, 255)  # ARGB8888
    osd_img.draw_rectangle(x1, y1, x2-x1, y2-y1, color)
    # 绘制文字
    text = f"ID:{tid}"
    osd_img.draw_string_advanced(x1, y1-12, 16, text, color=color)

def linear_sum_assignment(cost_matrix):
    """
    极简匈牙利算法实现，适配 SORT
    cost_matrix: N x M 矩阵（numpy/ulab array）
    return: row_ind, col_ind
    """

    n, m = cost_matrix.shape
    row_ind = []
    col_ind = []

    # 贪心匹配（最小值匹配）
    used_rows = set()
    used_cols = set()

    flat = []
    for i in range(n):
        for j in range(m):
            flat.append((cost_matrix[i,j], i, j))

    flat.sort(key=lambda x: x[0])  # 按 cost 从小到大排序

    for c, i, j in flat:
        if i not in used_rows and j not in used_cols:
            row_ind.append(i)
            col_ind.append(j)
            used_rows.add(i)
            used_cols.add(j)

    return row_ind, col_ind

class KalmanFilter:
    def __init__(self, dt=0.01):
        self.dt = dt
        dim_x = 7   # [cx, cy, s, r, vx, vy, vs]
        dim_z = 4   # [cx, cy, s, r]

        self.x = np.zeros(dim_x)
        self.P = np.eye(dim_x) * 10
        self.Q = np.eye(dim_x) * 0.01
        self.R = np.eye(dim_z) * 1

        # 状态转移矩阵 F
        self.F = np.eye(dim_x)
        self.F[0, 4] = dt  # cx += vx * dt
        self.F[1, 5] = dt  # cy += vy * dt
        self.F[2, 6] = dt  # s += vs * dt

        # 观测矩阵 H
        self.H = np.zeros((dim_z, dim_x))
        for i in range(4):
            self.H[i, i] = 1

    def initiate(self, bbox):
        x1, y1, x2, y2, score = bbox
        w = x2 - x1
        h = y2 - y1
        cx = x1 + w/2
        cy = y1 + h/2
        s = w * h
        r = w / (h + 1e-6)

        self.x[0] = cx
        self.x[1] = cy
        self.x[2] = s
        self.x[3] = r
        self.x[4] = 0  # vx
        self.x[5] = 0  # vy
        self.x[6] = 0  # vs

    def predict(self):
        self.x = np.dot(self.F, self.x)
        self.P = np.dot(np.dot(self.F, self.P), self.F.T) + self.Q

    def update(self, bbox):
        x1, y1, x2, y2, score = bbox
        w = x2 - x1
        h = y2 - y1
        cx = x1 + w/2
        cy = y1 + h/2
        s = w * h
        r = w / (h + 1e-6)

        z = np.array([cx, cy, s, r])

        # 卡尔曼增益
        S = np.dot(np.dot(self.H, self.P), self.H.T) + self.R
        K = np.dot(np.dot(self.P, self.H.T), np.linalg.inv(S))

        # 更新
        y = z - np.dot(self.H, self.x)
        self.x = self.x + np.dot(K, y)
        KH = np.dot(K, self.H)
        I = np.eye(len(self.x))
        self.P = np.dot(I - KH, self.P)

    def get_state(self):
        cx, cy, s, r = self.x[0], self.x[1], self.x[2], self.x[3]
        w = np.sqrt(s * r)
        h = s / (w + 1e-6)
        x1 = cx - w/2
        y1 = cy - h/2
        x2 = cx + w/2
        y2 = cy + h/2
        return [int(x1), int(y1), int(x2), int(y2)]

class KalmanBoxTracker:
    count = 0

    def __init__(self, bbox):
        self.kf = KalmanFilter()
        self.kf.initiate(bbox)

        self.time_since_update = 0
        self.id = KalmanBoxTracker.count
        KalmanBoxTracker.count += 1

        self.hits = 1
        self.hit_streak = 1
        self.age = 0

    def update(self, bbox):
        self.time_since_update = 0
        self.hits += 1
        self.hit_streak += 1
        self.kf.update(bbox)

    def predict(self):
        self.kf.predict()
        self.age += 1
        if self.time_since_update > 0:
            self.hit_streak = 0
        self.time_since_update += 1
        return self.kf.get_state()



class Sort(object):
    def __init__(self, max_age=30, min_hits=3, iou_threshold=0.3):
        self.max_age = max_age
        self.min_hits = min_hits
        self.iou_threshold = iou_threshold
        self.trackers = []

    def update(self, dets):
        """
        dets: [[x1,y1,x2,y2,score], ...]
        """

        # --- 0. 空检测的处理（最关键的修复） ---
        if dets is None or len(dets) == 0:
        # 所有跟踪器只执行 predict，不做匹配
            predictions = []
            for t in self.trackers:
                predictions.append(t.predict())

        # 删除超时轨迹
            self.trackers = [t for t in self.trackers if t.time_since_update <= self.max_age]

        # 返回结果
            results = []
            for t in self.trackers:
                if t.hits >= self.min_hits:
                    results.append((t.id, t.kf.get_state()))
            return results


    # --- 1. 预测所有轨迹 ---
        predictions = []
        for t in self.trackers:
            predictions.append(t.predict())

    # 如果当前还没有轨迹，直接创建
        if len(predictions) == 0:

            for d in dets:

                self.trackers.append(KalmanBoxTracker(d))

            return [(t.id, t.kf.get_state()) for t in self.trackers]


    # --- 2. 计算 IOU ---
        iou_mat = iou_matrix(predictions, dets)

    # --- 3. 构造 cost 矩阵 ---
        cost = 1 - iou_mat

    # --- 4. 匈牙利匹配 ---
        row, col = linear_sum_assignment(cost)

        matches = []
        unmatched_trk = list(range(len(self.trackers)))
        unmatched_det = list(range(len(dets)))

        for r, c in zip(row, col):
            if cost[r, c] <= (1 - self.iou_threshold):
                matches.append((r, c))
                unmatched_trk.remove(r)
                unmatched_det.remove(c)

    # --- 5. 更新已有轨迹 ---
        for t, d in matches:
            self.trackers[t].update(dets[d])

    # --- 6. 为未匹配到的检测框建立新轨迹 ---
        for d in unmatched_det:
            self.trackers.append(KalmanBoxTracker(dets[d]))

    # --- 7. 移除长时间未匹配 ---
        self.trackers = [t for t in self.trackers if t.time_since_update <= self.max_age]

    # --- 8. 返回结果 ---
        results = []
        for t in self.trackers:
            if t.hits >= self.min_hits:
                results.append((t.id, t.kf.get_state()))
        return results

def parse_res_to_dets(res):
    dets = []  # 最终输出 [[x1,y1,x2,y2,score], ...]

    if not res or len(res) < 3:
        return dets

    boxes = res[0]
    class_ids = res[1]
    scores = res[2]

    # 安全长度检查，取最小长度以防万一
    n_boxes = len(boxes)
    print(n_boxes)

    n_scores = len(scores)
    n_cls = len(class_ids)
    n = min(n_boxes, n_scores, n_cls)

    for i in range(n):
        box = boxes[i]
        print(box[0])
        # box 可能是 ulab ndarray 或 list/tuple
        try:
            x = int(box[0])
            y = int(box[1])
            w = int(box[2])
            h = int(box[3])
        except Exception:
            # 无法解析该框，跳过
            continue

        # 把 xywh -> x1,y1,x2,y2
        x1 = x
        y1 = y
        x2 = x + w
        y2 = y + h

        score = float(scores[i]) if scores[i] is not None else 1.0
        cls_id = int(class_ids[i]) if class_ids[i] is not None else -1

        dets.append([x1, y1, x2, y2, score])  # 你可以保留 class_id 作为第6列
        print(dets)
    return dets




tracker = Sort(max_age=30, min_hits=3, iou_threshold=0.3)

if __name__=="__main__":
    try:
        # 这里仅为示例，自定义场景请修改为您自己的模型路径、标签名称、模型输入大小
        kmodel_path="/data/tools.kmodel"
        labels = ["drill","strippers","pliers"]
        '''
        labels = [
            "person", "bicycle", "car", "motorcycle", "airplane", "bus", "train", "truck", "boat",
            "traffic light", "fire hydrant", "stop sign", "parking meter", "bench", "bird", "cat",
            "dog", "horse", "sheep", "cow", "elephant", "bear", "zebra", "giraffe", "backpack",
            "umbrella", "handbag", "tie", "suitcase", "frisbee", "skis", "snowboard", "sports ball",
            "kite", "baseball bat", "baseball glove", "skateboard", "surfboard", "tennis racket",
            "bottle", "wine glass", "cup", "fork", "knife", "spoon", "bowl", "banana", "apple",
            "sandwich", "orange", "broccoli", "carrot", "hot dog", "pizza", "donut", "cake", "chair",
            "couch", "potted plant", "bed", "dining table", "toilet", "tv", "laptop", "mouse", "remote",
            "keyboard", "cell phone", "microwave", "oven", "toaster", "sink", "refrigerator", "book",
            "clock", "vase", "scissors", "teddy bear", "hair drier", "toothbrush"
        ]
        '''
        model_input_size=[320,320] # 该参数必须与训练使用的imgsz参数保持一致，coco是640，水果是224，其他是320

        # 添加显示模式，默认hdmi，可选hdmi/lcd/lt9611/st7701/hx8399,其中hdmi默认置为lt9611，分辨率1920*1080；lcd默认置为st7701，分辨率800*480
        display_mode="lcd"
        rgb888p_size=[1920,1080]
        confidence_threshold = 0.5
        nms_threshold=0.45
        # 初始化PipeLine
        pl=PipeLine(rgb888p_size=rgb888p_size,display_mode=display_mode,osd_layer_num=2)
        pl.create()
        display_size=pl.get_display_size()
        # 初始化YOLO11实例
        yolo=YOLO11(task_type="detect",mode="video",
                    kmodel_path=kmodel_path,labels=labels,
                    rgb888p_size=rgb888p_size,
                    model_input_size=model_input_size,
                    display_size=display_size,
                    conf_thresh=confidence_threshold,
                    nms_thresh=nms_threshold,
                    max_boxes_num=50,debug_mode=0)
        yolo.config_preprocess()
        fps_img = image.Image(pl.display_size[0], pl.display_size[1], image.ARGB8888)
        last_time=utime.ticks_us()
        cur_time=utime.ticks_us()
        delta_time=(cur_time-last_time)/1000
        while True:
            os.exitpoint()
            cur_time=utime.ticks_us()
            delta_time=(cur_time-last_time)/1000
            last_time=cur_time


            # 逐帧推理
            img=pl.get_frame()
            res=yolo.run(img)

            # --- 解析 res（兼容多种返回形式） ---
            dets = parse_res_to_dets(res)

            tracked = tracker.update(dets)


            yolo.draw_result(res,pl.osd_img)

            fps_img.clear()
            fps_img.draw_string_advanced(0, 0, 32, f"{machine.temperature():.2f} C", color=(255, 0, 0))
            fps_img.draw_string_advanced(0, 32, 32, f"{delta_time:.2f} ms", color=(255, 0, 0))
            for tid, bbox in tracked:
                x1, y1, x2, y2 = bbox
                draw_id_on_osd(tid, x1, y1, x2, y2, fps_img)
            Display.show_image(fps_img, 0, 0, Display.LAYER_OSD2)


            pl.show_image()
            gc.collect()
    except KeyboardInterrupt as e:
        print("用户停止: ", e)
    except BaseException as e:
        print(f"异常: {e}")
    finally:
        yolo.deinit()
        pl.destroy()
