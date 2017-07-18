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
    'height':40
}

colorline = {
    'x':960,
    'y': 0,
    'width': 6,
    'color':'pink'
}

rect_white = {
    'x' :900, #下方白矩形的x坐标
    'width' : 80 #下方白矩形的宽
}

rect = {#小方块玩家 分别表示横纵坐标、速度、加速度
    'x':200,
    'y':270,
    'v': 0,
    'a': 0,
    'width':50,
    'color':'pink'
}

color = {
    'pink': (255, 193, 193),
    'blue': (135, 206, 250),
    'green': (154, 255, 154)
}

wind_status = {#1:小方块是dead/live 2：下方大矩形stop/move
   'rect_me' : 1,#1表示小球运动，0表示死亡
    'rect_me_white':0, #表示是否掉落死亡死亡 1表示是0表示不是
   'rect_bottom' : 1,#1表示继续显示，0表示暂停
    'triangle': 1,
    'colorline':1,
    'movespeed':3
}

def DrawMyRect(screen):
    '画正方形'
    time_passed = clock.tick()
    t = time_passed / 20.0
# 方块坐标
    pressed_keys = pygame.key.get_pressed()
    if pressed_keys[K_SPACE] and rect['y'] == 270 and wind_status['rect_me'] :
        rect['v'] = -35.0
        rect['a'] = 3.0
    rect['y'] += rect['v'] * t + rect['a'] * t * t / 2
    rect['v'] += rect['a'] * t

#如果方块越界
    if rect['y'] >= 270.0 and  wind_status['rect_me']:
         rect['y'] = 270.0

#如果方块死亡
    if not wind_status['rect_me']:
        if wind_status['rect_me_white']:
            rect['v'] = 2
            rect['a'] = 15
        else:
            rect['v'] = 0
            #rect['y'] = 270
            rect['a'] = -3

# 看是否撞线
    if rect['x']  <= colorline['x'] and \
        rect['x'] + rect['width'] >= colorline['x']:
        if colorline['color'] == 'blue':
            rect['color'] = 'blue'
        if colorline['color'] == 'pink':
            rect['color'] = 'pink'
        if colorline['color'] == 'green':
            rect['color'] = 'green'
    myRect = pygame.Rect((rect['x'], rect['y']), (rect['width'], rect['width']))
    pygame.draw.rect(screen, color[rect['color']], myRect, 0)

def DrawTriangle(screen):
    '画顶部为三角的障碍物'
    if wind_status['triangle']:
        triangle['x'] -=  wind_status['movespeed']
    x = triangle['x']
    w = 40
    pointlist = [(x,320),(x,640),(x + w,640),(x + w ,320),(x + w / 2,280)]
    pygame.draw.polygon(screen, color['pink'], pointlist, 0)

    x += 40
    pointlist = [(x, 320), (x, 640), (x + w, 640), (x + w, 320), (x + w / 2, 280)]
    pygame.draw.polygon(screen, color['blue'], pointlist, 0)

    x += 40
    pointlist = [(x, 320), (x, 640), (x + w, 640), (x + w, 320), (x + w / 2, 280)]
    pygame.draw.polygon(screen, color['green'], pointlist, 0)

    if x + w < 0.:
        triangle['x'] = 960

def DrawRectBottom(screen):
    '画窗口下半部分的黑色区域和白条'
    if wind_status['rect_bottom'] :
        rect_white['x'] -=  wind_status['movespeed']   #控制小白块的的移动速度 ，600为每秒移动的像素

    rect_bottom1 = pygame.Rect(0, 320, rect_white['x'], 320)#下半个窗口的数据
    pygame.draw.rect(screen, [105, 105, 105], rect_bottom1, 0)

    rect_bottom2 = pygame.Rect(rect_white['x'] +  rect_white['width'], 320, 960- rect_white['width'] - rect_white['x'], 320)  # 下半个窗口的数据
    pygame.draw.rect(screen, [105, 105, 105], rect_bottom2, 0)

    if rect_white['x'] + rect_white['width'] < 0.:
        rect_white['x'] = 960.

def DrawTranscolorLine(screen):
    #画三条彩线 方块穿过后会变色
    if wind_status['colorline']:
        colorline['x'] -= wind_status['movespeed']

    start_pos = (colorline['x'] , 0)
    end_pos = (colorline['x'] , 960)
 #   pygame.draw.line(screen, color['pink'], start_pos, end_pos, colorline['width'])

  #  start_pos = (colorline['x'] + colorline['width'], 0)
   # end_pos = (colorline['x'] + colorline['width'], 960)
    #pygame.draw.line(screen, color['blue'], start_pos, end_pos, colorline['width'])

    if rect['color'] == 'blue':
        pygame.draw.line(screen, color['pink'], start_pos, end_pos, colorline['width'])
        colorline['color'] = 'pink'

    if rect['color'] == 'pink':
        pygame.draw.line(screen, color['green'], start_pos, end_pos, colorline['width'])
        colorline['color'] = 'green'

    if rect['color'] == 'green':
        pygame.draw.line(screen, color['blue'], start_pos, end_pos, colorline['width'])
        colorline['color'] = 'blue'

    if colorline['x'] < 0.:
        colorline['x'] = 960

def HitTriangle(_color,num):
    '小方块是否被长三角碰撞'
    if rect['color'] == 'blue':
          rectcolor = color['blue']
    if rect['color'] == 'pink':
          rectcolor = color['pink']
    if rect['color'] == 'green':
          rectcolor = color['green']

    if ( rectcolor == color[_color]):
        xl = rect['x']
        xr = rect['x'] + rect['width']
        y = rect['y'] +  rect['width']
        Tx1 = triangle['x'] + num * triangle['height']
        Txr = triangle['x'] + (num + 1) * triangle['height']
        if Tx1 <= xr and \
            xl <= Txr and \
            y > triangle['y'] - triangle['height']:
            wind_status['rect_me'] = 0

def IsRectDead():
    #小方块是否掉到白区域
    if rect_white['x'] < rect['x']   and\
        rect_white['x'] + rect_white['width']  > rect['x'] + rect['width'] and \
        rect['y']  >= 270:
        wind_status['rect_me'] = 0
        wind_status['rect_me_white'] = 1
    else:
        wind_status['rect_me'] = 1

    #小方块是否被长三角碰撞
    HitTriangle('pink',0)
    HitTriangle('blue',1)
    HitTriangle('green',2)

    if wind_status['rect_me'] == 0 :#暂停其他区域移动
        wind_status['rect_bottom'] = 0
        wind_status['triangle'] = 0
        wind_status['colorline'] = 0







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

    DrawMyRect(screen)

    DrawRectBottom(screen)

    DrawTriangle(screen)

    DrawTranscolorLine(screen)

    pygame.time.delay(8)

    pygame.display.update()






