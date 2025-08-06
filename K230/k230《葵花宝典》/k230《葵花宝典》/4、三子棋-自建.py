# ==================================================
# 一、导入区：库的导入及头文件申明
# ==================================================
import time
import os
import sys
from media.sensor import *
from media.display import *
from media.media import *
from machine import Pin, FPIOA

# ==================================================
# 二、配置区：所有可调参数都在这里
# ==================================================
DISPLAY_MODE      = "LCD"
SENSOR_ID         = 2

# 棋盘矩形检测
RECT_AREA_MIN         = 5000                    # 大小阈值（最小值）
RECT_AREA_MAX         = 70000                   # 配置区可新增此参数（最大值）
RECT_COLOR            = (0, 255, 0)             # 颜色
RECT_THICKNESS        = 2                       # 宽度
RECT_BINARY_THRESHOLD = [(45, 100), (9, 92)]    # 灰度阈值
TARGET_RECT_COUNT     = 9                       # 识别9个矩形
SKIP_FRAMES           = 50                      # 跳过帧
DELAY_SECONDS         = 0.1                     # 跳帧后再延时
STABILIZATION_DELAY   = 2                       # 稳定识别矩形再延时
SHOW_RECT_NUMBERS     = True                    #True or False 是否让矩形标号显示出来

# 棋盘 ROI
ROI_ENABLE = True                               # 是否打开棋盘roi区
ROI_X, ROI_Y = 220, 0                           # 起始坐标
ROI_WIDTH, ROI_HEIGHT = 330, 480                # 宽和高
ROI_COLOR   = (255, 0, 0)                       # 颜色
ROI_THICKNESS = 2                               # 宽度
# 白棋 ROI
WHITE_ROI_X, WHITE_ROI_Y = 100, 0
WHITE_ROI_WIDTH, WHITE_ROI_HEIGHT = 110, 480
WHITE_THRESHOLD   = [(1, 60, 9, -15, 1, -46)]
MAX_WHITE_BLOBS   = 5
BLOB_MARK_COLOR_W = (255, 255, 255)
# 黑棋 ROI
BLACK_ROI_X, BLACK_ROI_Y = 560, 0
BLACK_ROI_WIDTH, BLACK_ROI_HEIGHT = 110, 480
BLACK_THRESHOLD   = [(0, 40, -10, 10, -10, 10)]
MAX_BLACK_BLOBS   = 5
BLOB_MARK_COLOR_B = (0, 0, 255)                  # 白色文字，黑背景看得清
# 共用（黑白棋区域阈值）
BLOB_AREA_THRESHOLD = 3000

# 首次编号后是否冻结左右 ROI 的实时检测（是否取消黑白区的识别）
LOCK_SIDE_BLOBS = True

# 游戏时间
GAME_WHITE_THRESHOLD = [(46, 81, -37, 11, -37, 15)]         # 白棋颜色阈值
GAME_BLACK_THRESHOLD = [(1, 42, -14, 7, -24, 5)]            # 黑棋颜色阈值
GAME_BLOB_AREA       = 3000                                 # 黑白棋大小阈值
board = [0]*9                                               # 0空 1黑 2白，3个状态判断每个格子（空、黑、白）
prev_board = [0]*9                                          # 稳定后的board状态
PRINT_STABLE_FRAMES = 25                                    # 连续帧状态一致才打印-改变帧数稳定识别
WIN_COLOR_WHITE = (255, 255, 255)                           # 胜利文字颜色白
WIN_COLOR_BLACK = (0, 0, 0)                                 # 胜利文字颜色黑
WIN_LINES = [
    [0, 1, 2], [3, 4, 5], [6, 7, 8],          # 横向
    [0, 3, 6], [1, 4, 7], [2, 5, 8],          # 纵向
    [0, 4, 8], [2, 4, 6]                      # 斜向
]
CURRENT_TURN = 1  # 1=黑棋回合，2=白棋回合
DRAW_COLOR = (128, 128, 128)   # 平局颜色

#按键控制
MODE_COUNT = 3          # 只有 2 个模式
current_mode = 1        # 初始模式

# ---------- 模式1：双人博弈 -------------
MODE1_ACTIVE = True  # 仅当 current_mode == 1 时启用

# ---------- 模式2 人机 AI 先行 ----------
AI_FIRST_MOVE      = 4          # 0~8，格子序号，默认放中心（第5格）
AI_DEPTH           = 7          # MinMax 搜索深度（可调）
ai_wait_human = False   # True 表示 AI 已下完，正在等人
ai_waiting_confirm = False   # True 表示 AI 已决策，待落子确认
ai_target_move = None        # 记录 AI 想下的格子（0-8）

# ==================================================
# 三、程序主体
# ==================================================
# ---------- 全局运行状态，保存关键信息和控制流程 ----------
fpioa = FPIOA()
fpioa.set_function(27, FPIOA.GPIO27) # KEY1:通过设置不同的模式
fpioa.set_function(14, FPIOA.GPIO14) # KEY1:通过设置不同的模式
key1 = Pin(27, Pin.IN, Pin.PULL_UP) # PULL_UP表示默认高电平，按下为低电平
key2 = Pin(14, Pin.IN, Pin.PULL_UP) # PULL_UP表示默认高电平，按下为低电平

sensor = None
detected_rects   = None   # 保存识别到的9个矩形
board = [0]*9
stable_detected  = False  # 9个矩形是否稳定识别
roi_regions      = []
first_detected_white_blobs = None   # (编号, (cx, cy))
first_detected_black_blobs = None   # (编号, (cx, cy))
print_stable_buffer = []  # 保存最近 N 帧的 board 状态
winner_shown = 0   # 0 未分胜负，1 黑，2 白，防止重复刷屏
frame_counter = 0


# 防抖机制：每个格子状态连续多少帧一致才算有效
DEBOUNCE_FRAMES = 60
board_debounce = [[0]*DEBOUNCE_FRAMES for _ in range(9)]  # 每格保存最近N帧状态

# ---------- 工具函数 ----------
"""
功能：判断一个矩形是否完全落在棋盘 ROI 内
作用：在第一次识别9个矩形时，过滤掉落在ROI之外的“假矩形”，确保最终只保留棋盘内的9个格子
"""
def is_in_roi(rect):
    for x, y in rect.corners():
        if not (ROI_X <= x <= ROI_X + ROI_WIDTH and ROI_Y <= y <= ROI_Y + ROI_HEIGHT):
            return False
    return True
"""
功能：把任意顺序传入的矩形列表，按“从上到下、从左到右”的顺序重新排序成 3×3 的棋盘顺序
作用：保证格子编号顺序
"""
def sort_rects(rects):
    if not rects:
        return []
    rects_with_center = [(sum(p[0] for p in r.corners()) / 4,
                          sum(p[1] for p in r.corners()) / 4, r)
                         for r in rects]
    rects_with_center.sort(key=lambda t: t[1])
    rows, row, last_y = [], [], None
    for cx, cy, r in rects_with_center:
        if last_y is None or abs(cy - last_y) < 50:
            row.append((cx, cy, r))
        else:
            rows.append(row); row = [(cx, cy, r)]
        last_y = cy
    if row:
        rows.append(row)
    sorted_rects = []
    for r in rows:
        r.sort(key=lambda t: t[0])
        sorted_rects.extend([t[2] for t in r])
    return sorted_rects
"""
功能：把find_rects返回的矩形对象转换成标准的(x, y, w, h)矩形格式
作用：生成后续棋子检测时所需的9个小ROI区域方便对每个格子独立判断是否有黑棋或白棋
"""
def get_roi_from_rect(rect):
    xs, ys = zip(*rect.corners())
    return int(min(xs)), int(min(ys)), int(max(xs) - min(xs)), int(max(ys) - min(ys))
def check_winner(brd):
    """返回 1 黑胜，2 白胜，0 未分胜负"""
    for line in WIN_LINES:
        a, b, c = line
        if brd[a] == brd[b] == brd[c] and brd[a] != 0:
            return brd[a]
    return 0
def scan_key_mode(key_pin, max_mode, now_mode):
    """
    key_pin   : Pin 对象
    max_mode  : 最大模式号
    now_mode  : 当前模式号（int）
    return    : 新的模式号
    """
    if key_pin.value() == 0:           # 按下为低
        time.sleep_ms(20)              # 消抖
        if key_pin.value() == 0:       # 再次确认
            now_mode += 1
            if now_mode > max_mode:
                now_mode = 1
            print("模式切换 ->", now_mode)
            while key_pin.value() == 0: # 等待松手
                pass
    return now_mode
# ---------- MinMax + α-β ----------
def is_terminal(state):
    return check_winner(state) != 0 or 0 not in state

def evaluate(state):
    score = 0
    for a, b, c in WIN_LINES:
        line = [state[a], state[b], state[c]]
        black = line.count(1)
        white = line.count(2)
        empty = line.count(0)

        # ✅ AI（黑）三连 → 胜利
        if black == 3:
            return 100000

        # ✅ 人（白）三连 → AI失败
        if white == 3:
            return -100000

        # ✅ AI 两步内可胜（进攻）
        if black == 2 and empty == 1:
            score += 7000
        elif black == 1 and empty == 2:
            score += 300

        # ✅ 人两步内可胜（必须封堵）
        if white == 2 and empty == 1:
            score -= 10000  # 🔴 封堵优先级必须远高于进攻
        elif white == 1 and empty == 2:
            score -= 200

    return score

def minimax(state, depth, alpha, beta, maximizing):
    if depth == 0 or is_terminal(state):
        return evaluate(state), None

    best_move = None
    if maximizing:      # AI 走黑子
        max_eval = -float('inf')
        for i in range(9):
            if state[i] == 0:
                state[i] = 1
                val, _ = minimax(state, depth-1, alpha, beta, False)
                state[i] = 0
                if val > max_eval:
                    max_eval, best_move = val, i
                alpha = max(alpha, val)
                if beta <= alpha:
                    break
        return max_eval, best_move
    else:               # 人走白子
        min_eval = float('inf')
        for i in range(9):
            if state[i] == 0:
                state[i] = 2
                val, _ = minimax(state, depth-1, alpha, beta, True)
                state[i] = 0
                if val < min_eval:
                    min_eval, best_move = val, i
                beta = min(beta, val)
                if beta <= alpha:
                    break
        return min_eval, best_move






# ---------- 主循环级函数 ----------
"""
功能：首次识别并稳定 9 个棋盘矩形
"""
def detect_rectangles_once(img):
    global detected_rects, roi_regions, stable_detected
    if not stable_detected:
        img_gray = img.to_grayscale(copy=True).binary(RECT_BINARY_THRESHOLD)
        all_rects = img_gray.find_rects(threshold=RECT_AREA_MIN)
        all_rects = [r for r in all_rects if RECT_AREA_MIN <= r.w() * r.h() <= RECT_AREA_MAX]
        roi_rects = [r for r in all_rects if not ROI_ENABLE or is_in_roi(r)]
        if len(roi_rects) == TARGET_RECT_COUNT:
            detected_rects = sort_rects(roi_rects)
            roi_regions = [get_roi_from_rect(r) + (i + 1,) for i, r in enumerate(detected_rects)]
            print(f"检测到 {TARGET_RECT_COUNT} 个矩形，稳定 {STABILIZATION_DELAY}s...")
            time.sleep(STABILIZATION_DELAY)
            stable_detected = True
            for x, y, w, h, n in roi_regions:
                            cx = x + w // 2
                            cy = y + h // 2
                            print(f"格子{n}中心: ({cx},{cy})")
"""
功能：画棋盘ROI和9个矩形
"""
def draw_rectangles_and_roi(img):
    if ROI_ENABLE:
        img.draw_rectangle(ROI_X, ROI_Y, ROI_WIDTH, ROI_HEIGHT,
                           color=ROI_COLOR, thickness=ROI_THICKNESS)
    for i, r in enumerate(detected_rects or []):
        corners = r.corners()
        for j in range(4):
            p1, p2 = corners[j], corners[(j + 1) & 3]
            img.draw_line(p1[0], p1[1], p2[0], p2[1],
                          color=RECT_COLOR, thickness=RECT_THICKNESS)
        if SHOW_RECT_NUMBERS:
            cx = sum(p[0] for p in corners) // 4
            cy = sum(p[1] for p in corners) // 4
            img.draw_string_advanced(cx - 10, cy - 10, 30,
                                     str(i + 1), color=(255, 0, 255))
"""
功能：首次检测并冻结左右棋子编号
"""
def freeze_side_blobs(img):
    global first_detected_white_blobs, first_detected_black_blobs
    # 白棋
    if first_detected_white_blobs is None:
        white_roi = img.copy(roi=(WHITE_ROI_X, WHITE_ROI_Y,
                                  WHITE_ROI_WIDTH, WHITE_ROI_HEIGHT))
        white_raw = white_roi.find_blobs(WHITE_THRESHOLD,
                                         area_threshold=BLOB_AREA_THRESHOLD)
        white_blobs = [{'cx': b.cx() + WHITE_ROI_X,
                        'cy': b.cy() + WHITE_ROI_Y} for b in white_raw]
        if white_blobs:
            white_blobs.sort(key=lambda b: b['cy'])
            white_blobs = white_blobs[:MAX_WHITE_BLOBS]
            first_detected_white_blobs = [(i + 1, (b['cx'], b['cy']))
                                          for i, b in enumerate(white_blobs)]
            print("白棋编号完成:", [n for n, _ in first_detected_white_blobs])
            print("白棋中心坐标:")
            for b in white_blobs:
                print(f"  ({b['cx']}, {b['cy']})")
    else:
        for num, (cx, cy) in first_detected_white_blobs:
            img.draw_string_advanced(cx - 10, cy - 20, 30, str(num),
                                     color=BLOB_MARK_COLOR_W)
    # 黑棋
    if first_detected_black_blobs is None:
        black_roi = img.copy(roi=(BLACK_ROI_X, BLACK_ROI_Y,
                                  BLACK_ROI_WIDTH, BLACK_ROI_HEIGHT))
        black_raw = black_roi.find_blobs(BLACK_THRESHOLD,
                                         area_threshold=BLOB_AREA_THRESHOLD)
        black_blobs = [{'cx': b.cx() + BLACK_ROI_X,
                        'cy': b.cy() + BLACK_ROI_Y} for b in black_raw]
        if black_blobs:
            black_blobs.sort(key=lambda b: b['cy'])
            black_blobs = black_blobs[:MAX_BLACK_BLOBS]
            first_detected_black_blobs = [(i + 1, (b['cx'], b['cy']))
                                          for i, b in enumerate(black_blobs)]
            print("黑棋编号完成:", [n for n, _ in first_detected_black_blobs])
            print("黑棋中心坐标:")
            for b in black_blobs:
                print(f"  ({b['cx']}, {b['cy']})")
    else:
        for num, (cx, cy) in first_detected_black_blobs:
            img.draw_string_advanced(cx - 10, cy - 20, 30, str(num),
                                     color=BLOB_MARK_COLOR_B)
"""
功能：检测9个格子的黑白棋子并更新，只能由0到1或2
"""
def update_board_state(img):
    global board
    if not stable_detected:
        return

    temp_board = [0] * 9
    board_roi = img.copy(roi=(ROI_X, ROI_Y, ROI_WIDTH, ROI_HEIGHT))

    # 检测黑棋 → 1
    for b in board_roi.find_blobs(GAME_BLACK_THRESHOLD, area_threshold=GAME_BLOB_AREA):
        cx, cy = ROI_X + b.cx(), ROI_Y + b.cy()
        for idx, (x, y, w, h, _) in enumerate(roi_regions):
            if x <= cx < x + w and y <= cy < y + h:
                temp_board[idx] = 1
                img.draw_cross(cx, cy, size=12, color=(0, 0, 255), thickness=2)

    # 检测白棋 → 2
    for b in board_roi.find_blobs(GAME_WHITE_THRESHOLD, area_threshold=GAME_BLOB_AREA):
        cx, cy = ROI_X + b.cx(), ROI_Y + b.cy()
        for idx, (x, y, w, h, _) in enumerate(roi_regions):
            if x <= cx < x + w and y <= cy < y + h:
                temp_board[idx] = 2
                img.draw_cross(cx, cy, size=12, color=(255, 255, 255), thickness=2)

    # ✅ 防抖逻辑：每格状态存入缓冲区
    for i in range(9):
        board_debounce[i].pop(0)
        board_debounce[i].append(temp_board[i])

    # ✅ 只有当连续N帧一致才更新
    for i in range(9):
        if board[i] == 0 and all(s == 1 for s in board_debounce[i]):
            board[i] = 1
        elif board[i] == 0 and all(s == 2 for s in board_debounce[i]):
            board[i] = 2
"""
功能：状态变化时打印
"""
def print_board_if_changed():
    global prev_board, print_stable_buffer,CURRENT_TURN

    if not stable_detected:
        return

    # 加入当前状态到缓冲区
    print_stable_buffer.append(board[:])
    if len(print_stable_buffer) > PRINT_STABLE_FRAMES:
        print_stable_buffer.pop(0)

    # 只有当缓冲区满，且所有帧状态一致，才打印一次
    if len(print_stable_buffer) == PRINT_STABLE_FRAMES:
        if all(b == print_stable_buffer[0] for b in print_stable_buffer):
            if board != prev_board:
                moved = False
                for i in range(9):
                    if prev_board[i] == 0 and board[i] == CURRENT_TURN:
                        moved = True
                        break
                if moved:
                    print(f"棋盘状态（稳定）: {board} | {'黑' if CURRENT_TURN==1 else '白'}棋落子")
                    prev_board[:] = board[:]
                    CURRENT_TURN = 2 if CURRENT_TURN == 1 else 1  # 切换回合

# ---------- 主流程 ----------
try:
    print("程序启动...")
    sensor = Sensor(id=SENSOR_ID)
    sensor.reset()

    if DISPLAY_MODE == "LCD":
        DISPLAY_WIDTH, DISPLAY_HEIGHT = 800, 480
        Display.init(Display.ST7701, width=DISPLAY_WIDTH, height=DISPLAY_HEIGHT, to_ide=True)
    sensor.set_framesize(width=DISPLAY_WIDTH, height=DISPLAY_HEIGHT, chn=CAM_CHN_ID_0)
    sensor.set_pixformat(Sensor.RGB565, chn=CAM_CHN_ID_0)

    MediaManager.init()
    sensor.run()
    clock = time.clock()

    board[:] = [0]*9

    if DELAY_SECONDS > 0:
        time.sleep(DELAY_SECONDS)

    for _ in range(SKIP_FRAMES):
        sensor.snapshot(chn=CAM_CHN_ID_0)

    print("进入主循环...")
    while True:
        clock.tick()
        os.exitpoint()
        img = sensor.snapshot(chn=CAM_CHN_ID_0)

        current_mode = scan_key_mode(key1, MODE_COUNT, current_mode)
        detect_rectangles_once(img)         # 首次识别并稳定 9 个棋盘矩形
        draw_rectangles_and_roi(img)        # 画棋盘ROI和9个矩形
        freeze_side_blobs(img)              # 首次检测并冻结左右棋子编号
        update_board_state(img)             # 检测 9 个格子的黑白棋子并更新 board
        print_board_if_changed()            # 状态变化时打印



        # ---------------- 模式 1：双人对战 ----------------
        if current_mode == 1:
            img.draw_string_advanced(305, 35, 35, "双人对战", color=(0,255,255))

            win = check_winner(board)

            # 判断是否平局：未分胜负且棋盘无空格
            is_draw = (win == 0 and 0 not in board)

            if win == 0 and not is_draw:
                turn_text = "黑棋回合" if CURRENT_TURN == 1 else "白棋回合"
                turn_color = WIN_COLOR_BLACK if CURRENT_TURN == 1 else WIN_COLOR_WHITE
                img.draw_string_advanced(310, 400, 32, turn_text, color=turn_color)
            elif win == 1:
                img.draw_string_advanced(300, 400, 36, "黑棋胜利！", color=WIN_COLOR_BLACK)
            elif win == 2:
                img.draw_string_advanced(300, 400, 36, "白棋胜利！", color=WIN_COLOR_WHITE)
            elif is_draw:
                img.draw_string_advanced(300, 400, 36, "平局！", color=DRAW_COLOR)
        # ---------------- 模式 2：人机对战-AI先 ----------------
        elif current_mode == 2:
            img.draw_string_advanced(280, 30, 35,"人机对战-AI先", color=(0,255,255))

            win = check_winner(board)
            is_draw = (win == 0 and 0 not in board)

            # 1) 初始空棋盘 → AI 计算第一步
            if not any(board) and not ai_waiting_confirm and not ai_wait_human:
                _, ai_target_move = minimax(board, AI_DEPTH, -float('inf'), float('inf'), True)
                ai_waiting_confirm = True
                x, y, w, h, _ = roi_regions[ai_target_move]
                cx, cy = x + w//2, y + h//2
                img.draw_circle(cx, cy, 4, color=(255,255,0), thickness=0)
                print(f"请将黑棋放入格子 {ai_target_move+1} (中心位置: {cx},{cy})")

            # 2) 等待玩家放置黑棋
            elif ai_waiting_confirm:
                # 检查目标位置是否出现黑棋
                x, y, w, h, _ = roi_regions[ai_target_move]
                roi = img.copy(roi=(x, y, w, h))
                black_blobs = roi.find_blobs(GAME_BLACK_THRESHOLD, area_threshold=GAME_BLOB_AREA)
                if black_blobs:  # 检测到黑棋
                    board[ai_target_move] = 1

                    # ✅ 立即判断是否三连
                    win = check_winner(board)
                    if win == 1:
                        print("AI 三连！胜利！")
                        # 立即显示胜利文字
                        img.draw_string_advanced(300, 400, 36, "AI胜利！", color=WIN_COLOR_BLACK)
                        Display.show_image(img)
                        time.sleep(1)  # 停留 1 秒防止闪烁

                    prev_board[:] = board[:]
                    CURRENT_TURN = 2
                    ai_waiting_confirm = False
                    ai_wait_human = True
                    print(f"当前棋盘状态: {board}")
                    print(f"确认黑棋已放入格子 {ai_target_move+1}")

            # 3) 等待人类放置白棋
            elif ai_wait_human:
                human_moved = False
                moved_pos = -1

                # 检查所有可能的白棋位置
                for i in range(9):
                    if prev_board[i] == 0:  # 只检查空位
                        x, y, w, h, _ = roi_regions[i]
                        roi = img.copy(roi=(x, y, w, h))
                        white_blobs = roi.find_blobs(GAME_WHITE_THRESHOLD, area_threshold=GAME_BLOB_AREA)
                        if white_blobs:
                            human_moved = True
                            moved_pos = i
                            break

                if human_moved:
                    board[moved_pos] = 2
                    prev_board[:] = board[:]
                    ai_wait_human = False
                    print(f"当前棋盘状态: {board}")
                    print(f"检测到白棋放入格子 {moved_pos+1}")

                    # AI计算下一步
                    if check_winner(board) == 0 and 0 in board:
                        _, ai_target_move = minimax(board, AI_DEPTH, -float('inf'), float('inf'), True)
                        x, y, w, h, _ = roi_regions[ai_target_move]
                        cx, cy = x + w//2, y + h//2
                        img.draw_circle(cx, cy, 4, color=(255,255,0), thickness=0)
                        print(f"请将黑棋放入格子 {ai_target_move+1} (中心位置: {cx},{cy})")
                        ai_waiting_confirm = True

            # 显示状态
            if win == 0 and not is_draw:
                status = "等待放置黑棋" if ai_waiting_confirm else "请放置白棋" if ai_wait_human else "AI思考中"
                img.draw_string_advanced(310, 400, 32, status, color=(255, 255, 255))
            elif win == 1:
                img.draw_string_advanced(300, 400, 36, "AI胜利！", color=WIN_COLOR_BLACK)
            elif win == 2:
                img.draw_string_advanced(300, 400, 36, "人胜利！", color=WIN_COLOR_WHITE)
            elif is_draw:
                img.draw_string_advanced(300, 400, 36, "平局！", color=DRAW_COLOR)

        # ---------------- 模式 3：人机对战-人先 ----------------
        elif current_mode == 3:
            img.draw_string_advanced(280, 30, 35, "人机对战-人先", color=(0,255,255))

        # -------------- 复位按键：只在出现结果后生效 -------------
        if (win == 1 or win == 2 or is_draw) and key2.value() == 0:
            time.sleep_ms(20)               # 消抖
            if key2.value() == 0:
                board[:] = [0] * 9          # 清棋盘
                prev_board[:] = [0] * 9
                CURRENT_TURN = 1            # 重置先手
                print("复位完成")
                while key2.value() == 0:    # 等待松手
                    pass

        img.draw_string_advanced(10, 10, 30,f"fps: {clock.fps():.2f}",color=(255, 255, 0))
        if not stable_detected:
            cnt = len(detected_rects) if detected_rects else 0
            img.draw_string_advanced(10, 50, 30,f"矩形数: {cnt}",color=(255, 255, 0))
        Display.show_image(img)

except KeyboardInterrupt as e:
    print("用户停止:", e)
except BaseException as e:
    print("程序异常:", e)
finally:
    print("释放资源...")
    if sensor:
        sensor.stop()
    Display.deinit()
    MediaManager.deinit()
    print("程序已退出")
