
def recurr(loopnum,counter):
    if loopnum > 0:
        print "This is loop iteration",counter
        recurr(loopnum - 1,counter + 1)
    else:
        print "The loop is complete."

recurr(10, 1)
