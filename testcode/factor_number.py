def factors_number(n):
    print "factors of number are..."
    x = n + 1
    i = 1
    while i < x:
        if n % i == 0:
            print i
        i = i + 1
factors_number(100)
