import pygame
import random
from pygame.locals import *
from sys import exit
#全局变量

color = {
    'pink': (255, 193, 193),#'pink'
    'blue': (135, 206, 250),# 'blue'
    'green': (154, 255, 154),#'green'
}

color_n = {
    666: (255, 193, 193),#'pink'
    667: (135, 206, 250),# 'blue'
    668: (154, 255, 154),#'green'
}

color_ntoc = {
    666: 'pink',
    667: 'blue',
    668: 'green',
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
   'movespeed':3.5,
}
#控制关卡
levelcontrol = {
    'whitetrap':1,
    'triangle':1,
    'transcolorline':1,
    'blackwood':1
}

class Rect_ME :
    def __init__(self,x,y,v,a,w,h,c,ys,heapwave):
        self.x = x
        self.y = y
        self.v = v
        self.a =  a
        self.width =  w
        self.height = h
        self.color =  c
        self.y_safe =  ys
        self.heapwave = heapwave

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
       # if self.x <= col_x and \
        #        self.x + self.width/20  >= col_x:
        #        self.color = color_ntoc[col]
        if self.x <= col_x and \
              self.x + 5 >= col_x:
            self.color = color_ntoc[col]

    def onKeyDown(self):
       # time_passed = clock.tick()
       # t = time_passed / 20.0
        t = 0.5
        pressed_keys = pygame.key.get_pressed()
        if pressed_keys[K_SPACE] and self.y == self.y_safe and wind_status['rect_me']:
            self.heapwave.play()
            self.v = -40.0
            self.a = 4.

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
    def __init__(self,x,y,h,w,l,n,d,inte,of,op,againx):
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
        self.againx = againx

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
        #if x + w < 0.:
           # self.x = self.againx

class RectBottom:
    def __init__(self,x,y,h,w,xw,yw,ww,againx):
        self.x = x
        self.y = y
        self.height = h
        self.width = w
        self.x_white = xw
        self.y_white = yw
        self.width_white =ww
        self.againx = againx

    def DrawRectBottom(self,screen):
        if levelcontrol['whitetrap']:
            if wind_status['rect_bottom']:
                self.x_white -= wind_status['movespeed']  # 控制小白块的的移动速度 ，600为每秒移动的像素
            rect_bottom1 = pygame.Rect(self.x, self.y, self.x_white, self.height)  # 下半个窗口的数据
            pygame.draw.rect(screen, [105, 105, 105], rect_bottom1, 0)

            rect_bottom2 = pygame.Rect(self.x_white + self.width_white, 320,
                                       self.width  - self.width_white - self.x_white, 320)  # 下半个窗口的数据
            pygame.draw.rect(screen, [105, 105, 105], rect_bottom2, 0)

           # if  self.x_white + self.width_white < 0.:
           #     self.x_white = self.againx
        else:
            pygame.draw.rect(screen, [105, 105, 105], [self.x, self.y, self.width, self.height], 0)

class TransColorLine:
    color = 668
    setx = 1300
    def __init__(self,x,y,w,h,againx):
        self.x = x
        self.y = y
        self.width = w
        self.height = h
        self.againx = againx

    def DrawTranscolorLine(self,screen):
        # 画三条彩线 方块穿过后会变色
        if not levelcontrol['transcolorline'] :
            return

        if self.x == self.setx:
            self.color += 1
            if self.color == 669:
                self.color = 666

        if wind_status['colorline']:
             self.x -= wind_status['movespeed']

        start_pos = (self.x, self.y)
        end_pos = (self.x, self.height)

        pygame.draw.line(screen, color_n[self.color], start_pos, end_pos, self.width)

class BlackWood:
    def __init__(self,x,y,w,h):
        self.x = x
        self.y = y
        self.width = w
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
        rxl = rect.x
        rxr= rect.x + rect.width
        rect_BlackWood = pygame.Rect((x, y),(w, h))  # 下半个窗口的数据
        pygame.draw.rect(screen, [0, 0, 0], rect_BlackWood, 0)


        #if x + w < 0.:
         #   self.x = 960

def ReachBB(rect,bb,RectBottom):
    if rect.x <= bb.x + bb.width and rect.x + rect.width >= bb.x  and rect.y < bb.y:
        rect.y_safe = bb.y - rect.height
    else:
        rect.y_safe = RectBottom.y - rect.height

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
                Tyu = triangle.y - triangle.h
                Tyd = triangle.y
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
        #gameover_wav.play()
        wind_status['rect_me_white'] = 1
   # else:
       # wind_status['rect_me'] = 1

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

def starmove():
    star_pos[0][0] -= wind_status['movespeed']
    star_pos[1][0] -= wind_status['movespeed']
    star_pos[2][0] -= wind_status['movespeed']
    star_pos[3][0] -= wind_status['movespeed']
    star_pos[4][0] -= wind_status['movespeed']
    star_pos[5][0] -= wind_status['movespeed']
    star_pos[6][0] -= wind_status['movespeed']
    star_pos[7][0] -= wind_status['movespeed']

def starinit():
    star_pos[0][0] = 400
    star_pos[1][0] = 500
    star_pos[2][0] = 600
    star_pos[3][0] = 700
    star_pos[4][0] = 800
    star_pos[5][0] = 900
    star_pos[6][0] = 1000
    star_pos[7][0] = 1100
    for i in range(8):
        star_pos[i][2] = 1

def HitStar(rect,score):
    scoreadd = 0
    for i in range(8):
        x = star_pos[i][0] + 25
        y = star_pos[i][1] + 25
        if rect.x - 100 < x and x < rect.x + rect.width + 100 and \
            rect.y < y - 100 and y < rect.y + rect.height + 100:
            if star_pos[i][2] == 1:
                getstar_wav.play()
                star_pos[i][2] = 0
                scoreadd += 0.5
    return scoreadd

pygame.init()
pygame.mixer.init()
clock = pygame.time.Clock()
framerate = pygame.time.Clock()
screen = pygame.display.set_mode((640, 640), 0, 32)  # Alpha通道用32
pygame.display.set_caption("ColorBlock")

font_score = pygame.font.SysFont('幼圆', 40, True)
font_over1 = pygame.font.SysFont('幼圆', 100, True)
font_over2 = pygame.font.SysFont('幼圆', 100, True)
fontsize = [30,40]
gameover_wav=pygame.mixer.Sound("over.wav")
readygo_wav=pygame.mixer.Sound("readygo.wav")
   #奖励品
star_file_name = 'star1.png'
star_pos = [[-100,200,1],[-100,200,1],[-100,200,1],[-100,200,1],[-100,200,1],[-100,200,1],[-100,200,1],[-100,200,1]]
getstar_wav=pygame.mixer.Sound("getstar.wav")
    #障碍对象
star = pygame.image.load(star_file_name).convert()
heapwav=pygame.mixer.Sound("heap.wav")
rect = Rect_ME(200, 270, 0, 0, 50, 50, 'blue', 270,heapwav)
RectBottom_1 = RectBottom(0, 320, 320, 640, -200, 320, 80, 2000)
colorline = TransColorLine(-200, 0, 6, 640, 640)

Triangle_UP1 = Triangle(-200, 320, 40, 40, 220, 3, 'up', 50, 0, 20,1000)
Triangle_UP2 = Triangle(-200, 320, 40, 40, 320, 1, 'up', 40, 0, 20,1000)
Triangle_UP3 = Triangle(-200, 320, 40, 40, 320, 1, 'up', 40, 0, 20,1000)
Triangle_DOWN1 = Triangle(-200, 250, 40, 40, 200, 3, 'down', 60, 20, 0, 1000)
Triangle_DOWN2 = Triangle(-200, 255, 40, 40, 200, 1, 'down', 60, 20, 0, 1000)
blackwood_1 = BlackWood(-400, 235, 240, 20)

Score = [0,0]
pygame.mixer.music.set_volume(4)

def GamePro():
    score = 0
    initcontrol = 0
    count = 250
    n = 0
    level = 0
   # readygo_wav.play()
   # pygame.time.delay(50000)
#游戏主循环
    while True:

         for event in pygame.event.get():
             if event.type == QUIT:
                 exit()
         wind_status['movespeed'] = 3 + level * 1
         #gameover_wav = pygame.mixer.Sound("over.wav")
         if count >= 350 and wind_status['rect_me']:
             count = 0

             if n == 0:
                RectBottom_1.width_white = 80
                RectBottom_1.x_white = 840
                Triangle_UP1.x = 1040
                colorline.x = 1300

             elif n == 1:
                 RectBottom_1.width_white = 80
                 RectBottom_1.x_white = 840
                 Triangle_UP1.x = 1040
                 colorline.x = 1300

             elif n == 2:
                 starinit()
                 addcount = 1

             elif n == 3:
                 RectBottom_1.width_white = 80
                 RectBottom_1.x_white = 840
                 Triangle_UP1.x = 1040
                 colorline.x = 1300

             elif n == 4:
                 Triangle_UP2.num = 1
                 Triangle_UP2.x = 640
                 RectBottom_1.width_white = 240
                 RectBottom_1.x_white = 840
                 Triangle_UP3.num = 1
                 Triangle_UP3.x = 1240
                 colorline.setx = 960
                 colorline.x = 960

             elif n == 5:
                 starinit()
                 addcount = 1
             elif n == 6:
                 Triangle_UP2.num = 2
                 Triangle_UP2.x = 640
                 RectBottom_1.width_white = 160
                 RectBottom_1.x_white = 880
                 Triangle_UP3.num = 2
                 Triangle_UP3.x = 1200
                 colorline.setx = 960
                 colorline.x = 960

             elif n == 7:
                 Triangle_UP2.num = 3
                 Triangle_UP2.x = 640
                 RectBottom_1.width_white = 80
                 RectBottom_1.x_white = 920
                 Triangle_UP3.num = 3
                 Triangle_UP3.x = 1160
                 colorline.setx = 960
                 colorline.x = 960

             elif n == 8:
                 starinit()
                 addcount = 1
             elif n == 9:
                 Triangle_DOWN1.x = 640
                 Triangle_DOWN2.x = 820
                 RectBottom_1.width_white = 320
                 RectBottom_1.x_white = 960
                 blackwood_1.x = 1000
                 colorline.setx = 1300
                 colorline.x = 1300

             elif n == 10:
                 Triangle_DOWN1.x = 640
                 Triangle_DOWN2.x = 820
                 RectBottom_1.width_white = 320
                 RectBottom_1.x_white = 960
                 blackwood_1.x = 1000
                 colorline.setx = 1300
                 colorline.x = 1300

             elif n == 11:
                 starinit()
                 addcount = 1
             elif n == 12:
                 Triangle_DOWN1.x = 640
                 Triangle_DOWN2.x = 820
                 RectBottom_1.width_white = 320
                 RectBottom_1.x_white = 960
                 blackwood_1.x = 1000
                 colorline.setx = 1300
                 colorline.x = 1300

             elif n == 13:
                 starinit()
                 addcount = 1
             n += 1
             if n == 14:
                 n = 0

         screen.fill((255, 255, 255))

         rect.DrawMyRect(screen, colorline)
         RectBottom_1.DrawRectBottom(screen)
         colorline.DrawTranscolorLine(screen)

         level_1()
         starmove()
         score += HitStar(rect,score)

         for i in range(8):
             if star_pos[i][2]:
                screen.blit(star, (star_pos[i][0], star_pos[i][1]))

         Triangle_UP1.DrawTriangle(screen)
         IsRectDead(rect, RectBottom_1, Triangle_UP1)

         Triangle_UP2.DrawTriangle(screen)
         IsRectDead(rect, RectBottom_1, Triangle_UP2)

         Triangle_UP3.DrawTriangle(screen)
         IsRectDead(rect, RectBottom_1, Triangle_UP3)

         Triangle_DOWN1.DrawTriangle(screen)
         IsRectDead(rect, RectBottom_1, Triangle_DOWN1)

         Triangle_DOWN2.DrawTriangle(screen)
         IsRectDead(rect, RectBottom_1, Triangle_DOWN2)

         blackwood_1.DrawBlackWood(screen, rect, RectBottom_1)
         ReachBB(rect, blackwood_1, RectBottom_1)

         pygame.time.delay(8)

         #score = Score[0]
         #if wind_status['rect_me'] == 0:



         if wind_status['rect_me'] == 1:
             surfaceScore = font_score.render("SCORE:%d LEVEL:%d" % (score, level), True, [0, 0, 0])
             screen.blit(surfaceScore, [20, 20])
             count += 1
             level = (int(score) - int(score) % 10) / 10 + 1
             score += 0.01
         else:
             if fontsize[0] < 480:
                 fontsize[0] += 2
                 fontsize[1] += 1
             surfaceScore = font_over1.render("SCORE:%d" % score, True, [0, 0, 0])
             surfaceScore =  pygame.transform.scale(surfaceScore, (fontsize[0], fontsize[1]))
             screen.blit(surfaceScore, [100, 100])
             surfaceScore = font_over2.render("LEVEL:%d" % level, True, [0, 0, 0])
             surfaceScore = pygame.transform.scale(surfaceScore, (fontsize[0], fontsize[1]))
             screen.blit(surfaceScore, [100, 250])

         pygame.display.update()

         if initcontrol == 0:
             readygo_wav.play()
             pygame.time.delay(800)
             initcontrol = 1

         if wind_status['rect_me'] == 0 :

             if initcontrol == 1:
                gameover_wav.play()
                initcontrol = 2






