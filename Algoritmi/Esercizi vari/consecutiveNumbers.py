
def numeriConsecutivi(numbs, i):
    if len(numbs) <= i:
        return 0
    if numbs[i] == numbs[i-1]+1:
        return 1 + numeriConsecutivi(numbs, i+1)
    return numeriConsecutivi(numbs, i+1)
    

numbers = [12, 13, 24, 25, 26, 35, 67]
print(numeriConsecutivi(numbers, 1))