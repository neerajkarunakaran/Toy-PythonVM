import sys, os, subprocess

def factorial_test(list):
    """function will test the 
    factorial.pyc output """ 
    text = list[0]
    expected = 479001600
    if int(text.strip()) == expected:
        print "factorial test: passed"
    else:
        print "factorial test: failed"

def factor_test(list):
    """function will test the
    factor_number.pyc output"""
    result = []
    for text in list[1:]:
        result.append(int(text.strip()))
    expected = [1,2,4,5,10,20,25,50,100]
    if result == expected:
        print "factor_number test: passed"
    else:
        print "factor_number test: failed"

def largest_test(list):
    """function will test the 
    large.pyc output """
    text = list[1]
    expected = 3234
    if int(text.strip()) == expected:
        print "largest test: passed"
    else:
        print "largest test: failed"

def sumof_digit_test(list):
    """function will test the
    sumof_digits.pyc output"""
    text = list[0]
    expected = 25
    if int(text.strip()) == expected:
        print "sumof_digits test: passed"
    else:
        print "sumof_digits test: failed"

def mul_test(list):
    """function will test the
    mul.pyc output"""
    text = list[0]
    expected = 225
    if int(text.strip()) == expected:
        print "mul test: passed"
    else:
        print "mul test: failed"

def  recursion_test(list):
    """function will test the 
    recursion.pyc output"""
    text = list[0]
    expected = "This is loop iteration1"
    if text.strip() == expected:
        print "recursion test: passed"
    else:
        print "recursion test: failed"

def checkprime_test(list):
    """function will test the 
    checkprime.pyc output"""
    text = list[0]
    expected = "Number is not prime"
    if text.strip() == expected:
        print "check prime test: passed"
    else:
        print "check prime test: failed"


if __name__ == '__main__':


    filename = sys.argv[1]    
f = subprocess.Popen(["/home/neeraj/neerajgit/pyvm/pyvm", \
    filename], stdout = subprocess.PIPE)
list = f.stdout.readlines()
file = filename.split(".")
if file[0] == "checkprime":
    checkprime_test(list[3:])
elif file[0] == "factor_number":
    factor_test(list[3:])
elif file[0] == "large":
    largest_test(list[3:])
elif file[0] == "sumof_digits":
    sumof_digit_test(list[3:])
elif file[0] == "factorial":
    factorial_test(list[3:])
elif file[0] == "mul":
    mul_test(list[3:])
else:
    recursion_test(list[3:])
    
