#代码模仿之源：
#http://eyehere.net/2011/python-pygame-novice-professional-index/

import pygame

#background_file_name = 'bk_win.jpg'
#mouse_file_name = 'mouse_sheep.png'

from pygame.locals import *
from sys import exit
from pygame.color import THECOLORS#颜色

#全局变量
rect_white = {
    'x' :900, #下方白矩形的x坐标
    'width' : 80 #下方白矩形的宽
}

rect = {#小方块 分别表示横纵坐标、速度、加速度
    'x':200,
    'y':270,
    'v': 0,
    'a': 0
}

wind_status = {#1:小方块是dead/live 2：下方大矩形stop/move
   'rect_me' : 1,#1表示小球运动，0表示死亡
   'rect_bottom' : 1#1表示继续显示，0表示暂停
}

def DrawMyRect(screen):
    time_passed = clock.tick()
    t = time_passed / 50.0

    pressed_keys = pygame.key.get_pressed()
    if pressed_keys[K_SPACE]:
        rect['v'] = -25.0
        rect['a'] = 3.0
        rect['y'] = 270
    rect['y'] += rect['v'] * t + rect['a'] * t * t / 2
    rect['v'] += rect['a'] * t

    if rect['y'] >= 270.0 and  wind_status['rect_me']:
         rect['y'] = 270.0

    myRect = pygame.Rect((rect['x'], rect['y']), (50, 50))

    pygame.draw.rect(screen, (0,191,225), myRect, 0)

def DrawRectBottom(screen):
    '画窗口下半部分的黑色区域和白条'

    time_passed = clock_bottom.tick()
    time_passed_seconds = time_passed / 1000.0
    if wind_status['rect_bottom'] :
        rect_white['x'] -= time_passed_seconds * 200#控制小白块的的移动速度 ，600为每秒移动的像素

    rect_bottom1 = pygame.Rect(0, 320, rect_white['x'], 320)#下半个窗口的数据
    pygame.draw.rect(screen, [105, 105, 105], rect_bottom1, 0)

    rect_bottom2 = pygame.Rect(rect_white['x'] +  rect_white['width'], 320, 960- rect_white['width'] - rect_white['x'], 320)  # 下半个窗口的数据
    pygame.draw.rect(screen, [105, 105, 105], rect_bottom2, 0)

    if rect_white['x'] + rect_white['width'] < 0.:
        rect_white['x'] = 960. - rect_white['width']

def IsRectDead():
    if rect_white['x'] < rect['x']  and  rect['x']  < rect_white['x'] + rect_white['width']  and rect['y'] >= 270 :
        wind_status['rect_me'] = 0
        wind_status['rect_bottom'] = 0
    else:
        wind_status['rect_me'] = 1
        wind_status['rect_bottom'] = 1




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
clock_bottom  = pygame.time.Clock()



#游戏主循环
while True:
    for event in pygame.event.get():
        if event.type == QUIT:
            exit()

    screen.fill((255, 255, 255))

    IsRectDead()

    DrawMyRect(screen)

    DrawRectBottom(screen)

    pygame.display.update()






