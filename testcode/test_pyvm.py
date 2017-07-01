import sys, os, subprocess

def factorial_test(list):
    """function will test the 
    factorial.pyc output """ 
    text = list[0]
    expected = 3628800
    if int(text.strip()) == expected:
        print "factorial test: passed"
    else:
        print "factorial test: failed ! expected: %d got: %s" %(expected, text.strip())
                
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
        print "factor_number test: failed ! expected: %s got : %s" %(str(expected), str(result))

def largest_test(list):
    """function will test the 
    large.pyc output """
    text = list[1]
    expected = 3234
    if int(text.strip()) == expected:
        print "largest test: passed"
    else:
        print "largest test: failed! expected: %d got: %s" %(expected, textstrip())

def sumof_digit_test(list):
    """function will test the
    sumof_digits.pyc output"""
    text = list[0]
    expected = 24
    if int(text.strip()) == expected:
        print "sumof_digits test: passed"
    else:
        print "sumof_digits test: failed! expected: %d got: %s" %(expected, text.strip())

def mul_test(list):
    """function will test the
    mul.pyc output"""
    text = list[0]
    expected = 225
    if int(text.strip()) == expected:
        print "mul test: passed"
    else:
        print "mul test: failed! expected: %d got: %s " %(expected, text.strip())

def  recursion_test(list):
    """function will test the 
    recursion.pyc output"""
    text = list[0]
    expected = "This is loop iteration1"
    if text.strip() == expected:
        print "recursion test: passed"
    else:
        print "recursion test: failed! expected: %s got: %s" %(expected, text.strip())

def checkprime_test(list):
    """function will test the 
    checkprime.pyc output"""
    text = list[0]
    expected = "Number is not prime"
    if text.strip() == expected:
        print "check prime test: passed"
    else:
        print "check prime test: failed! expeced: %s got: %s" %(expected, text.strip())

def argcount_test(list):
    """ function will test the
    argcountest.pyc output"""
    text = list[0]
    expected = 350
    if int(text.strip()) == expected:
        print "argcount test : passed"
    else:
        print "argcount test :failed! expected: %d got: %s" %(expected, text.strip())

def printhello_test(list):
    """ function will test the 
    printhello.pyc output"""
    text = list[0]
    expected = "hello world !"
    if text.strip() == expected:
        print "print hello test: passed"
    else:
        print "print hello test: failed! expected: %s got: %s" %(expected, text.strip())

def whileloop_test(list):
    """function will test the 
    whileloop.pyc output"""
    result = []
    expected = range(1, 50)
    for text in list:
        result.append(int(text.strip()))
    if result == expected:
        print "while loop test: passed"
    else:
        print "while loop test: failed! expected: %s got: %s" %(str(expected), str(result))

if __name__ == '__main__':

    files = os.listdir("/home/neeraj/neerajgit/pyvm/testcode")
    for filename in files:
        if filename.endswith(".pyc"):
   
            try: 
                f = subprocess.Popen(["/home/neeraj/neerajgit/pyvm/pyvm", \
                                filename], stdout = subprocess.PIPE)
            except:
                print "error"
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
            elif file[0] == "recursion":
                recursion_test(list[3:])
            elif file[0] == "argcountest":
                argcount_test(list[3:])
            elif file[0] == "printhello":
                printhello_test(list[3:])
            elif file[0] == "whileloop":
                whileloop_test(list[3:])
    
