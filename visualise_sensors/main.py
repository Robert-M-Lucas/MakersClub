import math

import pygame
import serial

# ! Doesn't work well with Python 3.12 !

ser = serial.Serial(port="/dev/ttyACM1")

pygame.init() # intialize the library

# Set up the drawing window
screen = pygame.display.set_mode([1500, 1000])

# Run until the user asks to quit
running = True
while running:
    # Check if the user clicked the close button so that they dont keep playing for like
    # 2 years
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            running = False

    # Fill the background with white (R, G, B) (also if you dont like lightmode, just replace it with (0,0,0))
    screen.fill((255, 255, 255))

    while True:
        try:
            data = [int(x) for x in ser.readline().strip().split(b';')]
            if len(data) != 6:
                raise ValueError
            break
        except:
            pass

    points = []
    INVERSE_CONST = 220
    DIAGONAL_INVERSE_CONST = math.sqrt((INVERSE_CONST ** 2) / 2)
    points.append((-INVERSE_CONST+data[0], 0))
    rs = math.sqrt((data[1] * data[1]) / 2)
    points.append((-DIAGONAL_INVERSE_CONST+rs, -DIAGONAL_INVERSE_CONST+rs))
    points.append((0, -INVERSE_CONST+data[2]))
    points.append((0, -INVERSE_CONST+data[5]))
    rs = math.sqrt((data[4] * data[4]) / 2)
    points.append((DIAGONAL_INVERSE_CONST-rs, -DIAGONAL_INVERSE_CONST+rs))
    points.append((INVERSE_CONST-data[3], 0))

    points = [(x * 3.5, y * 3.5) for x, y in points]

    points = [(730 + x, 950 + y) for x, y in points[:3]] + [(770 + x, 950 + y) for x, y in points[3:]]

    pygame.draw.line(screen, (0, 0, 0), (730, 950), points[0], width=4)
    pygame.draw.line(screen, (0, 0, 0), (730, 950), points[1], width=4)
    pygame.draw.line(screen, (0, 0, 0), (730, 950), points[2], width=4)

    pygame.draw.line(screen, (0, 0, 0), (770, 950), points[3], width=4)
    pygame.draw.line(screen, (0, 0, 0), (770, 950), points[4], width=4)
    pygame.draw.line(screen, (0, 0, 0), (770, 950), points[5], width=4)

    for i in range(len(points) - 1):
        pygame.draw.line(screen, (255, 0, 0), points[i], points[i+1])

    # Flip the display
    pygame.display.flip()

# Quit the program
pygame.quit()