import pygame
from pygame.locals import *
from sys import exit
#from pygame.color import THECOLORS #颜色
#import threading
#全局变量

color = {
    'pink': (255, 193, 193),#'pink'
    'blue': (135, 206, 250),# 'blue'
    'green': (154, 255, 154),#'green'
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
   'movespeed':3,
}
#控制关卡
levelcontrol = {
    'whitetrap':1,
    'triangle':1,
    'transcolorline':1,
    'blackwood':1
}

pygame.init()
clock = pygame.time.Clock()

class Rect_ME :
    def __init__(self,x,y,v,a,w,h,c,ys):
        self.x = x
        self.y = y
        self.v = v
        self.a =  a
        self.width =  w
        self.height = h
        self.color =  c
        self.y_safe =  ys

    def isOverBoundry(self):
        # 如果方块越界
        if self.y >= self.y_safe and wind_status['rect_me']:
            self.y = self.y_safe

    def isDead(self):
        # 如果方块死亡
        if not wind_status['rect_me']:
            if wind_status['rect_me_white']:
                self.v = 2
                self.a = 15
            else:
                # self.v = 0
                self.y = 270
                # self.a = -0.1

    def hitTransLine(self,col_x,col):
        # 看是否撞变色线
        if self.x <= col_x and \
                self.x + self.width/20  >= col_x:
                self.color = col

    def onKeyDown(self):
       # time_passed = clock.tick()
       # t = time_passed / 20.0
        t = 0.5
        pressed_keys = pygame.key.get_pressed()
        if pressed_keys[K_SPACE] and self.y == self.y_safe and wind_status['rect_me']:
            self.v = -35.0
            self.a = 3.0

        self.y += self.v * t + self.a * t * t / 2
        self.v += self.a * t

        if pressed_keys[K_q]:
            self.v = 0.0
            self.a = 0.0
            self.y = self.y_safe - 5
        if pressed_keys[K_w]:
            self.v = 0.0
            self.a = 0.0
            self.y = self.y_safe

    def DrawMyRect(self,screen,transcolorline ):
        '画正方形'
        # 判断键值
        self.onKeyDown()

        # 方块是否越界
        self.isOverBoundry()

        # 方块是否死亡
        self.isDead()

        # 看是否撞变色线
        self.hitTransLine(transcolorline.x,transcolorline.color)

        myRect = pygame.Rect((self.x, self.y), (self.width, self.width))

        pygame.draw.rect(screen, color[self.color ], myRect, 0)

class Triangle:
    def __init__(self,x,y,h,w,l,n,d,inte,of,op):
        self.x = x
        self.y = y
        self.h = h
        self.w = w
        self.len = l
        self.num = n
        self.dir = d
        self.interval = inte
        self.offset_foot = of
        self.offset_peak =op

    def moveControl(self):
        if wind_status['triangle']:
            self.x -= wind_status['movespeed']

    def DrawTriangle(self,screen):
        '画顶部为三角的障碍物'
        if not levelcontrol['triangle']:
            return

        self.moveControl()

        x = self.x
        y = self.y
        len = self.len
        n = self.num
        h = self.h
        w = self.w
        col = 666
        i = 0
        offset = self.offset_foot
        color = {
            666: (255, 193, 193),  # 'pink'
            667: (135, 206, 250),  # 'blue'
            668: (154, 255, 154),  # 'green'
        }
        while i < n:
            if self.dir== 'down':
                pointlist = [(x, y - h), (x + offset, y - h - len), (x + w + offset, y - h - len), (x + w, y - h),
                             (x + self.offset_peak, y)]
            else:
                pointlist = [(x, y), (x + offset, y + len), (x + w + offset, y + len), (x + w, y),
                             (x + self.offset_peak, y - h)]
            pygame.draw.polygon(screen, color[col], pointlist, 0)
            x += self.interval
            i += 1
            col += 1
        if x + w < 0.:
            self.x = 960



class RectBottom:
    def __init__(self,x,y,h,w,xw,yw,ww):
        self.x = x
        self.y = y
        self.height = h
        self.width = w
        self.x_white = xw
        self.y_white = yw
        self.width_white =ww

    def DrawRectBottom(self,screen):
        if levelcontrol['whitetrap']:
            if wind_status['rect_bottom']:
                self.x_white -= wind_status['movespeed']  # 控制小白块的的移动速度 ，600为每秒移动的像素
            rect_bottom1 = pygame.Rect(self.x, self.y, self.x_white, self.height)  # 下半个窗口的数据
            pygame.draw.rect(screen, [105, 105, 105], rect_bottom1, 0)

            rect_bottom2 = pygame.Rect(self.x_white + self.width_white, 320,
                                       self.width  - self.width_white - self.x_white, 320)  # 下半个窗口的数据
            pygame.draw.rect(screen, [105, 105, 105], rect_bottom2, 0)

            #if  self.x_white + self.width_white < 0.:
            #    self.x_white = 960.
        else:
            pygame.draw.rect(screen, [105, 105, 105], [self.x, self.y, self.width, self.height], 0)

class TransColorLine:
    def __init__(self,x,y,w,h,col):
        self.x = x
        self.y = y
        self.width = w
        self.height = h
        self.color = col

    def DrawTranscolorLine(self,screen):
        # 画三条彩线 方块穿过后会变色
        if not levelcontrol['transcolorline']:
            return

        if wind_status['colorline']:
             self.x -= wind_status['movespeed']

        start_pos = (self.x, self.y)
        end_pos = (self.x, self.height)

        pygame.draw.line(screen, color[self.color], start_pos, end_pos, self.width)

        #if self.x < 0.:
         #   self.x = 960

class BlackWood:
    def __init__(self,x,y,w,h):
        self.x = x
        self.y = y
        self. width = w
        self.height = h

    def DrawBlackWood(self,screen,rect,RectBottom):
        if not levelcontrol['blackwood']:
            return
        if wind_status['blackwood']:
            self.x -= wind_status['movespeed']
        x = self.x
        y = self.y
        h = self.height
        w = self.width
        rect_BlackWood = pygame.Rect(x, y, w, h)  # 下半个窗口的数据
        pygame.draw.rect(screen, [0, 0, 0], rect_BlackWood, 0)
        if rect.x + rect.width >= x and \
                        rect.x < x + w and \
                                rect.y + rect.height < y + h:
            rect.y_safe = y - rect.height
        else:
            rect.y_safe = RectBottom.height - rect.height

        #if x + w < 0.:
         #   self.x = 960

def HitTriangle(rect,triangle):
        '小方块是否被长三角碰撞'
        firstcol = 666
        if rect.color == 'pink':
            reccol = 666
        if rect.color == 'blue':
            reccol = 667
        if rect.color == 'green':
            reccol = 668

        num = 0
        while num < triangle.num:
            if (reccol == firstcol):
                xl = rect.x
                xr = rect.x + rect.width
                yu = rect.y
                yd = rect.y + rect.height
                Tx1 = triangle.x + num * triangle.interval
                Txr = triangle.x + num * triangle.interval + triangle.w
                Tyu = triangle.y
                Tyd = triangle.y + triangle.h
                if Tx1 <= xr and \
                   xl <= Txr and \
                   yd >= Tyu and \
                   yd <= Tyd:
                    wind_status['rect_me'] = 0
                if Tx1 <= xr and \
                    xl <= Txr and \
                    yu >= Tyu and \
                    yu <= Tyd:
                    wind_status['rect_me'] = 0
            num += 1
            firstcol += 1

def IsRectDead(rect,rect_bot,triangle):
    #小方块是否掉到白区域
    if rect_bot.x_white < rect.x   and \
        rect_bot.x_white + rect_bot.width_white  > rect.x + rect.width and \
        rect.y  >= 270:
        wind_status['rect_me'] = 0
        wind_status['rect_me_white'] = 1
    else:
        wind_status['rect_me'] = 1

    #小方块是否被三角碰撞
    HitTriangle(rect,triangle)

    if wind_status['rect_me'] == 0 :#暂停其他区域移动
        wind_status['rect_bottom'] = 0
        wind_status['triangle'] = 0
        wind_status['colorline'] = 0
        wind_status['blackwood'] = 0
        #wind_status['smalltriangle'] = 0

def level_1():

    levelcontrol['whitetrap'] = 1
    levelcontrol['triangle'] = 1
    levelcontrol['transcolorline'] = 1
    levelcontrol['blackwood'] = 1

def gameproc():

    screen = pygame.display.set_mode((960, 640), 0, 32)  # Alpha通道用32
    pygame.display.set_caption("小方块哦哦哦")
    rect = Rect_ME(200, 270, 0, 0, 50, 50, 'pink', 270)
    Triangle_1 = Triangle(1360, 320, 40, 40, 200, 3, 'up', 50, 0, 20)
    RectBottom_1 = RectBottom(0, 320, 320, 960, 900, 320, 80)
    RectBottom_2 = RectBottom(0, 320, 320, 960, 900, 320, 80)
    colorline_pink = TransColorLine(1200, 0, 6, 960, 'pink')
    colorline_blue = TransColorLine(1500, 0, 6, 960, 'blue')
    colorline_green = TransColorLine(1200, 0, 6, 960, 'green')

    blackwood_1 = BlackWood(1400, 250, 100, 20)

#游戏主循环
    while True:
         for event in pygame.event.get():
             if event.type == QUIT:
                 exit()

         screen.fill((255, 255, 255))

         level_1()

         rect.DrawMyRect(screen,colorline_blue)

         RectBottom_1.DrawRectBottom(screen)

         Triangle_1.DrawTriangle(screen)

         colorline_blue.DrawTranscolorLine(screen)

         IsRectDead(rect,RectBottom_1,Triangle_1)

         pygame.time.delay(8)

         pygame.display.update()

gameproc()



