######画初始窗口
import pygame
from sys import exit

pygame.init()
startpage_file_name = 'startpage.jpg'
startbut_up_file_name = 'start.png'
startbut_down_file_name = '_start.png'
endbut_up_file_name = 'end.png'
endbut_down_file_name = '_end.png'

screen_start = pygame.display.set_mode((960,640),0,32)#Alpha通道用32
pygame.display.set_caption("起始页")
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

button_start = Button(startbut_up_file_name,startbut_down_file_name, (320,350))
button_end = Button(endbut_up_file_name,endbut_down_file_name, (320,450))

def DrawStarPage():
    while True:
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
               # pygame.quit()
                exit()
        if event.type == pygame.MOUSEBUTTONDOWN:
                if button_end.isOver():
                    pygame.quit()
                    exit()
                elif button_start.isOver():
                    break
                    #score= 0
        screen_start.blit(startpage, (0, 0))
        #screen.fill((200, 200, 200))
        button_start.render()
        button_end.render()
        pygame.display.update()

#DrawStarPage()

DrawStarPage()