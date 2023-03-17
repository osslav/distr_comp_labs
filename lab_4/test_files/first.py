from random import random
import math
import array
import numpy as np

#critical val for alpha = 0,05
arr_critical_val = array.array('f', [0, 3.84, 5.99,7.81, 9.48, 11.07, 12.59, 14.06, 15.5, 16.91, 18.3, 19.6, 21.02, 22.36, 23.68, 24.99, 26.29, 27.58, 28.86, 30.14, 31.41])

#random() generates float in the half-open range 0.0 <= X < 1.0.
def randSensor():
    ans = 0.0
    while ans == 0.0:
        ans = random()
    return ans

def checkSensor(func_sensor, count_num):
    k = math.floor(1 + 3.33 * math.log10(count_num))
    print(f"k = {k}")
    arr_inter = np.zeros(k)
    for i in range(count_num):
        arr_inter[math.floor(func_sensor() * k)] += 1

    #for i in range(k):
        #print(f"interval #{i} = {arr_inter[i]}")
    
    count_div_k = count_num / k
    ans = 0
    for i in range(k):
        ans += ((arr_inter[i] - count_div_k)**2)/count_div_k

    print(f"x2 = {ans:5.2f}")
    if k-1 <= 20:
        if arr_critical_val[k-1] >= ans:
            print(f"Good sampling (critical value = {arr_critical_val[k-1]:5.2f})")
        else:
            print(f"Bad samplig (critical value = {arr_critical_val[k-1]:5.2f})")

    return ans


count = int(input("Enter count numbers in your sampling: "))

for i in range(1):
    checkSensor(randSensor, count)

