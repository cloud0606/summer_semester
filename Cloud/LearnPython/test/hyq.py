#代码模仿之源：
#http://eyehere.net/2011/python-pygame-novice-professional-index/

import pygame
if pygame.font is None:
    print("no")
else:
    print("yes")

background_file_name = 'bk.jpg'
#mouse_file_name = 'mouse.jpg'
mouse_file_name = 'mouse_sheep.png'
from pygame.locals import *
from sys import exit

pygame.init()

screen = pygame.display.set_mode((1200,650),0,32)

pygame.display.set_caption("接鸡蛋哦哦哦")

background = pygame.image.load(background_file_name).convert()
mouse = pygame.image.load(mouse_file_name).convert_alpha()

while True:
    for event in pygame.event.get():
        if event.type == QUIT:
            exit()
    screen.blit(background,(0,0))

    x,y = pygame.mouse.get_pos()
    x -= mouse.get_width() / 2
    y -= mouse.get_height() / 2

    screen.blit(mouse,(x,y))

    pygame.display.update()






