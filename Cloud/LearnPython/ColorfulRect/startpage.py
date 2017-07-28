import pygame
from sys import exit
pygame.init()
startpage_file_name = 'startpage.jpg'
startbut_up_file_name = 'start.png'
startbut_down_file_name = '_start.png'
endbut_up_file_name = 'end.png'
endbut_down_file_name = '_end.png'

screen_start = pygame.display.set_mode((640,640),0,32)#Alpha通道用32
pygame.display.set_caption("ColorBlockStatpage")
startpage = pygame.image.load(startpage_file_name).convert()

class Button(object):
#此类书写借鉴网上代码 http://www.cnblogs.com/SRL-Southern/p/4949624.html
    def __init__(self, upimage, downimage, position):
        self.imageUp = pygame.image.load(upimage).convert_alpha()
        self.imageDown = pygame.image.load(downimage).convert_alpha()
        self.position = position

    def isOver(self):
        point_x, point_y = pygame.mouse.get_pos()
        x, y = self.position
        w, h = self.imageUp.get_size()

        in_x = x < point_x < x + w
        in_y = y  < point_y < y + h
        return in_x and in_y

    def render(self):
        w, h = self.imageUp.get_size()
        x, y = self.position

        if self.isOver():
            screen_start.blit(self.imageUp, (x , y ))
        else:
            screen_start.blit(self.imageDown, (x , y ))

class MySprite(pygame.sprite.Sprite):
    def __init__(self, target):
        pygame.sprite.Sprite.__init__(self)
        self.target_surface = target
        self.image = None
        self.master_image = None
        self.rect = None
        self.topleft = 0, 0
        self.frame = 0
        self.old_frame = -1
        self.frame_width = 1
        self.frame_height = 1
        self.first_frame = 0
        self.last_frame = 0
        self.columns = 1
        self.last_time = 0
        self.x = 0
        self.y = 0

    def setpos(self,x,y):
        self.x = x
        self.y = y

    def load(self, filename,width, height, columns):
        self.master_image = pygame.image.load(filename).convert_alpha()
        self.frame_width = width
        self.frame_height = height
        self.rect = self.x,self.y, width, height
        self.columns = columns
        rect = self.master_image.get_rect()
        self.last_frame = (rect.width // width) * (rect.height // height) - 1

    def update(self, current_time, rate=60):
        if current_time > self.last_time + rate:
            self.frame += 1
            if self.frame > self.last_frame:
                self.frame = self.first_frame
            self.last_time = current_time

        if self.frame != self.old_frame:
            frame_x = (self.frame % self.columns) * self.frame_width
            frame_y = (self.frame // self.columns) * self.frame_height
            rect = (frame_x, frame_y, self.frame_width, self.frame_height)
            self.image = self.master_image.subsurface(rect)
            self.old_frame = self.frame

#按钮
button_start = Button(startbut_up_file_name,startbut_down_file_name, (175,350))
button_end = Button(endbut_up_file_name,endbut_down_file_name, (175,450))
#背景音乐
bkmusic_filename = '/LearnPython/Sevepurebk.wav'
pygame.mixer.init()
pygame.mixer.music.load(bkmusic_filename)
#动图
framerate = pygame.time.Clock()
bomb = MySprite(screen_start)
group = pygame.sprite.Group()
group.add(bomb)

def DrawStarPage():
    flag = 0
    pygame.mixer.music.play(1)
    bomb.load("startpink.png", 640, 640, 4)
    while True:
        framerate.tick(1000)
        ticks = pygame.time.get_ticks()
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                   # pygame.quit()
                exit()
            if event.type == pygame.MOUSEBUTTONDOWN:
               if button_end.isOver():
                   pygame.quit()
                   exit()
               elif button_start.isOver():
                   flag = 1
                   break
        if flag == 1:
            break
        #screen_start.blit(startpage, (0, 0))
        screen_start.fill((255, 25, 255))
        group.update(ticks)
        group.draw(screen_start)
        button_start.render()
        button_end.render()

        pygame.time.delay(100)
        pygame.display.update()


