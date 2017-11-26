import serial
import subprocess

ser = serial.Serial('COM3', 9600)

ch = ser.read()
ser.write("o".encode())

while True:
    temp = float(ser.readline().strip())
    level = float(ser.readline().strip())

    proc = subprocess.Popen("php ../Web/artisan serial", shell=True, stdin=subprocess.PIPE, stdout=subprocess.PIPE)

    proc.stdin.write(str(temp).encode())
    proc.stdin.write("|".encode())
    proc.stdin.write(str(level).encode())
    proc.stdin.write("\n".encode())
    proc.stdin.close()

    ech = str(proc.stdout.readline().decode("UTF-8").strip()).split("|")

    for echo in ech:
        ser.write(echo.encode())

