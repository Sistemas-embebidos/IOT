import matplotlib.pyplot as plt
import matplotlib.animation as animation
from matplotlib import style
import numpy as np
import random
import serial
import time
import datetime as dt

# Inicializamos el puerto serie
ser = serial.Serial()
ser.port = 'COM9' # Puerto serie (para windows) /dev/USB01
ser.baudrate = 115200
ser.timeout = 5 # Tiempo sin leer
ser.open()
if ser.is_open==True:
	print("\n Perfecto! El puerto serie esta abierto. Configuraciones: :\n")
	print(ser, "\n") 

# Creacion de figuras

fig, ax = plt.subplots(2,2)
plt.subplots_adjust(hspace = 1)

t_s = [] # Para almacenar los tiempos
g_s = [] # Para almacenar los valores del giroscopio
a_s = [] # Para almacenar los valores de aceleracion
m_s = [] # Para almacenar los valores del campo magnetico
temp_s = [] # Para almacenar los valores de temperatura

N = 50      # Cantidad de lecturas a mostrar

#%%
def modulo(x,y,z):
    return (x**2+y**2+z**2)**(0.5)
#%%

# Funcion llamada periodicamente desde FuncAnimation
def animate(i, t_s, g_s, a_s, m_s, temp_s):

    # Leemos y parseamos desde el puerto serie
    line=ser.readline()              # ASCII
    line_as_list = line.split(b',')  # Convierto a binario y parseo con ','

    #print(line_as_list)

    gyro_x = float(line_as_list[0])
    gyro_y = float(line_as_list[1])
    gyro_z = float(line_as_list[2])

    giroscopio = modulo(gyro_x,gyro_y,gyro_z)
    #print(giroscopio)

    acc_x = float(line_as_list[3])
    acc_y = float(line_as_list[4])
    acc_z = float(line_as_list[5])
    
    acelerometro = modulo(acc_x,acc_y,acc_z)
    #print(acelerometro)

    mag_x = float(line_as_list[6])  
    mag_y = float(line_as_list[7])
    mag_z = float(line_as_list[8])
    
    magnetometro = modulo(mag_x,mag_y,mag_z)
    #print(magnetometro)

    temp = float(line_as_list[9])
    #print(temp)

    print('[[{:.3f},{:.3f},{:.3f}]rads,[{:.3f},{:.3f},{:.3f}]ms^2,[{:.3f},{:.3f},{:.3f}]uT,{:.3f}°C]'.format(gyro_x,gyro_y,gyro_z,acc_x,acc_y,acc_z,mag_x,mag_y,mag_z,temp))
    print('[{:.3f} rads | {:.3f} ms^2 | {:.3f} uT,{:.3f} °C]'.format(giroscopio,acelerometro,magnetometro,temp))

   	# Agrego los valores a la lista

    t_s.append(dt.datetime.now().strftime('%H:%M:%S')) 
    g_s.append(giroscopio)
    a_s.append(acelerometro)
    m_s.append(magnetometro)
    temp_s.append(temp)

    # Limito a N ultimos elementos
    t_s = t_s[-N:]
    g_s = g_s[-N:]
    a_s = a_s[-N:]
    m_s = m_s[-N:]
    temp_s = temp_s[-N:]

    # Limpiar figuras
    ax[0,0].clear()
    ax[0,1].clear()
    ax[1,0].clear()
    ax[1,1].clear()

    for i in range(2):
        for j in range(2):
            for tick in ax[i,j].get_xticklabels():
                tick.set_rotation(45)

    unidad = "[rads]"
    etiqueta = "Giroscopio" + unidad
    ax[0,0].plot(t_s, g_s, label = etiqueta) 
    ax[0,0].set_title(etiqueta)
    ax[0,0].set_ylabel(unidad)
    ax[0,0].legend()

    unidad = "[m/s^2]"
    etiqueta = "Aceleracion" + unidad
    ax[0,1].plot(t_s, a_s, label = etiqueta)
    ax[0,1].set_title(etiqueta)
    ax[0,1].set_ylabel(unidad)
    ax[0,1].legend()

    unidad = "[uT]"
    etiqueta = "Magnetismo" + unidad
    ax[1,0].plot(t_s, m_s, label = etiqueta)
    ax[1,0].set_title(etiqueta)
    ax[1,0].set_ylabel(unidad)
    ax[1,0].legend()

    unidad = "[°C]"
    etiqueta = "Temperatura" + unidad
    ax[1,1].plot(t_s, temp_s, label = etiqueta)
    ax[1,1].set_title(etiqueta)
    ax[1,1].set_ylabel(unidad)
    ax[1,1].legend()
    
    # Format plot
    plt.xticks(rotation=45, ha='right')
    plt.subplots_adjust(bottom=0.30)

    #plt.axis([1, None, 0, 1.1]) # Para ejes fijos

# Set up plot to call animate() function periodically
ani = animation.FuncAnimation(fig, animate, fargs=(t_s,  g_s, a_s, m_s, temp_s), interval=1000)

plt.show()