"""

LEER SWARM - FORMACIONES
Cada accin es un color
probar libre y con obstaculos (2) - presentar los casos de 1,2 agentes con el mismo mundo de (4,5).

 Example program to show using an array to back a grid on-screen.

 Sample Python/Pygame Programs
 Simpson College Computer Science
 http://programarcadegames.com/
 http://simpson.edu/computer-science/

 Explanation video: http://youtu.be/mdTeqiWyFnc

 Modified date: MAY 15 2017
 Modified by: GIAN NESTOR CUELLO
"""
import time

import math
import pygame
import pytweening as tween
# Define some colors
BLACK = (0, 0, 0)
WHITE = (255, 255, 255)
GREEN = (0, 255, 0)
RED = (255, 0, 0)


# This sets the margin between each cell
MARGIN = 1

x= 3 #squares per row
y= 3  #squares per column

# indexes needed
spaces =  x * y

#size of each space
WIDTH  = 99#(w_Height-(1*2))/y
HEIGHT = 99#(w_Width-(1*2))/x

#dimensions of window
w_Height = y * WIDTH
w_Width  = x * HEIGHT

print "Tiles Wanted ",spaces
print "WIDTH ", WIDTH, " HEIGHT " , HEIGHT,"Tile Area ", WIDTH * HEIGHT
print "Margin ", MARGIN
print "WINDOW Dim ",w_Height * w_Width
print " spaces not covered ", (MARGIN * spaces)

list_xy= ((10,16),(59,109),(40,49),(70,16))

#list_grid = [[0,0],[0,1],[1,0],[1,1],[2,0],[2,1],[3,0],[3,1],[4,0],[4,1]]
#list_grid= [[0,0],[1,0],[1,1],[0,1]]
# list_grid= [[0,0],[0,1],[1,0],[1,1],[2,0],[2,1]]
# list_grid= [[0,0],[1,0],[2,0],[2,1],[1,1],[0,1]]


swarm = 1
list_grid= [[0,0],[1,0],[2,0],[2,1],[1,1],[0,1],[0,2],[1,2],[2,2]]
# 3x3

# swarm = 2
# list_grid= [[0,0],[0,1],
#             [1,0],[1,1],
#             [2,0],[2,1],
#             [3,0],[2,2],
#             [3,1],[1,2],
#             [3,2],[0,2],
#             [3,3],[0,3],
#             [2,3],[0,3],
#             [1,3],[0,2],
#             [0,3],[1,2]]
#  4x4

# A = 0 + 2
# B = 1 + 2

def convert_to_pixels(_x, _y):
    print _x, " < ", x , "  ", _y, " < ",y
    if _x < y and _y < x:
        _xpixel = WIDTH*_x
        _ypixel = HEIGHT*_y
        return (_xpixel + 20),(_ypixel+20)
    return -1,-1
print convert_to_pixels(0,0)
print convert_to_pixels(0,1)
print convert_to_pixels(1,0)
print convert_to_pixels(1,1)
print convert_to_pixels(2,0)
print convert_to_pixels(1,2)

class Agent(pygame.sprite.Sprite):
    def __init__(self,posx,posy,_index):
        #sprite calling
        pygame.sprite.Sprite.__init__(self)

        #Create an agent image, and place it on the world
        self.index = _index
        self.gridx = 0
        self.gridy = 0
        self.rect = pygame.Rect(posx,posy, 50, 50)
        self.image = pygame.image.load("multicopter.png")
        self.image = pygame.transform.scale(self.image,(50,50))


    def update(self):
        error1 = 3
        error2 = 3
        flag = True
        # if (self.rect.x -xpix != 3) and (self.rect.y - ypix!= 3):
        if  self.index<len(list_grid):
            # print " Need ",self.index , " Len ", len(list_grid)
            xpix,ypix= convert_to_pixels(list_grid[self.index][0],list_grid[self.index][1])
            self.gridx = list_grid[self.index][0]
            self.gridy = list_grid[self.index][1]
            while(flag):
                # Because rect.x and rect.y are automatically converted
                # to integers, we need to create different variables that
                # store the location as floating point numbers. Integers
                # are not accurate enough for aiming.
                floating_point_x = float(self.rect.x)
                floating_point_y = float(self.rect.y)

                # Calculation the angle in radians between the start points
                # and end points. This is the angle the bullet will travel.
                x_diff = xpix - self.rect.x
                y_diff = ypix - self.rect.y
                angle = math.atan2(y_diff, x_diff);

                # Taking into account the angle, calculate our change_x
                # and change_y. Velocity is how fast the bullet travels.
                velocity = 5
                change_x = math.cos(angle) * velocity
                change_y = math.sin(angle) * velocity
                # self.rect.center=goal
                # The floating point x and y hold our more accurate location.
                floating_point_y += change_y
                floating_point_x += change_x

                # The rect.x and rect.y are converted to integers.
                self.rect.y = int(floating_point_y)
                self.rect.x = int(floating_point_x)
                # print "x",self.rect.x, " y " ,self.rect.y
                print abs(self.rect.x - xpix) ,"<=",error1 , " ",abs(self.rect.y - ypix),"<=", error2
                if (abs(self.rect.x - xpix) <=error1  ) and ( abs(self.rect.y - ypix) <=error2):
                    flag = False
                    self.index += swarm



# Create a 2 dimensional array. A two dimensional
# array is simply a list of lists.
grid = []
for row in range(x):
    # Add an empty array that will hold each cell
    # in this row
    grid.append([])
    for column in range(y):
        grid[row].append(0)  # Append a cell

# Set row 1, cell 5 to one. (Remember rows and
# column numbers start at zero.)
grid[list_grid[0][0]][list_grid[0][1]] = 1
# grid[list_grid[swarm][0]][list_grid[swarm][1]] = 1

# Initialize pygame
pygame.init()
# Set the HEIGHT and WIDTH of the screen
WINDOW_SIZE = [w_Height, w_Width]
screen = pygame.display.set_mode(WINDOW_SIZE)

agent_list=pygame.sprite.Group()

all_sprites_list=pygame.sprite.Group()

#create the sprites
for i in range(swarm):
    print "Agent ",i
    xpix,ypix=convert_to_pixels(list_grid[i][0],list_grid[i][1])
    print xpix,ypix
    agent = Agent(xpix,ypix,i)
    agent_list.add(agent)
    all_sprites_list.add(agent_list)

# Set title of screen
pygame.display.set_caption("LEARNING WORLD")

# Loop until the user clicks the close button.
done = False

# Used to manage how fast the screen updates
clock = pygame.time.Clock()

ticks=0

animate = False
# -------- Main Program Loop -----------
while not done:
    for event in pygame.event.get():  # User did something
        if event.type == pygame.QUIT:  # If user clicked close
            done = True  # Flag that we are done so we exit this loop
        elif event.type == pygame.MOUSEBUTTONDOWN:
            if animate:
                animate=False
            else:
                animate = True
            # User clicks the mouse. Get the position
            pos = pygame.mouse.get_pos()
            # Change the x/y screen coordinates to grid coordinates
            column = pos[0] // (WIDTH + MARGIN)
            row = pos[1] // (HEIGHT + MARGIN)
            # Set that location to one
            if grid[row][column]:
                grid[row][column]=0
            else:
                grid[row][column] = 1
            print("Click ", pos, "Grid coordinates: ", row, column)

    for agent in agent_list:
        grid[agent.gridy][agent.gridx] = 1
        # print "Element pos ",agent.gridx, " ", agent.gridy
            #
#    index = 1
    # if animate:
    all_sprites_list.update( )


    # Set the screen background
    screen.fill(BLACK)


    # Draw the grid
    for row in range(x):
        for column in range(y):
            color = WHITE
            if grid[row][column] == 1:
                color = GREEN
            pygame.draw.rect(screen,
                             color,
                             [(MARGIN + WIDTH) * column + MARGIN,
                              (MARGIN + HEIGHT) * row + MARGIN,
                              WIDTH,
                              HEIGHT])

    all_sprites_list.draw(screen)
    # Limit to 60 frames per second

    ticks = clock.tick(1)

    # Go ahead and update the screen with what we've drawn.
    pygame.display.flip()

# Be IDLE friendly. If you forget this line, the program will 'hang'
# on exit.
pygame.quit()
