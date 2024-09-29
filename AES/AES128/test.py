import numpy as np

def decimalToBin(input):
  bin_string = str(bin(input))[2::]
  bin_array = []
  bin_string_len = len(bin_string)
  while bin_string_len < 8:
    bin_array.append(0)
    bin_string_len += 1
  for i in range(len(bin_string)):
    bin_array.append(int(bin_string[i]))
  return bin_array

def GF_2_8_mul(input, input_from_mcm):
  time = input_from_mcm // 2
  plus_1 = input_from_mcm % 2
  gf28mul = 0
  input_ = decimalToBin(input)
  for i in range(time):
    if input_[0] == 0:
      input_.pop(0)
      input_.append(0)
      gf28mul ^= np.poly1d(input_)(2)
    else:
      input_.pop(0)
      input_.append(0)
      gf28mul ^= np.poly1d(input_)(2) ^ 27   
  
  if plus_1 == 1:
    gf28mul ^= input

  return gf28mul

def mixColumn(input):
  input = input.reshape(1, 16).reshape(4, 4, order = "F")
  b = np.zeros((4,4))
  for i in range(4):
    gf28mul = []
    for j in range(4):
      gf28mul.append(GF_2_8_mul(input[j], mixColumnMatrix[i][j]))

    b[i] = hex(gf28mul[0] ^ gf28mul[1] ^ gf28mul[2] ^ gf28mul[3])
    print(gf28mul)
  return b

mixColumnMatrix = np.array([[0x02, 0x03, 0x01, 0x01], [0x01, 0x02, 0x03, 0x01], [0x01, 0x01, 0x02, 0x03], [0x03, 0x01, 0x01, 0x02]])

a = [0x63,0x2f,0xaf,0xa2]

print(mixColumn(a))

print(GF_2_8_mul(83, 1))


# print(np.poly1d(decimalToBin(a[0])))
# print(np.poly1d(decimalToBin(mixColumnMatrix[0][0])))

# print(np.array(np.polymul(np.poly1d(decimalToBin(a[0])), np.poly1d(decimalToBin(mixColumnMatrix[0][0])))))

