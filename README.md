# EUI (Embedded User Interface)

**EUI** 是一个面向低像素单色显示器（如 OLED 128×64、LCD 等）的轻量级嵌入式 GUI 库，纯 C 语言编写，零依赖，资源占用极低。

## 功能特性

### 🎨 图形绘制
| 函数/宏 | 说明 |
|---------|------|
| `eui_point()` | 在当前焦点位置画一个像素点 |
| `eui_line(x, y)` | 从当前焦点到目标点画线（Bresenham 算法） |
| `eui_rect(w, h, style)` | 从焦点绘制矩形，`style` 为填充样式（0=空心，非0=填充图案） |
| `eui_circle(r, style)` | 从焦点绘制圆形（八分法），支持填充 |
| `eui_clear(style)` | 按样式清屏（0=全黑，0xFF=全白，其他值=图案填充） |

### 📝 文本输出
| 函数/宏 | 说明 |
|---------|------|
| `eui_putc(c)` | 在当前焦点输出一个字符（8×5 像素 ASCII 字库） |
| `eui_puts(x, y, str)` | 从指定位置输出字符串，支持 `\n` 换行、`\r` 回车 |
| `eui_set_align(align)` | 设置字符间距（默认 = 字宽 + 1） |

### 🔄 屏幕操作
| 函数 | 说明 |
|------|------|
| `eui_move_left(x)` | 屏幕内容左移 x 个像素 |
| `eui_move_right(x)` | 屏幕内容右移 x 个像素 |

### 🔢 数字转字符串（零分配，使用静态缓冲区）
| 函数 | 说明 |
|------|------|
| `eui_u2str(num)` | uint32_t → 十进制字符串 |
| `eui_i2str(num)` | int32_t → 十进制字符串（带负号） |
| `eui_u2hstr(num)` | uint32_t → 十六进制字符串 |
| `eui_u2bstr(num)` | uint16_t → 二进制字符串（16位） |

### ⌨️ 按键管理
| API | 说明 |
|-----|------|
| `eui_update_key(status, key)` | 更新按键状态机（需在轮询中调用） |
| `EUI_ON(key, mode, {action})` | 按键事件宏，支持短按 / 长按 / 双击 / 短按非双击 |
| `EUI_KEY_MODE_SHORT` | 短按模式 |
| `EUI_KEY_MODE_LONG` | 长按模式（默认 80 帧触发） |
| `EUI_KEY_MODE_DOUBLE` | 双击模式 |
| `EUI_KEY_MODE_SHORT_NDOUBLE` | 短按但不触发双击 |

## 🎯 焦点（Focus）工作原理

EUI 的焦点（Focus）是一个"虚拟画笔"位置，所有绘图操作都以焦点为基准点进行。

### 焦点变量

```c
extern uint8_t _EUI_FOCUS_X;      // 焦点的 X 坐标（水平像素位置）
extern uint8_t _EUI_FOCUS_Y;      // 焦点的 Y 坐标（垂直像素位置）
extern uint8_t _EUI_FOCUS_Y_H;    // 焦点所在的页索引（Y >> 3）
extern uint8_t _EUI_FOCUS_Y_L;    // 焦点在页内的位偏移（Y & 0x07）
```

### 设计动机

由于屏幕缓冲区采用**位压缩存储**（每字节存放 8 个垂直像素），定位一个像素需要执行除法和取模运算才能找到对应的 byte 和 bit。焦点机制**预计算**了 `_EUI_FOCUS_Y_H`（页索引）和 `_EUI_FOCUS_Y_L`（位偏移），避免在频繁的绘图操作中重复计算，提升性能。

### 设置焦点

```c
#define eui_set_focus(x, y) {
  _EUI_FOCUS_X = (x);
  _EUI_FOCUS_Y = (y);
  _EUI_FOCUS_Y_H = _EUI_FOCUS_Y >> 3;  // 计算页索引
  _EUI_FOCUS_Y_L = _EUI_FOCUS_Y & 0x07; // 计算位偏移
}
```

调用 `eui_set_focus(x, y)` 会同时更新所有四个焦点变量，将画笔定位到坐标 `(x, y)`。

### 焦点与缓冲区的映射关系

缓冲区 `EUI_buf[EUI_H>>3][EUI_W]` 的布局：

```
                   X 方向 →
    ┌─────────────────────────────────────────┐
    │  EUI_buf[0][0]  EUI_buf[0][1]  ...      │  ← 页 0（Y: 0~7）
    │  bit7~bit0       bit7~bit0              │
    ├─────────────────────────────────────────┤
    │  EUI_buf[1][0]  EUI_buf[1][1]  ...      │  ← 页 1（Y: 8~15）
    │  bit7~bit0       bit7~bit0              │
    ├─────────────────────────────────────────┤
    │  ...                                    │
    └─────────────────────────────────────────┘
```

坐标 `(x, y)` 对应的像素位于：
- **byte**: `EUI_buf[y >> 3][x]`
- **bit**: `1 << (y & 0x07)`

### 焦点在绘图中的使用

所有绘图 API 都以当前焦点作为起点：

| 操作 | 焦点含义 |
|------|---------|
| `eui_point()` | 在 `(FOCUS_X, FOCUS_Y)` 画一个点 |
| `eui_line(x, y)` | 从 `(FOCUS_X, FOCUS_Y)` 到 `(x, y)` 画线 |
| `eui_rect(w, h, style)` | 以 `(FOCUS_X, FOCUS_Y)` 为左上角画矩形 |
| `eui_circle(r, style)` | 以 `(FOCUS_X, FOCUS_Y)` 为圆心画圆 |
| `eui_putc(c)` | 在 `(FOCUS_X, FOCUS_Y)` 输出字符，**结束后自动推进 X** |
| `eui_puts(x, y, str)` | 先调用 `eui_set_focus` 设置焦点，再逐个输出字符，支持 `\n` 换行、`\r` 回车 |

### 焦点推进（Advance）

字符输出后，焦点 X 会自动前进 `_EUI_INTERVAL` 个像素（默认 = 字宽 + 1），方便连续输出文本：

```c
// 输出 'H' 后焦点移动到下一个字符位置
eui_putc('H');  // FOCUS_X += _EUI_INTERVAL
eui_putc('i');  // FOCUS_X += _EUI_INTERVAL
```

换行符 `\n` 会使焦点 Y 增加一个字符高度，回车符 `\r` 使 X 回到行首。

### 使用示例

```c
// 设置焦点到 (10, 10)，画一个空心矩形
eui_set_focus(10, 10);
eui_rect(30, 20, 0);

// 在 (10, 35) 输出文本
eui_puts(10, 35, "Hello");

// 在圆心 (64, 32) 画圆
eui_set_focus(64, 32);
eui_circle(15, 0);
```

## 坐标系与缓冲区

屏幕缓冲区是一个位压缩的二维数组：

```c
extern uint8_t EUI_buf[EUI_H>>3][EUI_W];
```

- 每个 bit 代表一个像素点：**1 = 亮（黑色）**，**0 = 灭（白色/背景）**
- 垂直方向每 8 个像素打包为一个 byte，`EUI_buf[y>>3][x]` 的 bit `y&0x07` 对应坐标 `(x, y)`

### 屏幕分辨率配置

在 `src/eui.h` 中修改：

```c
#define EUI_W 128   // 屏幕宽度
#define EUI_H 64    // 屏幕高度
```

## 使用方式

**EUI 仅维护内存中的像素缓冲区，不绑定具体显示设备。** 你需要自己实现将 `EUI_buf` 发送到目标显示器。

### 基本使用流程

1. 包含头文件 `#include "eui.h"`
2. 使用绘图 API 操作 `EUI_buf`
3. 在刷新循环中将 `EUI_buf` 发送到显示设备

### 显示设备驱动示例（伪代码）

```c
void flush_display() {
    for (int y = 0; y < EUI_H; y++) {
        for (int x = 0; x < EUI_W; x++) {
            bool pixel = EUI_buf[y >> 3][x] & (1 << (y & 0x07));
            // 将 pixel 写入显示设备...
        }
    }
}
```

### 常见 OLED 驱动（如 SSD1306 I2C）

```c
void oled_flush() {
    for (int page = 0; page < EUI_H >> 3; page++) {
        ssd1306_write_data_block(EUI_buf[page], EUI_W);
    }
}
```

## SDL 模拟器（默认 main.c）

项目默认提供的 `main.c` 使用 **SDL3** 在计算机上模拟显示效果，方便开发和调试。

### 构建与运行

```bash
# 确保已安装 SDL3 开发库
sudo apt install libsdl3-dev       # Debian/Ubuntu
sudo pacman -S sdl3                  # Arch Linux

# 编译并运行
make run
```

### 模拟器操作

| 按键 | 功能 |
|------|------|
| <kbd>W</kbd> | 短按：数字 Y-1 / 长按：圆形 Y-1 |
| <kbd>A</kbd> | 短按：数字 X-1 / 长按：圆形 X-1 |
| <kbd>S</kbd> | 短按：数字 Y+1 / 长按：圆形 Y+1 |
| <kbd>D</kbd> | 短按：数字 X+1 / 长按：圆形 X+1 |

## 项目结构

```
eui/
├── main.c              # SDL3 模拟器主程序（用户可替换为自己的驱动）
├── keytest.c           # 按键状态机测试
├── strtest.c           # 数字转字符串测试
├── Makefile            # 构建文件
├── README.md
├── src/
│   ├── eui.h           # EUI 库头文件（API 定义 + 屏幕配置）
│   ├── eui.c           # EUI 库实现
│   ├── key.h           # 按键状态机头文件
│   ├── key.c           # 按键状态机实现
│   ├── pid.h           # PID 控制器头文件
│   ├── pid.c           # PID 控制器实现
│   └── fonts/
│       ├── en8x5.h     # 8×5 像素英文字库头文件
│       └── en8x5.c     # 8×5 像素英文字库（ASCII 0x20~0x7E）
```

## 设计理念

- **零动态分配**：所有缓冲区在编译期静态分配，适合 MCU 环境
- **极简依赖**：仅依赖 `<stdint.h>`，可在任何 C 编译器上使用
- **平台无关**：显示驱动由用户实现，EUI 只负责图形渲染
- **小巧高效**：位压缩缓冲区，节省 RAM 和传输带宽

