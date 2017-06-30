def sumof_digits(n):
    x = 0
    while n:
        x = x + n % 10
        n = n // 10
    return x
print sumof_digits(6954)
