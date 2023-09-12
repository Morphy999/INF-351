import serial
import pygame

porta = "COM3"
velocidade = 9600

conecao = serial.Serial(porta,velocidade)

pygame.init()

def write(x):
    print(x)
    conecao.write(bytes(x, 'utf-8'))
    return

def read():
    data = input()
    return data

while True:
    data1 = read()
    write(data1)
    


