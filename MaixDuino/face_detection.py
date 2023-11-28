import sensor
import image
import lcd
import KPU as kpu         #Neural Network Processor

lcd.init()                      #inicializa lcd
sensor.reset()                  #inicaliza camera
sensor.set_pixformat(sensor.RGB565)   #Coloca a camera no formato default
sensor.set_framesize(sensor.QVGA)     #resoluçao 320x240
sensor.run(1)

task = kpu.load(0x300000) ##task = kpu.load("/sd/face.kmodel")         #carrega o modelo
anchor = (1.889, 2.5245, 2.9465, 3.94056, 3.99987, 5.3658, 5.155437, 6.92275, 6.718375, 9.01025) #Valores ja pre-definidos no treinamento do modelo
a = kpu.init_yolo2(task, 0.5, 0.3, 5, anchor) # YOLO V2
while True:
        img = sensor.snapshot() #pega imagem
        code = kpu.run_yolo2(task, img) #roda imagem na CNN
        if code:           #Se a probabilidade é maior que a predefinida na funçao (kpu.init_yolo2)
            for i in code:
                print(i)
                a = img.draw_rectangle(i.rect())  #desenha o retangulo
        a = lcd.display(img)
a = kpu.deinit(task)
