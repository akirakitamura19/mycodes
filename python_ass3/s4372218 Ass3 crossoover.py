
#######################################################################
# This module use crossover and mutation for creating next generation #
#######################################################################

import pygame
from pygame.locals import *
import random
import sys
import math
import copy


SCR_RECT = Rect(0, 0, 800, 600)  # screen size
LEFT, MIDDLE, RIGHT = -1, 0, 1   # define the direction
UP, MIDDLE, DOWN = -1, 0, 1


class RedMan(object):
    """
    set up basic information of Team B  
    """
    def __init__(self, speed):
        self.speed = speed
        self.x = 0
        self.y = 0
        self.x_direction = MIDDLE 
        self.y_direction = MIDDLE 
        self.life = 0
        self.command = []

        
class WhiteMan(object):
    """
    set up basic information of Team A
    """
    def __init__(self, speed):
        self.x = 0
        self.y = 0
        self.speed = speed


class Crossover(object):
    """
    set up crossover function to return best two gene in the generation
    list[floats] -> list[integers], list[integers]
    """
    def __init__(self, score):
        #make sure to copy the score 
        self.cross_score = copy.deepcopy(score)


    def find2gene(self):
        #make best two gene to take over them for next generation
        self.cross_score.sort()
        best1gene = self.cross_score[0]
        best2gene = self.cross_score[1]
        return best1gene, best2gene


        
class Main:
    """
    set up master class of this project
    """
    def __init__(self):
        """
        set up basic information of pygame
        """
        pygame.init()
        screen = pygame.display.set_mode(SCR_RECT.size)
        pygame.display.set_caption("GAs TAGGING")  #the title name 
        self.font = pygame.font.SysFont(None, 25)  #font size
        clock = pygame.time.Clock()                #timer
        
        #set up default numbers of valuables 
        self.generation = 0
        self.gene = 0
        self.framenum = 0
        self.command = 0
        self.score = [0,0,0,0,0,0,0,0,0,0] #recode the distance 10 times every generation

        #set up the default numbers of encoding of teamB's gene
        # -> [[0,0,0...][0,0,0...]....[...0,0,0]]
        self.redgene = \
        [[[0] * 21 for i in range(10)],
         [[0] * 21 for i in range(10)],
         [[0] * 21 for i in range(10)],
         [[0] * 21 for i in range(10)],
         [[0] * 21 for i in range(10)],
         [[0] * 21 for i in range(10)],
         [[0] * 21 for i in range(10)],
         [[0] * 21 for i in range(10)],
         [[0] * 21 for i in range(10)],
         [[0] * 21 for i in range(10)]]

        #the change the gene of the 0th generation
        for i in range(9):
            self.redgene[i][random.randrange(10)][random.randrange(20)] = random.randrange(9)
        
        #set up 10 players of teamB
        self.red =[RedMan(1.5),
                   RedMan(2.0),
                   RedMan(2.5),
                   RedMan(3.0),
                   RedMan(3.5),
                   RedMan(3.5),
                   RedMan(3.0),
                   RedMan(2.5),
                   RedMan(2.0),
                   RedMan(1.5)]

        #set up 6 players of teamA
        self.white=[WhiteMan(1.0),
                    WhiteMan(2.0),
                    WhiteMan(3.0),
                    WhiteMan(2.0),
                    WhiteMan(1.0),
                    WhiteMan(3.0)]
        
        #set up initializating function
        self.initialize(self.gene)


        #set up main loop function
        while True:
            self.update()            #updating function to run new game
            Clock = clock.tick(700)  #game speed
            self.draw(screen)        #drawing function on screen
            pygame.display.update()  

            #set up quit function
            for event in pygame.event.get():
                if event.type == QUIT:
                    pygame.quit()
                    sys.exit()
                    
            for event in pygame.event.get():
                if self.get_goal >= 4:
                    pygame.quit()
                    sys.exit()
                    


    def initialize(self, gene):
        """
        set up initialization function to run new game
        """
        self.framenum = 0      #initialize time
        self.command = 0
        
        #initialize the position of players and command of teamB
        for i in range(10):
            self.red[i].x = 600.0
            self.red[i].y = 75.0 + 50.0*i
            self.red[i].x_direction = MIDDLE
            self.red[i].y_direction = MIDDLE
            self.red[i].life = 0
            self.red[i].command = self.redgene[gene][i]

        #initialize the position of players of teamA
        for i in range(5):
            self.white[i].x = 200.0
            self.white[i].y = 100.0 + 100.0*i
        self.white[5].x = 150.0
        self.white[5].y = 300.0
    
    def distance(self, x, y):
        """
        set up the function of distance between 2 points
        """
        return math.sqrt(x**2+y**2)

   
    def evaluate(self, gene):
        """
        set up evaluation/fitness with distance between the goal and teamB
        """
        score = 0
        self.get_goal =0
        for i in range(10):
            distance_goal =self.distance((100.0 - self.red[i].x),(300.0 - self.red[i].y))
            score += distance_goal
            if distance_goal < 30:
                self.get_goal += 1
        self.score[gene] = score




    def update(self):
        """
        set up the updating function
        """
        if self.framenum == 1000:
            self.evaluate(self.gene)     #evaluation of fitness
            
            #set up the next generation
            if self.gene == 9:
                
                #check the best gene(minimum distance) in the generation
                bestgene = self.score.index(min(self.score))                
                bestscore = self.redgene[bestgene]
                
                #set up 9 genes by using crossover of best two genes 
                self.crossover = Crossover(self.score)
                best1gene, best2gene = self.crossover.find2gene()
                self.best1 = self.redgene[self.score.index(best1gene)]
                self.best2 = self.redgene[self.score.index(best2gene)]

                self.newgene0 = []
                self.newgene1 = []
                self.newgene2 = []
                self.newgene3 = []
                self.newgene4 = []
                self.newgene5 = []
                self.newgene6 = []
                self.newgene7 = []
                self.newgene8 = []

                for i in range(10):
                    a = random.randrange(21)
                    new1 = self.best1[i][:a] + self.best2[i][a:]
                    new2 = self.best2[i][:a] + self.best1[i][a:]
                    b = random.randrange(21)
                    new3 = self.best1[i][:b] + self.best2[i][b:]
                    new4 = self.best2[i][:b] + self.best1[i][b:]
                    c = random.randrange(21)
                    new5 = self.best1[i][:c] + self.best2[i][c:]
                    new6 = self.best2[i][:c] + self.best1[i][c:]
                    d = random.randrange(21)
                    new7 = self.best1[i][:d] + self.best2[i][d:]
                    new8 = self.best2[i][:d] + self.best1[i][d:]
                    e = random.randrange(21)
                    new9 = self.best1[i][:e] + self.best2[i][e:]
                    self.newgene0.append(new1)
                    self.newgene1.append(new2)
                    self.newgene2.append(new3)
                    self.newgene3.append(new4)
                    self.newgene4.append(new5)
                    self.newgene5.append(new6)
                    self.newgene6.append(new7)
                    self.newgene7.append(new8)
                    self.newgene8.append(new9)


                #print every generation's information
                if self.generation % 1 == 0:
                    print("Generetion: {0}".format(self.generation))
                    print("Best gene num: {0}".format(bestgene))
                    print("Distance to the goal: {0:.2f}".format(min(self.score)))
                    print("reach the goal: {0}".format(self.get_goal))
                    print("")
                    
                #take over the best gene for next generation
                self.redgene[0] = self.newgene0
                self.redgene[1] = self.newgene1
                self.redgene[2] = self.newgene2
                self.redgene[3] = self.newgene3
                self.redgene[4] = self.newgene4
                self.redgene[5] = self.newgene5
                self.redgene[6] = self.newgene6
                self.redgene[7] = self.newgene7
                self.redgene[8] = self.newgene8
                self.redgene[9] = bestscore
                for i in range(10):
                    self.score[i] = 0.0      #make sure to inisialize the sore
                    
                #set up the mutating  9 out of 10 genes
                for i in range(9):
                    self.redgene[i][random.randrange(10)][random.randrange(20)]= random.randrange(9)
                    
                #update the screen information
                self.generation += 1
                self.gene = 0

            #if not changing the generation, run next gene
            else:
                self.gene += 1
            self.initialize(self.gene)

            
        #set up the direction of teamB
        if self.framenum % 50 == 0:
            #update the direction every 1 second
            self.command += 1
            for i in range(10):
                #if being caught, the player pauses
                if self.red[i].life > 0:
                    self.red[i].life -= 1;
                #deside the direction
                if self.red[i].command[self.command] % 9 == 1:
                    self.red[i].x_direction = LEFT
                    self.red[i].y_direction = MIDDLE
                if self.red[i].command[self.command] % 9 == 2:
                    self.red[i].x_direction = LEFT
                    self.red[i].y_direction = UP
                if self.red[i].command[self.command] % 9 == 3:
                    self.red[i].x_direction = MIDDLE
                    self.red[i].y_direction = UP
                if self.red[i].command[self.command] % 9 == 4:
                    self.red[i].x_direction = RIGHT
                    self.red[i].y_direction = UP
                if self.red[i].command[self.command] % 9 == 5:
                    self.red[i].x_direction = RIGHT
                    self.red[i].y_direction = MIDDLE
                if self.red[i].command[self.command] % 9 == 6:
                    self.red[i].x_direction = RIGHT
                    self.red[i].y_direction = DOWN
                if self.red[i].command[self.command] % 9 == 7:
                    self.red[i].x_direction = MIDDLE
                    self.red[i].y_direction = DOWN
                if self.red[i].command[self.command] % 9 == 8:
                    self.red[i].x_direction = LEFT
                    self.red[i].y_direction = DOWN

        #If not update the direction, the player of teamB keep moving the same direction
        for i in range(10):
            if self.red[i].life == 0:
                if self.red[i].x < 800 and self.red[i].x > 0:
                    self.red[i].x += self.red[i].x_direction*self.red[i].speed
                if self.red[i].y < 600 and self.red[i].y > 0:
                    self.red[i].y += self.red[i].y_direction*self.red[i].speed


        #set up the direction of teamA
        #chase the closest player of B who activate
        for i in range(5):
            distance = sys.maxsize
            target = 11
            for j in range(10):
                if self.red[j].life == 0:
                    tmp = self.distance((self.white[i].x - self.red[j].x),(self.white[i].y - self.red[j].y))
                    if tmp < distance:
                        distance = tmp
                        target = j
            if target !=11:
                if self.white[i].x > self.red[target].x:
                    self.white[i].x -= self.white[i].speed
                if self.white[i].x < self.red[target].x:
                    self.white[i].x += self.white[i].speed
                if self.white[i].y > self.red[target].y:
                    self.white[i].y -= self.white[i].speed
                if self.white[i].y < self.red[target].y:
                    self.white[i].y += self.white[i].speed

        #one of the team A stick the goal and catch player who is close to the goal
        distance = sys.maxsize
        target = 11
        for j in range(10):
            tmp = self.distance((100.0 - self.red[j].x),(300.0 - self.red[j].y))
            if tmp > 30 and tmp < 120 and tmp < distance:
                distance = tmp
                target = j
        if target != 11:
            if self.white[5].x > self.red[target].x:
                self.white[5].x -= self.white[5].speed
            if self.white[5].x < self.red[target].x:
                self.white[5].x += self.white[5].speed
            if self.white[5].y > self.red[target].y:
                self.white[5].y -= self.white[5].speed
            if self.white[5].y < self.red[target].y:
                self.white[5].y += self.white[5].speed
        
        #if being caught or reach the goal, the player have to pause for 4 seconds 
        for i in range(10):
            get_goal_num = 0
            for j in range(6):
                if self.distance((self.red[i].x-self.white[j].x),(self.red[i].y-self.white[j].y)) < 2.0:
                    self.red[i].life = 4 
            if self.distance((self.red[i].x-100.0),(self.red[i].y-300.0)) < 30.0:
                self.red[i].life = 4
                
        #update the time
        self.framenum += 1


            
    def draw(self, screen):
        """
        set up the drawing function of the players and the goal
        """
        #draw the back screen to be black  
        pygame.draw.rect(screen, (0,0,0), Rect(0,0,800,600))
        
        #draw the goal
        pygame.draw.circle(screen, (200,200,200), (100, 300), 30)
        
        #draw the team B depending on the activate or not
        for i in range(10):
            if self.red[i].life == 0:
                pygame.draw.circle(screen, (255,0,0), (int(self.red[i].x),int(self.red[i].y)),6)
            else:
                pygame.draw.circle(screen, (100,0,0), (int(self.red[i].x),int(self.red[i].y)),6)
                
        #draw the team A to be white and blue
        for i in range(5):
            pygame.draw.circle(screen, (255,255,255), (int(self.white[i].x),int(self.white[i].y)),6)
        pygame.draw.circle(screen, (150,150,255), (int(self.white[5].x),int(self.white[5].y)),6)
        
        #draw the information of current generation and the gene on the screen
        info = self.font.render("generation "+str(self.generation)+" gene "+str(self.gene), True, (255,255,255))
        screen.blit(info, (0,0))


        

#run the game
if __name__ == "__main__":
    Main()
