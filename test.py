from random import randint
import numpy as np
k = 5
n = 1200 

for k in range(5,505,5):
    playerList = [i for i in range(n)]
    permutation = np.arange(n) 
    np.random.shuffle(permutation)

    while len(playerList) > 1:
        i1 = randint(0,len(playerList)-1)
        a1 = permutation[playerList[i1]]
        availablePairs = []
        for i in range(1,k):
            p = playerList[(i1+i)%len(playerList)]-playerList[i1]
            if not (n-k > p > k) and p:
                availablePairs.append((i1+i)%len(playerList))
            p = playerList[i1]-playerList[(i1-i)%len(playerList)]
            if not (n-k > p > k) and p:
                availablePairs.append((i1-i)%len(playerList))
            # p = playerList[(i1+i)%len(playerList)]-playerList[i1]
            # if p < 0:
            #     p += n
            # if p <= k and p != 0:
            #     availablePairs.append((i1+i)%len(playerList))
            # p = playerList[i1]-playerList[(i1-i)%len(playerList)]
            # if p < 0:
            #     p += n
            # if p <= k and p != 0:
            #     availablePairs.append((i1-i)%len(playerList))
        if len(availablePairs) == 0:
            del playerList[i1]
        else:
            i2 = availablePairs[randint(0,len(availablePairs)-1)]
            a2 = permutation[playerList[i2]]
            # print(playerList[i1], playerList[i2])
            assert i1 != i2, f"i1: {i1}, i2: {i2}"
            del playerList[i1]
            if i2 > i1: 
                del playerList[i2-1]
            else:
                del playerList[i2]
