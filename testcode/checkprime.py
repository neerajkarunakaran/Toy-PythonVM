

def checkprime(n):
    x = n // 2
    k = 0
    i = 2
    while i < x:
        i+=1
        if n % i == 0:
            k = k + 1
    if k <= 0:
        print "Number is prime"
    else:
        print "Number is not prime"

checkprime(100)
