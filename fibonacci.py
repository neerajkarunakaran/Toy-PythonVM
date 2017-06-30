
def fibonacci(n):
    i = 0;
    x = 0
    y = 1
    while i < n: 
        if i <= 1:
            n1 = i
        else:
            n1 = x + y
            x = y
            y = n1
        print n1
        i = i + 1




fibonacci(10)

