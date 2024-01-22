from dataclasses import dataclass

import math

import pygame
import serial


@dataclass
class SensorConfig:
    name: str
    reading: int = 0
    x_mul: int = 0
    y_mul: int = 0
    x_offset: int = 0
    y_offset: int = 0
    floor: int = 0
    ceiling: int = 1023
    scaled: int = 0
    inv_sqr: int = 0

    def update(self):
        self.scaled = int((max(0, min(self.reading, self.ceiling) - self.floor) / (self.ceiling - self.floor)) * 1023)
        inv_sqr_min, inv_sqr_max = math.sqrt(1 / 1024), 1
        scaled = max(1, self.scaled)
        inv_sqr = math.sqrt(1 / scaled)
        self.inv_sqr = int((inv_sqr - inv_sqr_min) / (inv_sqr_max - inv_sqr_min) * 1023)


# ! Doesn't work well with Python 3.12 !

SPOOF_DATA = False

if not SPOOF_DATA:
    ser = serial.Serial(port="/dev/ttyACM1")
else:
    ser = None

pygame.init()  # Initialise the library

WIDTH, HEIGHT = 1500, 1000
screen = pygame.display.set_mode([WIDTH, HEIGHT])

sensors = [
    SensorConfig(f"Sensor LS"),
    SensorConfig(f"Sensor LA"),
    SensorConfig(f"Sensor LF"),
    SensorConfig(f"Sensor RF"),
    SensorConfig(f"Sensor RA"),
    SensorConfig(f"Sensor RS"),
]

sensors[0].x_mul = -1
sensors[1].x_mul = -math.sqrt(2) / 2
sensors[4].x_mul = math.sqrt(2) / 2
sensors[5].x_mul = 1
sensors[2].y_mul = -1
sensors[3].y_mul = -1
sensors[1].y_mul = -math.sqrt(2) / 2
sensors[4].y_mul = -math.sqrt(2) / 2

for i in (0, 1, 2):
    sensors[i].x_offset = -10

for i in (3, 4, 5):
    sensors[i].x_offset = 10

font = pygame.font.SysFont('consolas', 30)
font_bold = pygame.font.SysFont('consolas', 30, bold=True)

line_centre = (WIDTH / 2, HEIGHT - 20)

print("""\
Use Left and Right Arrows to select a sensor to configure
Use F/G to adjust sensor floor
Use C/V to adjust sensor ceiling or A to set it to the current sensor reading
Press Enter to send config to mouse\
""")

run = True
selected = 0
while run:
    keyboard = pygame.key.get_pressed()
    mul = 1
    if keyboard[pygame.K_LSHIFT]:
        mul = 10
    elif keyboard[pygame.K_LCTRL]:
        mul = 100
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            run = False
        if event.type == pygame.KEYDOWN:
            if event.key == pygame.K_LEFT:
                selected = max(0, selected - 1)
            elif event.key == pygame.K_RIGHT:
                selected = min(5, selected + 1)
            elif event.key == pygame.K_RETURN:
                for i in (0, 1, 2, 5, 4, 3):
                    ser.write(bytes(f"{sensors[i].floor}", encoding='ascii'))
                    ser.write(b";")
                    ser.write(bytes(f"{sensors[i].ceiling}", encoding='ascii'))
                    ser.write(b";")
            elif event.key == pygame.K_f:
                sensors[selected].floor = max(0, sensors[selected].floor - mul)
            elif event.key == pygame.K_c:
                sensors[selected].ceiling = max(0, sensors[selected].ceiling - mul)
            elif event.key == pygame.K_g:
                sensors[selected].floor = min(1023, sensors[selected].floor + mul)
            elif event.key == pygame.K_v:
                sensors[selected].ceiling = min(1023, sensors[selected].ceiling + mul)
            elif event.key == pygame.K_a:
                sensors[selected].ceiling = sensors[selected].reading
                selected = min(5, selected + 1)

    screen.fill((255, 255, 255))

    if not SPOOF_DATA:
        while True:
            try:
                data = [int(x) for x in ser.readline().strip().split(b';')]
                if len(data) != 6:
                    raise ValueError
                break
            except:
                pass
    else:
        data = [3 for _ in range(6)]

    for i in range(6):
        sensors[i].reading = data[i]
        sensors[i].update()

    points = []

    for sensor in sensors:
        ax, ay = line_centre
        ax += sensor.x_offset
        ay += sensor.y_offset

        bx = ax + ((sensor.inv_sqr / 1.5) * sensor.x_mul)
        by = ay + ((sensor.inv_sqr / 1.5) * sensor.y_mul)

        pygame.draw.line(screen, (0, 0, 0), (ax, ay), (bx, by), width=5)

        points.append((bx, by))

        bx = ax + ((sensor.reading / 1.5) * sensor.x_mul)
        by = ay + ((sensor.reading / 1.5) * sensor.y_mul)

        pygame.draw.line(screen, (0, 255, 255), (ax, ay), (bx, by), width=2)

    for i in range(len(points) - 1):
        pygame.draw.line(screen, (255, 0, 0), points[i], points[i + 1])

    for i, sensor_config in enumerate(sensors):
        string = sensor_config.name
        if selected == i:
            string = f"[{string}]"
        string += f"\nRaw:{sensor_config.reading}"
        string += f"\nFloor:{sensor_config.floor}"
        string += f"\nCeiling:{sensor_config.ceiling}"
        string += f"\nScaled:{sensor_config.scaled}"
        string += f"\nInv. Sqr:{sensor_config.inv_sqr}"
        offset = 0
        for s in string.split('\n'):
            if selected == i:
                text = font_bold.render(s, True, (150, 0, 0))
            else:
                text = font.render(s, True, (0, 0, 0))
            text_rect = text.get_rect()
            text_rect.center = ((WIDTH / 6) * i + (WIDTH / 12), 20 + offset)
            offset += text_rect.height
            screen.blit(text, text_rect)

    pygame.display.flip()

# Quit the program
pygame.quit()
