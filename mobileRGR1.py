import numpy as np
import matplotlib.pyplot as plt

user_name = []
user_full_name = []

name = input("Enter your name in Latin >> ")
surname = input("Enter your surname in Latin >> ")
full_name = name + surname

#Получение полного имени
def get_full_name():
    return full_name

# Кодирование имени в бинарный формат
def encode_to_binary(full_name):
    binary_name = []
    for char in full_name:
        binary_name.extend(list(format(ord(char), '08b')))
    return list(map(int, binary_name))

# Декодирование бинарных данных в строку
def decode_to_string(binary_name):
    decoded_name = ""
    for i in range(0, len(binary_name), 8):
        byte = binary_name[i:i+8]
        decoded_name += chr(int(''.join(map(str, byte)), 2))
    return decoded_name

# Вычисление циклического избыточного кода (CRC)
def calculate_crc(polynomial, extended_data, result, polynomial_length, data_length):
    temp = extended_data.copy()
    for i in range(data_length):
        if temp[i] == 1:
            for j in range(polynomial_length):
                temp[j+i] = temp[j+i] ^ polynomial[j]
    for i in range(polynomial_length-1):
        result[i] = temp[((polynomial_length-1 + data_length) - polynomial_length) + (i+1)]

# Сдвиг регистра X
def shift_register_x(register_x):
    temp = (register_x[2] + register_x[3]) % 2
    register_x = np.roll(register_x, 1)
    register_x[0] = temp
    return register_x

# Сдвиг регистра Y
def shift_register_y(register_y):
    temp = (register_y[1] + register_y[2]) % 2
    register_y = np.roll(register_y, 1)
    register_y[0] = temp
    return register_y

# Генерация последовательности Голда
def generate_gold_sequence(register_x, register_y, sequence, length):
    for i in range(length):
        sequence[i] = (register_x[4] + register_y[4]) % 2
        register_x = shift_register_x(register_x)
        register_y = shift_register_y(register_y)

# Вычисление корреляции
def correlation(gold_sequence, signal, sequence_length):
    max_correlation = -100.2
    max_index = 0
    temp_signal = np.copy(signal)
    temp_sequence = np.full(len(gold_sequence), fill_value=int(0))
    data_corr = []
    for i in range(len(temp_signal)):
        temp_signal = np.roll(signal, -i)
        temp_sequence = temp_signal[0:len(gold_sequence)]
        correlation_value = np.correlate(temp_sequence, gold_sequence)
        data_corr.append(correlation_value)
        if correlation_value > max_correlation:
            max_correlation = correlation_value
            max_index = i
    result = signal[max_index:]
    plt.figure(2, figsize=(10, 10))
    plt.title("graphe correlation")
    plt.plot(data_corr)
    return result

# Интерпретация сигнала
def interpret_signal(signal, N, data_length, crc_length, gold_sequence_length):
    temp = np.full(N, fill_value=float(0))
    temp_signal = np.copy(signal)
    result = np.full(data_length+crc_length+gold_sequence_length, fill_value=int(0))
    for i in range(data_length+crc_length+gold_sequence_length):
        for j in range(N):
            temp[j] = temp_signal[j]
        temp_signal = np.roll(temp_signal, -N)
        if np.mean(temp) >= 0.5:
            result[i] = 1
        else:
            result[i] = 0
    return result[gold_sequence_length:]

user_data = encode_to_binary(full_name)
array_length = len(user_data)

polynomial = [1, 1, 1, 1, 1, 1, 1]
crc_length = len(polynomial)
extended_data_length = array_length + crc_length

extended_data = np.full(extended_data_length, fill_value=int(0))
for i in range(array_length):
    extended_data[i] = user_data[i]

crc = np.full(crc_length, fill_value=int(0))
calculate_crc(polynomial, extended_data, crc, crc_length, array_length)

for i in range(array_length, extended_data_length):
    extended_data[i] = crc[i-array_length]

gold_sequence_length = 2**5 - 1
gold_sequence = np.full(gold_sequence_length, fill_value=int(0))
register_x = [0, 0, 1, 1, 0]
register_y = [0, 1, 1, 0, 1]
generate_gold_sequence(register_x, register_y, gold_sequence, gold_sequence_length)

extended_data_full = np.concatenate((gold_sequence, extended_data), axis=0)

N = 10
samples = np.repeat(extended_data_full, N)
signal_length = len(samples)

t = np.arange(0, array_length)
t2 = np.arange(0, extended_data_length + gold_sequence_length)
t3 = np.arange(0, signal_length)

plt.figure(figsize=(13, 20))
plt.subplot(3, 1, 1)
plt.plot(t, user_data)
plt.xlabel('array element')
plt.ylabel('value')
plt.title("Data")

plt.subplot(3, 1, 2)
plt.plot(t2, extended_data_full)
plt.xlabel('array element')
plt.ylabel('value')
plt.title("Gold+Data+CRC")

plt.subplot(3, 1, 3)
plt.plot(t3, samples)
plt.xlabel('array element')
plt.ylabel('value')
plt.title("Samples")

signal = np.full(2*N*(extended_data_length + gold_sequence_length), fill_value=float(0))

print("Enter a number from 0 to", len(samples))
offset = int(input(">> "))

for i in range(offset, offset+len(samples)):
    signal[i] = samples[i-offset]

signal_save = np.copy(signal)

q = float(input("Enter q >> "))
noise = np.random.normal(0, q, len(signal))

for i in range(len(signal)):
    signal[i] = signal[i] + noise[i]

ex_gold_sequence = np.repeat(gold_sequence, N)
correlated_signal = correlation(ex_gold_sequence, signal, signal_length)

t4 = np.arange(0, len(samples)*2)
t5 = np.arange(0, len(correlated_signal))

# Создание графиков для сигнала, шума и их комбинации
plt.figure(3, figsize=(13, 20))
plt.subplot(4, 1, 1)
plt.plot(t4, signal_save)
plt.xlabel('array element')
plt.ylabel('value')
plt.title("Signal")

plt.subplot(4, 1, 2)
plt.plot(t4, noise)
plt.xlabel('array element')
plt.ylabel('value')
plt.title("Noise")

plt.subplot(4, 1, 3)
plt.plot(t4, signal)
plt.xlabel('array element')
plt.ylabel('value')
plt.title("Signal+Noise")

plt.subplot(4, 1, 4)
plt.plot(t5, correlated_signal)
plt.xlabel('array element')
plt.ylabel('value')
plt.title("Cut Signal")

# Интерпретация коррелированного сигнала
return_signal = interpret_signal(correlated_signal, N, array_length, crc_length, gold_sequence_length)
return_crc = np.full(crc_length, fill_value=int(0))
calculate_crc(polynomial, return_signal, return_crc, crc_length, array_length)

# Проверка наличия ошибок
if np.mean(return_crc) > 0:
    print("Errors found")
else:
    print("No errors found")
    return_data = return_signal[0:array_length]
    decoded_name = decode_to_string(return_data)
    print("\n\nReceived data:", decoded_name)

ch = 0
    
# Расчет спектра передаваемого сигнала и сигнала с шумом
spect_signal = np.fft.fft(signal_save, 1000)
spect_noise_signal = np.fft.fft(signal, 1000)



if ch == 1:
    spect_signal = np.fft.fftshift(spect_signal)
    spect_noise_signal = np.fft.fftshift(spect_noise_signal)

# Расчет частотных значений
fs = 1000  # Замените на ваше значение частоты дискретизации
freq_signal = np.fft.fftfreq(len(spect_signal), 1/fs)
freq_noise_signal = np.fft.fftfreq(len(spect_noise_signal), 1/fs)

# Визуализация спектра передаваемого и принятого сигнала с шумом
plt.figure(4, figsize=(13, 20))
plt.subplot(2, 1, 1)
plt.plot(freq_signal+200, np.abs(spect_signal), color='green')
plt.xlabel('Frequency (Hz)')
plt.ylabel('Amplitude')
plt.title("Spectrum of transmitted signal with N = 10")
plt.xlim(0, 400)
plt.subplot(2, 1, 2)
plt.plot(freq_noise_signal+200, np.abs(spect_noise_signal), color='brown')
plt.xlabel('Frequency (Hz)')
plt.ylabel('Amplitude')
plt.title("Spectrum of received signal with noise and N = 10")
plt.xlim(0, 400)
samples_4N = np.repeat(extended_data_full, 4)
samples_16N = np.repeat(extended_data_full, 16)
signal_4N = np.full(2 * 4 * (extended_data_length + gold_sequence_length), fill_value=float(0))
signal_16N = np.full(2 * 16 * (extended_data_length + gold_sequence_length), fill_value=float(0))

for i in range(0, len(samples_4N)):
    signal_4N[i] = samples_4N[i]
for i in range(0, len(samples_16N)):
    signal_16N[i] = samples_16N[i]

# Расчет спектра для различных вариантов N
spect_signal_4N = np.fft.fft(signal_4N)
spect_signal_16N = np.fft.fft(signal_16N)

if ch == 1:
    spect_signal_4N = np.fft.fftshift(spect_signal_4N)
    spect_signal_16N = np.fft.fftshift(spect_signal_16N)

# Генерация шума для сигналов с различными N
noise_4N = np.random.normal(0, q, len(signal_4N))
signal_4N += noise_4N

noise_16N = np.random.normal(0, q, len(signal_16N))
signal_16N += noise_16N

# Расчет спектра для сигналов с различными N и шумом
spect_noise_signal_4N = np.fft.fft(signal_4N)
spect_noise_signal_16N = np.fft.fft(signal_16N)

if ch == 1:
    spect_noise_signal_4N = np.fft.fftshift(spect_noise_signal_4N)
    spect_noise_signal_16N = np.fft.fftshift(spect_noise_signal_16N)

# Расчет частотных значений
freq_signal_4N = np.fft.fftfreq(len(spect_signal_4N), 1/fs) + 200
freq_signal_16N = np.fft.fftfreq(len(spect_signal_16N), 1/fs) + 200
freq_noise_signal_4N = np.fft.fftfreq(len(spect_noise_signal_4N), 1/fs) + 200
freq_noise_signal_16N = np.fft.fftfreq(len(spect_noise_signal_16N), 1/fs) + 200

t2 = np.arange(0, len(spect_noise_signal_4N))
t3 = np.arange(0, len(spect_noise_signal_16N))

# Визуализация спектра для сигналов с различными N и шумом
plt.figure(5, figsize=(13, 20))
plt.subplot(2, 1, 1)
plt.plot(freq_signal_16N, np.abs(spect_signal_16N), color='blue')
plt.plot(freq_signal+200, np.abs(spect_signal), color='magenta')
plt.plot(freq_signal_4N, np.abs(spect_signal_4N), color='green')
plt.xlabel('Frequency (Hz)')
plt.ylabel('Amplitude')
plt.title("Spectrum of transmitted signal with N = 8, N=4, N = 16")
plt.xlim(0, 400)
plt.subplot(2, 1, 2)
plt.plot(freq_noise_signal_16N, np.abs(spect_noise_signal_16N), color='blue')
plt.plot(freq_noise_signal+200, np.abs(spect_noise_signal), color='magenta')
plt.plot(freq_noise_signal_4N, np.abs(spect_noise_signal_4N), color='green')

plt.xlabel('Frequency (Hz)')
plt.ylabel('Amplitude')
plt.title("Spectrum of received signal with noise and N = 8, N=4, N = 16")
plt.xlim(0, 400)
plt.show()
