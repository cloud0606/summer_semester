import pygame

#background_file_name = 'bk_win.jpg'
#mouse_file_name = 'mouse_sheep.png'
import threading
from pygame.locals import *
from sys import exit
from pygame.color import THECOLORS#颜色

#全局变量
triangle = {
    'x': 1060,
    'y': 320,
    'num': 3,
    'height':40,
    'width':40,
    'long':200,
    'interval':50,
    'dir':'up',
    'move':0,
    'move_peak':20,
}

colorline = {
    'x':960,
    'y': 0,
    'width': 6,
    'color':'pink'
}

rect_white = {
    'x' :900, #下方白矩形的x坐标
    'y' : 320,
    'width' : 80 #下方白矩形的宽
}

blackwood = {
    'x' : 2000,
    'y' : 220,
    'width' : 100,
    'height' : 20
}

rect = {#小方块玩家 分别表示横纵坐标、速度、加速度
    'x':200,
    'y':270,
    'v': 0,
    'a': 0,
    'width':50,
    'height':50,
    'color':666,
    'y_safe':320
}

color = {
    666: (255, 193, 193),#'pink'
    667: (135, 206, 250),# 'blue'
    668: (154, 255, 154)#'green'
}
#控制窗口
wind_status = {#1:小方块是dead/live 2：下方大矩形stop/move
   'rect_me' : 1,#1表示小球运动，0表示死亡
   'rect_me_white':0, #表示是否掉落死亡死亡 1表示是0表示不是
   'rect_bottom' : 1,#1表示继续显示，0表示暂停
   'triangle': 1,
   'smalltriangle': 1,
   'colorline':1,
    'blackwood':1,
   'movespeed':3
}
#控制关卡
levelcontrol = {
    'whitetrap':0,
    'triangle':0,
    'transcolorline':0,
    'blackwood':0
}

def DrawMyRect(screen):
    '画正方形'
    time_passed = clock.tick()
    t = time_passed / 20.0
# 方块坐标
    pressed_keys = pygame.key.get_pressed()
    if pressed_keys[K_SPACE] and rect['y'] == rect['y_safe'] - rect['height'] and wind_status['rect_me'] :
        rect['v'] = -35.0
        rect['a'] = 3.0

    rect['y'] += rect['v'] * t + rect['a'] * t * t / 2
    rect['v'] += rect['a'] * t

    if pressed_keys[K_q]:
        rect['v'] = 0.0
        rect['a'] = 0.0
        rect['y'] = rect['y_safe'] - rect['height'] - 5
    if pressed_keys[K_w]:
        rect['v'] = 0.0
        rect['a'] = 0.0
        rect['y'] = rect['y_safe'] - rect['height']

#如果方块越界
    if rect['y'] >= rect['y_safe'] - rect['height']and  wind_status['rect_me']:
         rect['y'] = rect['y_safe'] - rect['height']

#如果方块死亡
    if not wind_status['rect_me']:
        if wind_status['rect_me_white']:
            rect['v'] = 2
            rect['a'] = 15
        else:
            #rect['v'] = 0
            rect['y'] = 270
            #rect['a'] = -0.1

# 看是否撞变色线
    if rect['x']  <= colorline['x'] and \
        rect['x'] + rect['width']/20 >= colorline['x']:
        if colorline['color'] == 'pink':
            rect['color'] = 666
        if colorline['color'] == 'blue':
            rect['color'] = 667
        if colorline['color'] == 'green':
            rect['color'] = 668
    myRect = pygame.Rect((rect['x'], rect['y']), (rect['width'], rect['width']))
    pygame.draw.rect(screen, color[rect['color']], myRect, 0)

def DrawBlackWood(screen):
    if not levelcontrol['blackwood']:
        return
    if wind_status['blackwood']:
        blackwood['x'] -= wind_status['movespeed']
    x = blackwood['x']
    y = blackwood['y']
    h = blackwood['height']
    w = blackwood['width']
    rect_bottom1 = pygame.Rect(x, y, w, h)  # 下半个窗口的数据
    pygame.draw.rect(screen, [0, 0, 0], rect_bottom1, 0)
    if rect['x']+ rect['width'] >= x and \
       rect['x'] < x + w  :
        rect['y_safe'] = y
    else :
        rect['y_safe'] = rect_white['y']

    if x + w < 0.:
        blackwood['x'] = 1600

def DrawTriangle(screen):
    '画顶部为三角的障碍物'
    if not levelcontrol['triangle'] :
        return
    if wind_status['triangle']:
        triangle['x'] -=  wind_status['movespeed']
    x = triangle['x']
    y = triangle['y']
    long = triangle['long']
    h = triangle['height']
    w = 40
    col = 666
    i = 0
    move = triangle['move']

    while i < triangle['num']:
        if triangle['dir'] == 'down':
            pointlist = [(x, y - h), (x + move, y - h - long), (x + w + move, y - h - long), (x + w, y - h), (x + triangle['move_peak'], y)]
        else:
            pointlist = [(x,y),(x + move,y + long),(x + w + move,y + long),(x + w ,y),(x + triangle['move_peak'],y - h)]
        pygame.draw.polygon(screen, color[col], pointlist, 0)
        x += triangle['interval']
        col += 1
        i += 1

    if x + w < 0.:
        triangle['x'] = 960

def DrawRectBottom(screen):
    '画窗口下半部分的黑色区域和白条'
    if levelcontrol['whitetrap'] :
        if wind_status['rect_bottom'] :
            rect_white['x'] -=  wind_status['movespeed']   #控制小白块的的移动速度 ，600为每秒移动的像素
        rect_bottom1 = pygame.Rect(0, 320, rect_white['x'], 320)#下半个窗口的数据
        pygame.draw.rect(screen, [105, 105, 105], rect_bottom1, 0)

        rect_bottom2 = pygame.Rect(rect_white['x'] +  rect_white['width'], 320, 960- rect_white['width'] - rect_white['x'], 320)  # 下半个窗口的数据
        pygame.draw.rect(screen, [105, 105, 105], rect_bottom2, 0)

        if rect_white['x'] + rect_white['width'] < 0.:
              rect_white['x'] = 960.
    else:
        pygame.draw.rect(screen, [105, 105, 105], [0,320,960,640], 0)

def DrawTranscolorLine(screen):
    #画三条彩线 方块穿过后会变色
    if not levelcontrol['transcolorline']:
        return

    if wind_status['colorline']:
        colorline['x'] -= wind_status['movespeed']

    start_pos = (colorline['x'] , 0)
    end_pos = (colorline['x'] , 960)

    if rect['color'] == 667:
        pygame.draw.line(screen, color[666], start_pos, end_pos, colorline['width'])
        colorline['color'] = 'pink'

    if rect['color'] == 668:
        pygame.draw.line(screen, color[667], start_pos, end_pos, colorline['width'])
        colorline['color'] = 'blue'

    if rect['color'] == 666:
        pygame.draw.line(screen, color[668], start_pos, end_pos, colorline['width'])
        colorline['color'] = 'green'


    if colorline['x'] < 0.:
        colorline['x'] = 960

def HitTriangle():
    '小方块是否被长三角碰撞'
    firstcol = 666
    reccol = rect['color']
    num = 0
    while num <  triangle['num'] :
       if ( reccol == firstcol):
          xl = rect['x']
          xr = rect['x'] + rect['width']
          yu = rect['y']
          yd = rect['y'] + rect['width']
          Tx1 = triangle['x'] + num * triangle['interval']
          Txr = triangle['x'] + num * triangle['interval'] + triangle['width']
          if Tx1 <= xr and \
             xl <= Txr and \
             yd  >= triangle['y'] - triangle['height'] and \
             yd  <= triangle['y']:
             wind_status['rect_me'] = 0
          if Tx1 <= xr and \
             xl <= Txr and \
             yu  >= triangle['y'] - triangle['height'] and \
             yu  <= triangle['y']:
             wind_status['rect_me'] = 0
       num += 1
       firstcol += 1

def level_1():

    levelcontrol['whitetrap'] = 1
    levelcontrol['triangle'] = 1
    levelcontrol['transcolorline'] = 1
    levelcontrol['blackwood'] = 1

def IsRectDead():
    #小方块是否掉到白区域
    if rect_white['x'] < rect['x']   and\
        rect_white['x'] + rect_white['width']  > rect['x'] + rect['width'] and \
        rect['y']  >= 270:
        wind_status['rect_me'] = 0
        wind_status['rect_me_white'] = 1
    else:
        wind_status['rect_me'] = 1

    #小方块是否被三角碰撞
    HitTriangle()

    if wind_status['rect_me'] == 0 :#暂停其他区域移动
        wind_status['rect_bottom'] = 0
        wind_status['triangle'] = 0
        wind_status['colorline'] = 0
        wind_status['blackwood'] = 0
        #wind_status['smalltriangle'] = 0







#background = pygame.image.load(background_file_name).convert()
#mouse = pygame.image.load(mouse_file_name).convert_alpha()
# screen.blit(background,(0,0))
   #x,y = pygame.mouse.get_pos()
    #x -= mouse.get_width() / 2
    #y -= mouse.get_height() / 2
    #screen.blit(mouse,(x,y))

pygame.init()
screen = pygame.display.set_mode((960,640),0,32)#Alpha通道用32
pygame.display.set_caption("小方块哦哦哦")
clock = pygame.time.Clock()

#游戏主循环
while True:
    for event in pygame.event.get():
        if event.type == QUIT:
            exit()

    screen.fill((255, 255, 255))

    IsRectDead()

    level_1()

    DrawMyRect(screen)

    DrawRectBottom(screen)

    DrawBlackWood(screen)

    DrawTriangle(screen)

    DrawTranscolorLine(screen)

    pygame.time.delay(8)

    pygame.display.update()






