def mult(n, mul):
    "print multiple of numbers byr recursion"
    if n == 1:
        return mul
    else:
        return mult(n-1, mul) + mul
   
print mult(5, 45)
