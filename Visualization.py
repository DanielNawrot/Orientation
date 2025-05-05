import serial
import time
import pygame
import math

ser = serial.Serial("COM3", "115200")
time.sleep(2)

pygame.init()
WIDTH = 800
HEIGHT = 800
screen = pygame.display.set_mode((WIDTH, HEIGHT))
clock = pygame.time.Clock()
FPS = 30

position = pygame.Vector2(0, 0)
## def start():
def drawMain(screen, position, orientationD, length) :
    adjustmentX = WIDTH / 2
    adjustmentY = HEIGHT / 2
    adjustedPosition = pygame.Vector2(position.x + adjustmentX, position.y + adjustmentY)
    orientation = -math.radians(orientationD)
    pygame.draw.line(screen, "blue", adjustedPosition, (adjustedPosition.x + (length * math.sin(orientation)), adjustedPosition.y + (length * math.cos(orientation))), 4)
    #pygame.draw.polygon(screen, "blue", [(position.x + adjustmentX + (5 * math.sin(orientation)), position.y + adjustmentY + (5 * math.cos(orientation))),
    #                                     (position.x + adjustmentX - (3 * math.sin(orientation)), position.y + adjustmentY - (3 * math.cos(orientation))),
    #                                     (position.x + adjustmentX + (3 * math.sin(orientation)), position.y + adjustmentY - (3 * math.sin(orientation)))],
    #                                     3)

running = True
while running:

    for event in pygame.event.get():
        if event.type == pygame.QUIT or event.type == pygame.K_ESCAPE:
            running = False

    
    line = ser.readline().decode().strip()
    if line:
        try:
            time_ms, x, y, z, dist = map(float, line.split(","))
            print(f"Time {time_ms}s, Orientation: ({x}, {y}, {z}), Distance: {dist}cm")
            screen.fill("black")


            ## DISPLAY
            drawMain(screen, position, x, 30)

            pygame.display.flip()
            # Processing
        except ValueError:
            print(f"Invalid line: {line}")

    clock.tick(FPS)
pygame.quit()
