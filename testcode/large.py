def large(n1, n2, n3):
    if n1 >= n2 and n1 >= n3:
        large = n1
    elif n2 >= n1 and n2 >= n3:
        large = n2
    else:
        large = n3
    print "largest is"
    print large
large(1234,2000,3234)
