import numpy as np

def decimalToHex(input):
  ciphertext_postprocessing = [[] for i in range(4)]

  for i in range(4):
    for j in range(4):
      ciphertext_postprocessing[i].append(hex(int(input[i][j])))

  return np.array(ciphertext_postprocessing)

def hexToDecimal(input):
  ciphertext_postprocessing = [[] for i in range(4)]

  for i in range(4):
    for j in range(4):
      ciphertext_postprocessing[i].append(int(input[i][j], 16))

  return np.array(ciphertext_postprocessing)


class AES128:
  def __init__(self):
    self.sbox = np.array([
      #0     1    2      3     4    5     6     7      8    9     A      B    C     D     E     F
      0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76,
      0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0,
      0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15,
      0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75,
      0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84,
      0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf,
      0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8,
      0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2,
      0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73,
      0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb,
      0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79,
      0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08,
      0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a,
      0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e,
      0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf,
      0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16]).reshape(16, 16)
    
    self.rsbox = np.array([
      0x52, 0x09, 0x6a, 0xd5, 0x30, 0x36, 0xa5, 0x38, 0xbf, 0x40, 0xa3, 0x9e, 0x81, 0xf3, 0xd7, 0xfb,
      0x7c, 0xe3, 0x39, 0x82, 0x9b, 0x2f, 0xff, 0x87, 0x34, 0x8e, 0x43, 0x44, 0xc4, 0xde, 0xe9, 0xcb,
      0x54, 0x7b, 0x94, 0x32, 0xa6, 0xc2, 0x23, 0x3d, 0xee, 0x4c, 0x95, 0x0b, 0x42, 0xfa, 0xc3, 0x4e,
      0x08, 0x2e, 0xa1, 0x66, 0x28, 0xd9, 0x24, 0xb2, 0x76, 0x5b, 0xa2, 0x49, 0x6d, 0x8b, 0xd1, 0x25,
      0x72, 0xf8, 0xf6, 0x64, 0x86, 0x68, 0x98, 0x16, 0xd4, 0xa4, 0x5c, 0xcc, 0x5d, 0x65, 0xb6, 0x92,
      0x6c, 0x70, 0x48, 0x50, 0xfd, 0xed, 0xb9, 0xda, 0x5e, 0x15, 0x46, 0x57, 0xa7, 0x8d, 0x9d, 0x84,
      0x90, 0xd8, 0xab, 0x00, 0x8c, 0xbc, 0xd3, 0x0a, 0xf7, 0xe4, 0x58, 0x05, 0xb8, 0xb3, 0x45, 0x06,
      0xd0, 0x2c, 0x1e, 0x8f, 0xca, 0x3f, 0x0f, 0x02, 0xc1, 0xaf, 0xbd, 0x03, 0x01, 0x13, 0x8a, 0x6b,
      0x3a, 0x91, 0x11, 0x41, 0x4f, 0x67, 0xdc, 0xea, 0x97, 0xf2, 0xcf, 0xce, 0xf0, 0xb4, 0xe6, 0x73,
      0x96, 0xac, 0x74, 0x22, 0xe7, 0xad, 0x35, 0x85, 0xe2, 0xf9, 0x37, 0xe8, 0x1c, 0x75, 0xdf, 0x6e,
      0x47, 0xf1, 0x1a, 0x71, 0x1d, 0x29, 0xc5, 0x89, 0x6f, 0xb7, 0x62, 0x0e, 0xaa, 0x18, 0xbe, 0x1b,
      0xfc, 0x56, 0x3e, 0x4b, 0xc6, 0xd2, 0x79, 0x20, 0x9a, 0xdb, 0xc0, 0xfe, 0x78, 0xcd, 0x5a, 0xf4,
      0x1f, 0xdd, 0xa8, 0x33, 0x88, 0x07, 0xc7, 0x31, 0xb1, 0x12, 0x10, 0x59, 0x27, 0x80, 0xec, 0x5f,
      0x60, 0x51, 0x7f, 0xa9, 0x19, 0xb5, 0x4a, 0x0d, 0x2d, 0xe5, 0x7a, 0x9f, 0x93, 0xc9, 0x9c, 0xef,
      0xa0, 0xe0, 0x3b, 0x4d, 0xae, 0x2a, 0xf5, 0xb0, 0xc8, 0xeb, 0xbb, 0x3c, 0x83, 0x53, 0x99, 0x61,
      0x17, 0x2b, 0x04, 0x7e, 0xba, 0x77, 0xd6, 0x26, 0xe1, 0x69, 0x14, 0x63, 0x55, 0x21, 0x0c, 0x7d]).reshape(16, 16)
    
    self.mixColumnMatrix = np.array([[2, 3, 1, 1], [1, 2, 3, 1], [1, 1, 2, 3], [3, 1, 1, 2]])

    self.inverseMixColumnMatrix = np.array([[14, 11, 13, 9], [9, 14, 11, 13], [13, 9, 14, 11], [11, 13, 9, 14]])

    self.roundConstant = np.array([0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1B, 0x36])

    self.gf28mod = [1,0,0,0,1,1,0,1,1]

    self.W = [[] for i in range(10)]
  
  def decimalToBin(self, input):
    if "x" in str(input):
      bin_string = str(bin(int(input, 16)))[2::]
    else:
      bin_string = str(bin(int(input)))[2::]
    bin_array = []
    # bin_string_len = len(bin_string)
    # while bin_string_len < 8:
    #   bin_array.append(0)
    #   bin_string_len += 1
    for i in range(len(bin_string)):
      bin_array.append(int(bin_string[i]))
    return bin_array
  
  def hexToDecimal(self, h):
    if h == "a":
      return 10
    elif h == "b":
      return 11
    elif h == "c":
      return 12
    elif h == "d":
      return 13
    elif h == "e":
      return 14
    elif h == "f":
      return 15
    else:
      return int(h)

  def subBytes(self, input):
    input_flatten = list(input.reshape(1, 16, order = "F")[0])
    for i in range(len(input_flatten)):
      if "x" in str(input_flatten[i]):
        input_byte = str(input_flatten[i])[2::]
      else:
        input_byte = str(hex(int(input_flatten[i])))[2::]
      if len(input_byte) == 1:
        input_byte = "0" + input_byte
      input_flatten[i] = self.sbox[self.hexToDecimal(input_byte[0])][self.hexToDecimal(input_byte[1])]
    return np.array(input_flatten).reshape(4, 4, order = "F")
  
  def inverseSubBytes(self, input):
    input_flatten = list(input.reshape(1, 16, order = "F")[0])
    for i in range(len(input_flatten)):
      if "x" in str(input_flatten[i]):
        input_byte = str(input_flatten[i])[2::]
      else:
        input_byte = str(hex(int(input_flatten[i])))[2::]
      if len(input_byte) == 1:
        input_byte = "0" + input_byte
      input_flatten[i] = self.rsbox[self.hexToDecimal(input_byte[0])][self.hexToDecimal(input_byte[1])]
    return np.array(input_flatten).reshape(4, 4, order = "F")

  def shiftRows(self, input):
    for i in range(4):
      if i == 0:
        continue
      elif i == 1:
        input[i][[0, 1]] = input[i][[1, 0]]
        input[i][[1, 2]] = input[i][[2, 1]]
        input[i][[2, 3]] = input[i][[3, 2]]
      elif i == 2:
        input[i][[0, 2]] = input[i][[2, 0]]
        input[i][[1, 3]] = input[i][[3, 1]] 
      elif i == 3:
        input[i][[2, 3]] = input[i][[3, 2]]
        input[i][[1, 2]] = input[i][[2, 1]]
        input[i][[0, 1]] = input[i][[1, 0]]
    return input
  
  def inverseShiftRows(self, input):
    for i in range(4):
      if i == 0:
        continue
      elif i == 1:
        input[i][[2, 3]] = input[i][[3, 2]]
        input[i][[1, 2]] = input[i][[2, 1]]
        input[i][[0, 1]] = input[i][[1, 0]]
      elif i == 2:
        input[i][[0, 2]] = input[i][[2, 0]]
        input[i][[1, 3]] = input[i][[3, 1]]
      elif i == 3:
        input[i][[0, 1]] = input[i][[1, 0]]
        input[i][[1, 2]] = input[i][[2, 1]]
        input[i][[2, 3]] = input[i][[3, 2]]
    return input
  
  def XORw(self, w1, w2):
    w = []
    for i in range(4):
      w.append(hex(w1[i] ^ w2[i]))
    return np.array(w)

  def byte(self, x, n=8):
    return format(x, f"0{n}b")

  def GF_2_8_mul(self, input, input_from_mcm):
    a = input
    b = input_from_mcm
    if "x" in str(a):
      a = int(a, 16)
    if "x" in str(b):
      b = int(b, 16)
    tmp = 0
    b_byte = bin(b)[2:]
    for i in range(len(b_byte)):
        tmp = tmp ^ (int(b_byte[-(i+1)]) * (a << i))

    mod = int("100011011", 2)
    exp = len(bin(tmp)[2:])
    diff =  exp - len(bin(mod)[2:]) + 1

    for i in range(diff):
        if self.byte(tmp, exp)[i] == "1":
            tmp = tmp ^ (mod << diff - i - 1)
    return tmp

  def mixColumn(self, input):
    input = input.reshape(1, 16, order = "F").reshape(4, 4)
    b = np.zeros((4,4))
    for k in range(4):
      for i in range(4):
        gf28mul = []
        for j in range(4):
          gf28mul.append(self.GF_2_8_mul(input[k][j], self.mixColumnMatrix[i][j]))

        b[k][i] = gf28mul[0] ^ gf28mul[1] ^ gf28mul[2] ^ gf28mul[3]

    return b.reshape(1, 16, order = "F").reshape(4, 4)
  
  def inverseMixColumn(self, input):
    input = input.reshape(1, 16, order = "F").reshape(4, 4)
    b = np.zeros((4,4))
    for k in range(4):
      for i in range(4):
        gf28mul = []
        for j in range(4):
          gf28mul.append(self.GF_2_8_mul(input[k][j], self.inverseMixColumnMatrix[i][j]))

        b[k][i] = gf28mul[0] ^ gf28mul[1] ^ gf28mul[2] ^ gf28mul[3]
    return b.reshape(1, 16, order = "F").reshape(4, 4)

  def addRoundKeys(self, text, key):
    ark = []
    for i in range(4):
      ark.append(self.XORw(self.binhexToDecimal(text[i]), self.binhexToDecimal(key[i])))
    return np.array(ark)
  
  def checkPlainText(self, plaintext):
    if len(plaintext) != 16:
      print("Plaintext should have length of 16")
      quit()

  def reshape(self, target):
    byte = []

    for i in range(len(target)):
      byte.append(hex(target.encode()[i]))

    return np.array(byte).reshape(4, 4, order = "F")
  
  def g(self, w, round):
    w[[0, 1]] = w[[1, 0]]
    w[[1, 2]] = w[[2, 1]]
    w[[2, 3]] = w[[3, 2]]

    for i in range(len(w)):
      w_byte = str(w[i])[2::]
      if len(w_byte) == 1:
        w_byte = "0" + w_byte
      w[i] = self.sbox[self.hexToDecimal(w_byte[0])][self.hexToDecimal(w_byte[1])]

    t = [int(w[0]), int(w[1]), int(w[2]), int(w[3])]

    return self.XORw(t, np.array([self.roundConstant[round], 0, 0, 0]))
  
  def binhexToDecimal(self, input):
    temp = []
    for i in range(len(input)):
      if "x" in str(input[i]):
        temp.append(int(str(input[i]), 16))
      else:
        temp.append(int(input[i]))
    return np.array(temp)


  def keySchedulingPart(self, key, round):
    w0 = np.array([key[0][0], key[1][0], key[2][0], key[3][0]])
    w1 = np.array([key[0][1], key[1][1], key[2][1], key[3][1]])
    w2 = np.array([key[0][2], key[1][2], key[2][2], key[3][2]])
    w3 = np.array([key[0][3], key[1][3], key[2][3], key[3][3]])

    w4 = self.XORw(self.binhexToDecimal(w0), self.binhexToDecimal(self.g(w3, round - 1)))
    w5 = self.XORw(self.binhexToDecimal(w4), self.binhexToDecimal(w1))
    w6 = self.XORw(self.binhexToDecimal(w5), self.binhexToDecimal(w2))
    w7 = self.XORw(self.binhexToDecimal(w6), self.binhexToDecimal(w3))

    return np.array([w4, w5, w6, w7]).reshape(1, 16).reshape(4, 4, order = "F")
  
  def keySchedulingAll(self, key_matrix):
    w0 = np.array([key_matrix[0][0], key_matrix[1][0], key_matrix[2][0], key_matrix[3][0]])
    w1 = np.array([key_matrix[0][1], key_matrix[1][1], key_matrix[2][1], key_matrix[3][1]])
    w2 = np.array([key_matrix[0][2], key_matrix[1][2], key_matrix[2][2], key_matrix[3][2]])
    w3 = np.array([key_matrix[0][3], key_matrix[1][3], key_matrix[2][3], key_matrix[3][3]])

    W = [np.array([w0, w1, w2, w3])]
    for i in range(10):
      key_matrix = self.keySchedulingPart(key_matrix, i + 1)
      W.append(key_matrix)

    return W
  
  def preprocess(self, input):
    return self.reshape(input)

  def encrypt(self, key, plaintext):
    key_matrix = self.preprocess(key)
    plaintext_matrix = self.preprocess(plaintext)
    # plaintext_matrix = plaintext
    
    W = self.keySchedulingAll(key_matrix)

    cipher_text = plaintext_matrix


    cipher_text = self.addRoundKeys(cipher_text, W[0])
  
    for i in range(1, 10):
      cipher_text =  self.addRoundKeys(self.mixColumn(self.shiftRows(self.subBytes(cipher_text))), W[i])

    cipher_text =  self.addRoundKeys(self.shiftRows(self.subBytes(cipher_text)), W[10])
    
    return cipher_text

  def decrypt(self, key, ciphertext):
    key_matrix = self.preprocess(key)
    ciphertext_matrix = ciphertext

    plaintext = ciphertext_matrix

    W = self.keySchedulingAll(key_matrix)

    plaintext = self.inverseSubBytes(self.inverseShiftRows(self.addRoundKeys(plaintext, W[10])))

    for i in range(9, 0, -1):
      plaintext = self.inverseSubBytes(self.inverseShiftRows(self.inverseMixColumn(self.addRoundKeys(plaintext, W[i]))))

    plaintext = self.addRoundKeys(plaintext, W[0])

    return plaintext

    


plaintext = "IamPhucnicetosee"
key = "abcabcabcabcabcx"

# p_test = np.array([[0x47, 0x40, 0xa3, 0x4c], [0x37, 0xd4, 0x70, 0x9f], [0x94, 0xe4, 0x3a, 0x42], [0xed, 0xa5, 0xa6, 0xbc]])
# p_test = np.array([[0x63, 0xeb, 0x9f, 0xa0], [0x2f, 0x93, 0x92, 0xc0], [0xaf, 0xc7, 0xab, 0x30], [0xa2, 0x20, 0xcb, 0x2b]])
# p_test = np.array([[0x00, 0x3c, 0x6e, 0x47], [0x1f, 0x4e, 0x22, 0x74], [0x0e, 0x08, 0x1b, 0x31], [0x54, 0x59, 0x0b, 0x1a]])

aes128 = AES128()

print("Plaintext:", aes128.preprocess(plaintext))

ciphertext = aes128.encrypt(key, plaintext)

print("Ciphertext:", ciphertext)

decrypted = aes128.decrypt(key, ciphertext)

print("\nDecrypted:", ''.join([chr(int(d, 16)) for d in decrypted.reshape(1, 16, order = "F")[0]]))
