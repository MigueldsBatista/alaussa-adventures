string="0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 "
count=0
for char in string:
    if char == '0':
        count = count + 1
print(count)